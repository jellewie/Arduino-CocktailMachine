import { getSelectedDrink, initDrinkSelector, markRecentDrink, setDrinkFilter } from "./drinkSelector.js";
import { customizeButtonEl, mainMixButtonEl, searchEl } from "./globalElements.js";
// @ts-ignore
import globalStyleSheet from "./globalStyle.css" assert {type: "css"};
// @ts-ignore
import drinkDisplaySheet from "./DrinkDisplay.css" assert {type: "css"};
// @ts-ignore
import drinkCustomizationDialogSheet from "./drinkCustomizationDialog/style.css" assert {type: "css"};
import { sendMixRequest } from "./sendMixRequest.js";
import { showModal } from "./drinkCustomizationDialog/dialog.js";

globalThis["DEBUG_BUILD"] = true;

document.adoptedStyleSheets = [globalStyleSheet, drinkDisplaySheet, drinkCustomizationDialogSheet];

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
	showModal(selectedDrink.config);
});

initDrinkSelector();

searchEl.addEventListener("input", () => {
	setDrinkFilter(searchEl.value);
});
