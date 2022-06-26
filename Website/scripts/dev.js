#!/usr/bin/env -S deno run --allow-env --allow-run --allow-read --allow-write --allow-net --no-check --unstable

/**
 * @fileoverview
 * Starts a local server for development.
 * This mostly serves files in the Website directory but also serves some extra
 * requests that the ESP would normally handle.
 */

import {serveDir} from "https://deno.land/std@0.145.0/http/file_server.ts";
import {Server} from "https://deno.land/std@0.145.0/http/server.ts";
import {resolve, fromFileUrl, dirname} from "https://deno.land/std@0.145.0/path/mod.ts";
import {generateTypes} from "https://deno.land/x/deno_tsc_helper@v0.0.1/generateTypes.js";

await generateTypes({
	typeUrls: [
		"https://deno.land/x/deno_tsc_helper@v0.0.1/generateTypes.js",
		"https://deno.land/std@0.145.0/http/file_server.ts",
		"https://deno.land/std@0.145.0/http/server.ts",
		"https://deno.land/std@0.145.0/path/mod.ts",
	],
	outputDir: "../.denoTypes",
});

const server = new Server({
	port: 8080,
	handler: request => {
		const url = new URL(request.url);
		if (url.pathname == "/get") {
			// TODO
		} else if (url.pathname == "/set") {
			// TODO
		}
		return serveDir(request, {
			fsRoot: resolve(dirname(fromFileUrl(import.meta.url)), ".."),
			showDirListing: true,
			showDotfiles: true,
			quiet: true,
		});
	},
});
server.listenAndServe();
console.log("Dev server running on http://localhost:8080");
