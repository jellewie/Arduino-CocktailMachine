/**
 * @fileoverview This file contains management of getting data by fetching
 * from the /get endpoint and caches the state in memory.
 * This also contains some utility functions for using the data.
 */

/**
 * @typedef ConfigJson
 * @property {DispenserConfigArr[]} dispensers
 * @property {import("./drinksConfig.js").Ingredients[]} ingredients
 * @property {SettingsConfig} settings
 */

/**
 * @typedef ParsedConfigData
 * @property {import("./drinksConfig.js").Ingredients[]} ingredients
 * @property {DispenserConfig[]} dispensers
 * @property {SettingsConfig} settings
 */

/**
 * @typedef {[typeId: number, x: number, y: number, z: number, msMl: number, msOff: number, ingredientId: number]} DispenserConfigArr
 */

/**
 * @typedef DispenserConfig
 * @property {number} typeId
 * @property {number} x
 * @property {number} y
 * @property {number} z
 * @property {number} msMl
 * @property {number} msOff
 * @property {import("./drinksConfig.js").Ingredients} ingredient
 */

/**
 * @typedef {Object.<string, number | boolean>} SettingsConfig
 */

/** @typedef {(config: ParsedConfigData) => void} OnConfigLoadedCallback */

/** @type {ParsedConfigData?} */
let currentConfigData = null;

let isRefreshingConfigData = false;
/** @type {Promise<void>?} */
let lastRefreshingConfigPromise = null;

/** @type {Set<OnConfigLoadedCallback>} */
const onConfigLoadedCbs = new Set();

/**
 * Returns the cached config if it exists, otherwise it loads the config first.
 */
export async function getConfig() {
	if (!currentConfigData) {
		await refreshConfig();
	}
	if (!currentConfigData) {
		throw new Error("Failed to load config data.");
	}
	return currentConfigData;
}

export async function refreshConfig() {
	if (!isRefreshingConfigData || !lastRefreshingConfigPromise) {
		lastRefreshingConfigPromise = refreshConfigFn();
	}
	return await lastRefreshingConfigPromise;
}

async function refreshConfigFn() {
	const response = await fetch("/get");
	if (!response.ok) {
		// TODO: show notification
	} else {
		/** @type {ConfigJson} */
		const data = await response.json();
		/** @type {DispenserConfig[]} */
		const dispensers = [];
		for (const dispenser of data.dispensers) {
			dispensers.push({
				typeId: dispenser[0],
				x: dispenser[1],
				y: dispenser[2],
				z: dispenser[3],
				msMl: dispenser[4],
				msOff: dispenser[5],
				ingredient: data.ingredients[dispenser[6]]
			});
		}
		currentConfigData = {
			dispensers,
			ingredients: [...data.ingredients],
			settings: data.settings,
		};
		const config = currentConfigData;
		onConfigLoadedCbs.forEach(cb => cb(config));
	}
}

export async function getAvailableIngredients() {
	const config = await getConfig();
	/** @type {Set<import("./drinksConfig.js").Ingredients>} */
	let availableIngredients = new Set();
	for (const dispenser of config.dispensers) {
		availableIngredients.add(dispenser.ingredient);
	}
	return availableIngredients;
}

/**
 * @param {OnConfigLoadedCallback} cb
 */
export function onConfigLoaded(cb) {
	onConfigLoadedCbs.add(cb);
}
