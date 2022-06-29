import { DrinkDisplay } from "./DrinkDisplay.js";
import { getSelectedDrink, initDrinkSelector } from "./drinkSelector.js";
import { mixButtonEl } from "./globalElements.js";
// @ts-ignore
import globalStyleSheet from "./globalStyle.css" assert {type: "css"};
import { sendMixRequest } from "./sendMixRequest.js";

document.adoptedStyleSheets = [globalStyleSheet];

customElements.define("drink-display", DrinkDisplay);

mixButtonEl.addEventListener("click", () => {
	const selectedDrink = getSelectedDrink();
	const config = selectedDrink.config;
	sendMixRequest({
		name: config.name,
		actions: config.actions,
	})
});

initDrinkSelector();
