/**
 * @fileoverview
 * Starts a local server for development.
 * This mostly serves files in the Website directory but also serves some extra
 * requests that the ESP would normally handle.
 * Run with `deno task dev`.
 */

import {serveDir} from "https://deno.land/std@0.145.0/http/file_server.ts";
import {Server} from "https://deno.land/std@0.145.0/http/server.ts";
import {resolve, fromFileUrl, dirname} from "https://deno.land/std@0.145.0/path/mod.ts";
import {generateTypes} from "https://deno.land/x/deno_tsc_helper@v0.1.2/mod.js";
import {setCwd} from "https://deno.land/x/chdir_anywhere@v0.0.2/mod.js";
setCwd();

generateTypes({
	outputDir: "../.denoTypes",
	excludeUrls: [
		"https://esm.sh/rollup@2.75.7?pin=v87",
		"https://esm.sh/clean-css@5.3.0?pin=v87",
	],
	logLevel: "WARNING",
});

const libs = {
	"construct-style-sheets-polyfill.js": "https://unpkg.com/construct-style-sheets-polyfill@3.1.0/dist/adoptedStyleSheets.js",
};

await Deno.mkdir("../lib", {recursive: true});
/** @type {Promise<void>[]} */
const libFetchPromises = [];
for (const [file, url] of Object.entries(libs)) {
	const promise = (async () => {
		const path = resolve("../lib", file);
		let exists = true;
		try {
			await Deno.stat(path);
		} catch (e) {
			exists = false;
		}
		if (!exists) {
			const response = await fetch(url);
			if (!response.ok) {
				throw new Error(`Failed to fetch ${url}`);
			}
			await Deno.writeTextFile(path, await response.text());
		}
	})();
	libFetchPromises.push(promise);
}
await Promise.all(libFetchPromises);

const dispensers = [];
for (let i = 0; i < 11; i++) {
	const type = Math.floor(Math.random() * 3);
	const x = Math.floor(Math.random() * 3000);
	const y = Math.floor(Math.random() * 3000);
	const z = Math.floor(Math.random() * 3000);
	const msMl = Math.floor(Math.random() * 500);
	const msOff = Math.floor(Math.random() * 500);
	const ingredientId = Math.floor(Math.random() * 18);
	dispensers.push([type, x, y, z, msMl, msOff, ingredientId]);
}

// TODO: use the ingredients list from drinksConfig.js somehow
const ingredients = ["VODKA", "GIN", "ORANGE_JUICE",
"UNK","VODKA","GIN","ORANGE_JUICE","RUM","CRANBERRY_JUICE","LEMONADE","TRIPLE_SEC","LEMON_JUICE","COLA","SPRITE","TEQUILA","GRENADINE_SYRUP","MILK","PINEAPPLE_JUICE","LIME_JUICE","PEACH_TREE","WHITE_RUM","TONIC_WATER","WATERMELON_VODKA","GINGER_ALE","SIMPLE_SYRUP","WHISKY","LEMON_SODA"];

/** @type {Object.<string, number | boolean>} */
const currentSettings = {
	booleanA: true,
	booleanB: false,
	numberA: 0,
	numberB: 1,
	numberC: 200,
	numberD: 3000,
	"Boolean with spaces": true,
	"Number with spaces": 1234,
}

const mockGetData = {
	dispensers,
	ingredients,
	settings: currentSettings,
};

const server = new Server({
	port: 8080,
	handler: request => {
		const url = new URL(request.url);
		if (url.pathname == "/get") {
			// @ts-expect-error Response.json is implemented in Deno but typescript
			// doesn't have types for it yet.
			return Response.json(mockGetData);
		} else if (url.pathname == "/set") {
			const searchParams = Array.from(url.searchParams.entries()).map(([k,v]) => `${k}: ${v}`).join("\n");
			console.log(`/set request received!\n${searchParams}`);
			for (const [searchKey, searchValue] of url.searchParams) {
				for (const [settingKey, settingValue] of Object.entries(currentSettings)) {
					if (searchKey == settingKey.replaceAll(" ", "")) {
						if (typeof settingValue == "boolean") {
							currentSettings[settingKey] = searchValue == "true";
						} else {
							currentSettings[settingKey] = parseInt(searchValue);
						}
					}
				}
			}
			return new Response();
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
