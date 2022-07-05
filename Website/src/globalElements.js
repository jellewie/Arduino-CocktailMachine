const search = document.getElementById("search");
const searchEl = /** @type {HTMLInputElement} */ (search);

const drinkSelector = document.getElementById("drinkSelector");
const drinkSelectorEl = /** @type {HTMLDivElement} */ (drinkSelector);

const customizeButton = document.getElementById("customizeButton");
const customizeButtonEl = /** @type {HTMLButtonElement} */ (customizeButton);

const mainMixButton = document.getElementById("mainMixButton");
const mainMixButtonEl = /** @type {HTMLButtonElement} */ (mainMixButton);

const settingsButton = /** @type {HTMLButtonElement} */ (document.getElementById("settingsButton"));

export {
	searchEl,
	drinkSelectorEl,
	customizeButtonEl,
	mainMixButtonEl,
	settingsButton,
}
