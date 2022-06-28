import { getAvailableIngredients } from "./configLoader.js";
import { DrinkDisplay } from "./DrinkDisplay.js";
import { drinksConfig } from "./drinksConfig.js";
import { drinkSelectorEl } from "./globalElements.js";
// @ts-ignore
import globalStyleSheet from "./globalStyle.css" assert {type: "css"};

document.adoptedStyleSheets = [globalStyleSheet];

customElements.define("drink-display", DrinkDisplay);

/**
 * @typedef CreatedDrinkData
 * @property {DrinkDisplay} el
 * @property {import("./drinksConfig.js").DrinkConfig} config
 */

/** @type {CreatedDrinkData[]} */
const createdDrinks = [];

for (const drinkConfig of drinksConfig) {
	const {name, cssColor} = drinkConfig;
	const drinkDisplayHtmlElement = document.createElement("drink-display");
	const drinkDisplay = /** @type {DrinkDisplay} */ (drinkDisplayHtmlElement);
	drinkDisplay.name = name;
	drinkDisplay.color = cssColor;
	drinkSelectorEl.appendChild(drinkDisplayHtmlElement);
	createdDrinks.push({
		el: drinkDisplay,
		config: drinkConfig,
	});
}

async function updateDrinkIngredients() {
	const availableIngredients = await getAvailableIngredients();
	for (const {el, config} of createdDrinks) {
		const ingredients = [];
		for (const action of config.actions) {
			if ("ingredient" in action) {
				ingredients.push({
					name: action.ingredient,
					available: availableIngredients.has(action.ingredient),
				});
			}
		}
		el.setIngredients(ingredients)
	}
}
updateDrinkIngredients();
