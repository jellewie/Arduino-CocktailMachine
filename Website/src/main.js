import { DrinkDisplay } from "./DrinkDisplay.js";
import { drinksConfig } from "./drinksConfig.js";
import { drinkSelectorEl } from "./globalElements.js";
// @ts-ignore
import globalStyleSheet from "./globalStyle.css" assert {type: "css"};

document.adoptedStyleSheets = [globalStyleSheet];

customElements.define("drink-display", DrinkDisplay);

for (const {name, cssColor, actions} of drinksConfig) {
	const drinkDisplayHtmlElement = document.createElement("drink-display");
	const drinkDisplay = /** @type {DrinkDisplay} */ (drinkDisplayHtmlElement);
	drinkDisplay.name = name;
	drinkDisplay.color = cssColor;
	const ingredients = [];
	for (const action of actions) {
		if ("ingredient" in action) {
			ingredients.push({
				name: action.ingredient,
				available: true,
			});
		}
	}
	drinkDisplay.setIngredients(ingredients)
	drinkSelectorEl.appendChild(drinkDisplayHtmlElement);
}
