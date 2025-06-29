/**
 * @fileoverview This file contains management of getting data by fetching
 * from the /get endpoint and caches the state in memory.
 * This also contains some utility functions for using the data.
 */

import { showToastMessage } from "./toastMessages/showToastMessage.js";

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
 * @typedef {[x: number, y: number, timeMsMl: number, delayAir: number, ingredientId: number]} DispenserConfigArr
 */

/**
 * @typedef DispenserConfig
 * @property {number} x
 * @property {number} y
 * @property {number} timeMsMl
 * @property {number} delayAir
 * @property {import("./drinksConfig.js").Ingredients} ingredient
 */

/**
 * @typedef {Object.<string, number | boolean>} SettingsConfig
 */

/** @typedef {(config: ParsedConfigData) => void} OnConfigUpdatedCallback */

/** @type {ParsedConfigData?} */
let currentConfigData = null;

let isRefreshingConfigData = false;
/** @type {Promise<void>?} */
let lastRefreshingConfigPromise = null;

/** @type {Set<OnConfigUpdatedCallback>} */
const onConfigUpdatedCbs = new Set();

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
		showToastMessage("Failed to load config data.");
	} else {
		/** @type {ConfigJson} */
		const data = await response.json();
		/** @type {DispenserConfig[]} */
		const dispensers = [];
		for (const dispenser of data.dispensers) {
			dispensers.push({
				x: dispenser[0],
				y: dispenser[1],
				timeMsMl: dispenser[2],
				delayAir: dispenser[3],
				ingredient: data.ingredients[dispenser[4]]
			});
		}
		currentConfigData = {
			dispensers,
			ingredients: [...data.ingredients],
			settings: data.settings,
		};
		const config = currentConfigData;
		const autoRefreshSetting = config.settings.DispenserHeartbeatS;
		if (typeof autoRefreshSetting == "number") {
			updateAutoRefreshInterval(autoRefreshSetting * 1000);
		}
		onConfigUpdatedCbs.forEach(cb => cb(config));
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
 * @param {OnConfigUpdatedCallback} cb
 */
export function onConfigUpdated(cb) {
	onConfigUpdatedCbs.add(cb);
}

let autoRefreshInterval = -1;

/**
 * @param {number} interval
 */
function updateAutoRefreshInterval(interval) {
	if (autoRefreshInterval != -1) {
		clearInterval(autoRefreshInterval);
	}
	autoRefreshInterval = setInterval(() => {
		refreshConfig();
	}, interval);
}
