/**
 * @fileoverview Script for bundling the client into a single html file,
 * which is then placed in a string in /Arduino/client.h.
 * Run with `deno task build`.
 */

import {rollup} from "npm:rollup@4.44.1";
import replace from "npm:@rollup/plugin-replace@4.0.0";
import {setCwd} from "https://deno.land/x/chdir_anywhere@v0.0.2/mod.js";
import {resolve, dirname} from "https://deno.land/std@0.146.0/path/mod.ts";
import postcss from "https://deno.land/x/postcss@8.4.13/mod.js";
import * as esbuild from "https://deno.land/x/esbuild@v0.14.48/mod.js"
// @ts-ignore
import CleanCSS from "$clean-css";
import { cssImportAttributesPlugin } from "./rollup-css-import-attribute.js";
setCwd();

function postCssInlineUrlsPlugin() {
	const urlRegex = /url\("?(?!#)(.+)"?\)/d;
	const plugin = {
		postcssPlugin: "postcss-inline-urls",
		// @ts-ignore
		async Declaration(decl, {result}) {
			const from = result.opts.from;
			if (!from) {
				throw new Error("Assertion failed, from is undefined");
			}
			const match = decl.value.match(urlRegex);
			if (match && match[1]) {
				const url = match[1];
				if (url.startsWith("data:")) {
					// Already inline.
					return;
				}
				await new Promise(r => setTimeout(r, 100));

				const filePath = resolve(dirname(from), url);
				const file = await Deno.readTextFile(filePath);
				const base64 = btoa(file);
				let mediatype = "text/plain";
				if (url.endsWith(".svg")) {
					mediatype = "image/svg+xml";
				}
				decl.value = `url(data:${mediatype};base64,${base64})`;
			}
		}
	}
	return plugin;
}

function postCssPlugin() {
	return {
		name: "postcss",
		/**
		 * @param {string} code
		 * @param {string} id
		 */
		async transform(code, id) {
			if (id.endsWith(".css")) {
				const processor = postcss([
					postCssInlineUrlsPlugin(),
				]);
				// @ts-ignore
				const result = await processor.process(code, {
					from: id,
					to: id,
				});
				return result.css;
			}
		}
	}
}

function cleanCssPlugin() {
	const cleanCss = new CleanCSS();
	return {
		name: "clean-css",
		/**
		 * @param {string} code
		 * @param {string} id
		 */
		transform(code, id) {
			if (id.endsWith(".css")) {
				const minified = cleanCss.minify(code);
				return minified.styles;
			}
		}
	}
}

console.log("Building client...");
const bundle = await rollup({
	input: "../src/main.js",
	onwarn: message => {
		if (message.code == "CIRCULAR_DEPENDENCY") return;
		console.error(message.message);
	},
	plugins: [
		postCssPlugin(),
		replace({
			values: {
				DEBUG_BUILD: "false",
			},
			preventAssignment: true,
		}),
		cleanCssPlugin(),
		cssImportAttributesPlugin(),
	],
});
const {output} = await bundle.generate({
	format: "esm",
});

let htmlContent = await Deno.readTextFile("../src/index.html");

// Find "./main.js"
const mainJsIndex = htmlContent.indexOf("./main.js");
// Find the first "<script" occurrence before it
const scriptStartIndex = htmlContent.lastIndexOf("<script", mainJsIndex);
// Find the first "</script>" occurrence after it
const scriptEndIndex = htmlContent.indexOf("</script>", scriptStartIndex) + "</script>".length;
// Remove the script tag
htmlContent = htmlContent.slice(0, scriptStartIndex) + htmlContent.slice(scriptEndIndex);

{
	console.log("Minifying js...");
	const esbuildResult = await esbuild.transform(output[0].code, {
		loader: "js",
		minify: true,
	});
	const minifiedJs = esbuildResult.code;

	// Inject an inline script tag with the build output before "<!--inline main.js inject position-->"
	const injectIndex = htmlContent.indexOf("<!--inline main.js inject position-->");
	htmlContent = htmlContent.slice(0, injectIndex) + `<script>${minifiedJs}</script>` + htmlContent.slice(injectIndex);
}

{
	console.log("Injecting svgs in html...");
	/** @type {Map<string, string>} */
	const svgs = new Map();
	for await (const dirEntry of Deno.readDir("../svg")) {
		if (dirEntry.isFile && dirEntry.name.endsWith(".svg")) {
			const id = dirEntry.name.slice(0, -4);
			const file = await Deno.readTextFile(`../svg/${dirEntry.name}`);
			svgs.set(id, file);
		}
	}

	let svgsContent = "";
	for (const [id, svg] of svgs) {
		let svgContent = svg;

		// Remove the xml tag from the svg
		const xmlTagStart = svg.indexOf("<?xml");
		if (xmlTagStart >= 0) {
			const xmlTagEnd = svg.indexOf(">", xmlTagStart);
			svgContent = svgContent.slice(0, xmlTagStart) + svgContent.slice(xmlTagEnd + 1);
		}

		// wrap the svg content in a <template>
		svgContent = `<template id="${id}">${svgContent}</template>`;

		svgsContent += svgContent;
	}

	const injectIndex = htmlContent.indexOf("<!--inline svgs inject position-->");
	htmlContent = htmlContent.slice(0, injectIndex) + svgsContent + htmlContent.slice(injectIndex);
}

// Remove new lines
htmlContent = htmlContent.replaceAll("\r\n", "").replaceAll("\n", "");

// Write the output to dist.html so that we debug the client without having to build a new esp binary.
console.log("writing to dist.html");
await Deno.writeTextFile("../dist.html", htmlContent);

// Generate the Arduino clienthtml.h file
const clientH = `// This file is automatically generated by /Website/scripts/build.js
// To update it, run \`deno task build\`.

const String HTML = "${htmlContent.replaceAll('"', '\\"')}";
`;

console.log("writing to clienthtml.h");
await Deno.writeTextFile("../../Arduino/Primary/clienthtml.h", clientH);
console.log("done!");
Deno.exit();
