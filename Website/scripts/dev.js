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
// @ts-expect-error see https://github.com/jespertheend/deno-tsc-helper/issues/2
import {generateTypes} from "https://deno.land/x/deno_tsc_helper@v0.0.1/generateTypes.js";

await generateTypes({
	typeUrls: [
		"https://deno.land/x/deno_tsc_helper@v0.0.1/generateTypes.js",
		"https://deno.land/std@0.145.0/http/file_server.ts",
		"https://deno.land/std@0.145.0/http/server.ts",
		"https://deno.land/std@0.145.0/path/mod.ts",
		"https://esm.sh/rollup@2.75.7?pin=v86",
		"https://deno.land/x/chdir_anywhere@v0.0.2/mod.js",
	],
	outputDir: "../.denoTypes",
});

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

const mockGetData = {
	dispensers,
	ingredients,
	settings: {
		homed: false,
		disableSteppersAfterMixDone: true,
		bedSizeX: 26000,
		bedSizeY: 3000,
		bedSizeZ: 1000,
		manualX: 0,
		manualY: 0,
		motorMaxSpeed: 5500,
		motorMaxAccel: 3000,
		shotDispenserMl: 30,
		homeMaxSpeed: 200,
		homedistanceBounce: 400,
		naxGlassSize: 300
	}
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
