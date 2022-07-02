import { DrinkDisplay } from "./DrinkDisplay.js";
import { getSelectedDrink, initDrinkSelector, markRecentDrink, setDrinkFilter } from "./drinkSelector.js";
import { mixButtonEl, searchEl } from "./globalElements.js";
// @ts-ignore
import globalStyleSheet from "./globalStyle.css" assert {type: "css"};
// @ts-ignore
import drinkDisplaySheet from "./DrinkDisplay.css" assert {type: "css"};
import { sendMixRequest } from "./sendMixRequest.js";

document.adoptedStyleSheets = [globalStyleSheet, drinkDisplaySheet];

mixButtonEl.addEventListener("click", () => {
	const selectedDrink = getSelectedDrink();
	const config = selectedDrink.config;
	markRecentDrink(config.name);
	sendMixRequest({
		name: config.name,
		actions: config.actions,
	})
});

initDrinkSelector();

searchEl.addEventListener("input", () => {
	setDrinkFilter(searchEl.value);
});
