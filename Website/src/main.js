import "../lib/construct-style-sheets-polyfill.js";
import { getSelectedDrink, initDrinkSelector, markRecentDrink, scrollToRandomAvailableDrink, setDrinkFilter } from "./drinkSelector.js";
import { customizeButtonEl, mainMixButtonEl, openDispensersDialogButton, searchEl, randomDrinkButton, settingsButton } from "./globalElements.js";
import { sendMixRequest } from "./sendMixRequest.js";
import { showModal as showDrinkCustomizationModal } from "./drinkCustomizationDialog/dialog.js";
import { showModal as showSettingsModal } from "./settingsDialog/dialog.js";
import { showModal as showDispensersModal } from "./dispensersDialog/dialog.js";

// @ts-ignore
import globalStyleSheet from "./globalStyle.css" assert {type: "css"};
// @ts-ignore
import drinkDisplaySheet from "./DrinkDisplay.css" assert {type: "css"};
// @ts-ignore
import drinkCustomizationDialogSheet from "./drinkCustomizationDialog/style.css" assert {type: "css"};
// @ts-ignore
import settingsDialogSheet from "./settingsDialog/style.css" assert {type: "css"};
// @ts-ignore
import dispensersDialogSheet from "./dispensersDialog/style.css" assert {type: "css"};
// @ts-ignore
import toastMessagesSheet from "./toastMessages/style.css" assert {type: "css"};

globalThis["DEBUG_BUILD"] = true;

document.adoptedStyleSheets = [globalStyleSheet, drinkDisplaySheet, drinkCustomizationDialogSheet, settingsDialogSheet, dispensersDialogSheet, toastMessagesSheet];

mainMixButtonEl.addEventListener("click", () => {
	const selectedDrink = getSelectedDrink();
	const config = selectedDrink.config;
	markRecentDrink(config.name);
	sendMixRequest({
		name: config.name,
		actions: config.actions,
	})
});

customizeButtonEl.addEventListener("click", () => {
	const selectedDrink = getSelectedDrink();
	showDrinkCustomizationModal(selectedDrink.config);
});

settingsButton.addEventListener("click", () => {
	showSettingsModal();
});

openDispensersDialogButton.addEventListener("click", () => {
	showDispensersModal();
});

initDrinkSelector();

searchEl.addEventListener("input", () => {
	setDrinkFilter(searchEl.value);
});

randomDrinkButton.addEventListener("click", () => {
	scrollToRandomAvailableDrink();
});
