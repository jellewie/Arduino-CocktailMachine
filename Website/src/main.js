import { getSelectedDrink, initDrinkSelector, markRecentDrink, setDrinkFilter } from "./drinkSelector.js";
import { customizeButtonEl, mainMixButtonEl, searchEl, settingsButton } from "./globalElements.js";
// @ts-ignore
import globalStyleSheet from "./globalStyle.css" assert {type: "css"};
// @ts-ignore
import drinkDisplaySheet from "./DrinkDisplay.css" assert {type: "css"};
// @ts-ignore
import drinkCustomizationDialogSheet from "./drinkCustomizationDialog/style.css" assert {type: "css"};
// @ts-ignore
import settingsDialogSheet from "./settingsDialog/style.css" assert {type: "css"};
import { sendMixRequest } from "./sendMixRequest.js";
import { showModal as showDrinkCustomizationModal } from "./drinkCustomizationDialog/dialog.js";
import { showModal as showSettingsModal } from "./settingsDialog/dialog.js";

globalThis["DEBUG_BUILD"] = true;

document.adoptedStyleSheets = [globalStyleSheet, drinkDisplaySheet, drinkCustomizationDialogSheet, settingsDialogSheet];

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

initDrinkSelector();

searchEl.addEventListener("input", () => {
	setDrinkFilter(searchEl.value);
});
