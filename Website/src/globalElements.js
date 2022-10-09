const search = document.getElementById("search");
const searchEl = /** @type {HTMLInputElement} */ (search);

const drinkSelector = document.getElementById("drinkSelector");
const drinkSelectorEl = /** @type {HTMLDivElement} */ (drinkSelector);

const customizeButton = document.getElementById("customizeButton");
const customizeButtonEl = /** @type {HTMLButtonElement} */ (customizeButton);

const mainMixButton = document.getElementById("mainMixButton");
const mainMixButtonEl = /** @type {HTMLButtonElement} */ (mainMixButton);

const settingsButton = /** @type {HTMLButtonElement} */ (document.getElementById("settingsButton"));
const settingsDialog = /** @type {HTMLDialogElement} */ (document.getElementById("settingsDialog"));
const randomDrinkButton = /** @type {HTMLButtonElement} */ (document.getElementById("randomDrinkButton"));
const openDispensersDialogButton = /** @type {HTMLButtonElement} */ (document.getElementById("openDispensersDialogButton"));
const dispensersDialog = /** @type {HTMLDialogElement} */ (document.getElementById("dispensersDialog"));

export {
	searchEl,
	drinkSelectorEl,
	customizeButtonEl,
	mainMixButtonEl,
	settingsButton,
	settingsDialog,
	randomDrinkButton,
	openDispensersDialogButton,
	dispensersDialog,
}
