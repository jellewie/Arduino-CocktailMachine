import { DrinkDisplay } from "./DrinkDisplay.js";
import { getAvailableIngredients } from "./configLoader.js";
import { drinksConfig } from "./drinksConfig.js";
import { drinkSelectorEl } from "./globalElements.js";

/** @type {Map<DrinkDisplay, import("./drinksConfig.js").DrinkConfig>} */
const createdDrinks = new Map();

/** @type {Set<DrinkDisplay>} */
const visibleDrinks = new Set();

const intersectionObserver = new IntersectionObserver(entries => {
	for (const entry of entries) {
		entry.target.classList.toggle("selected", entry.isIntersecting);
		if (!(entry.target instanceof DrinkDisplay)) throw new Error("Assertion failed");
		if (entry.isIntersecting) {
			visibleDrinks.add(entry.target);
		} else {
			visibleDrinks.delete(entry.target);
		}
	}
}, {
	threshold: 1,
	root: drinkSelectorEl,
});

export function initDrinkSelector() {
	for (const drinkConfig of drinksConfig) {
		const {name, cssColor} = drinkConfig;
		const drinkDisplayHtmlElement = document.createElement("drink-display");
		const drinkDisplay = /** @type {DrinkDisplay} */ (drinkDisplayHtmlElement);
		drinkDisplay.name = name;
		drinkDisplay.color = cssColor;
		drinkSelectorEl.appendChild(drinkDisplayHtmlElement);
		createdDrinks.set(drinkDisplay, drinkConfig);
		intersectionObserver.observe(drinkDisplay);
	}

	updateDrinkIngredients();
}

async function updateDrinkIngredients() {
	const availableIngredients = await getAvailableIngredients();
	for (const [el, config] of createdDrinks) {
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

export function getSelectedDrink() {
	const selectorBounds = drinkSelectorEl.getBoundingClientRect();
	const selectorCenter = selectorBounds.left + selectorBounds.width / 2;

	let centerDrink = null;
	let closestCenterDist = Infinity;
	for (const drink of visibleDrinks) {
		const bounds = drink.getBoundingClientRect();
		const center = bounds.left + bounds.width / 2;
		const centerDist = Math.abs(center - selectorCenter);
		if (centerDist < closestCenterDist) {
			closestCenterDist = centerDist;
			centerDrink = drink;
		}
	}

	if (!centerDrink) throw new Error("No drink selected");
	const config = createdDrinks.get(centerDrink);
	if (!config) throw new Error("No config for selected drink");
	return {
		el: centerDrink,
		config,
	};
}
