import { DrinkDisplay } from "./DrinkDisplay.js";
import { getAvailableIngredients, getConfig } from "./configLoader.js";
import { drinksConfig, ingredientNames } from "./drinksConfig.js";
import { drinkSelectorEl } from "./globalElements.js";
import { randFromArray } from "./util.js";

/**
 * @typedef DrinkData
 * @property {import("./drinksConfig.js").DrinkConfig} config
 * @property {boolean} allIngredientsAvailable
 * @property {DrinkDisplay} drinkDisplay
 * @property {boolean} isCustomDrink
 */

/** @type {Map<Element, DrinkData>} */
const createdDrinks = new Map();

/** @type {Set<Element>} */
const visibleDrinkEls = new Set();

/** @type {string[]} */
const recentDrinks = [];
if (localStorage.recentDrinks) {
	try {
		recentDrinks.push(...JSON.parse(localStorage.recentDrinks));
	} catch (e) {
		console.error(e);
	}
}

const intersectionObserver = new IntersectionObserver(entries => {
	for (const entry of entries) {
		entry.target.classList.toggle("selected", entry.isIntersecting);
		if (entry.isIntersecting) {
			visibleDrinkEls.add(entry.target);
		} else {
			visibleDrinkEls.delete(entry.target);
		}
	}
}, {
	threshold: 1,
	root: drinkSelectorEl,
});

/**
 * @param {import("./drinksConfig.js").DrinkConfig} drinkConfig
 */
function addDrink(drinkConfig, isCustomDrink = false) {
	const drinkDisplay = new DrinkDisplay({
		name: drinkConfig.name,
		isCustomDrink,
	});
	drinkSelectorEl.appendChild(drinkDisplay.el);
	createdDrinks.set(drinkDisplay.el, {
		config: drinkConfig,
		allIngredientsAvailable: false,
		drinkDisplay: drinkDisplay,
		isCustomDrink,
	});
	intersectionObserver.observe(drinkDisplay.el);
}

export async function initDrinkSelector() {
	// We'll wait until the config is available, otherwise the drink locations
	// jump once it loads.
	await getConfig();
	addDrink({
		name: "Custom",
		actions: [],
	}, true);
	for (const drinkConfig of drinksConfig) {
		addDrink(drinkConfig);
	}

	await updateDrinkIngredients();

	if (drinkSelectorEl.children.length > 1) {
		// The very first element is the custom drink
		const firstDrink = drinkSelectorEl.children[2];
		firstDrink.scrollIntoView();
	}
}

async function updateDrinkIngredients() {
	const availableIngredients = await getAvailableIngredients();
	for (const [el, drinkData] of createdDrinks) {
		const ingredients = [];
		let allIngredientsAvailable = true;
		for (const action of drinkData.config.actions) {
			if ("ingredient" in action) {
				const available = availableIngredients.has(action.ingredient);
				const name = ingredientNames.get(action.ingredient) || action.ingredient;
				ingredients.push({
					name,
					available,
				});
				if (!available) allIngredientsAvailable = false;
			}
		}
		drinkData.allIngredientsAvailable = allIngredientsAvailable;
		drinkData.drinkDisplay.available = allIngredientsAvailable;
		drinkData.drinkDisplay.setIngredients(ingredients)
	}

	sortDrinkElements();
}

function sortDrinkElements() {
	while (drinkSelectorEl.firstChild) {
		drinkSelectorEl.removeChild(drinkSelectorEl.firstChild);
	}
	const sortedDrinks = Array.from(createdDrinks.values());
	sortedDrinks.sort((a, b) => {
		if (a.isCustomDrink && !b.isCustomDrink) return -1;
		if (!a.isCustomDrink && b.isCustomDrink) return 1;
		if (a.allIngredientsAvailable && !b.allIngredientsAvailable) return -1;
		if (!a.allIngredientsAvailable && b.allIngredientsAvailable) return 1;
		const aIndex = recentDrinks.indexOf(a.config.name);
		const bIndex = recentDrinks.indexOf(b.config.name);
		if (aIndex >= 0 && bIndex >= 0) {
			return bIndex - aIndex;
		} else if (aIndex >= 0 && bIndex < 0) {
			return -1;
		} else if (aIndex < 0 && bIndex >= 0) {
			return 1;
		}
		return a.config.name.localeCompare(b.config.name);
	});

	for (const drinkData of sortedDrinks) {
		drinkSelectorEl.appendChild(drinkData.drinkDisplay.el);
	}
}

function getSelectorCenter() {
	const selectorBounds = drinkSelectorEl.getBoundingClientRect();
	return selectorBounds.left + selectorBounds.width / 2;
}

export function getSelectedDrink() {
	const selectorCenter = getSelectorCenter();

	let centerDrink = null;
	let closestCenterDist = Infinity;
	for (const drinkEl of visibleDrinkEls) {
		const bounds = drinkEl.getBoundingClientRect();
		const center = bounds.left + bounds.width / 2;
		const centerDist = Math.abs(center - selectorCenter);
		if (centerDist < closestCenterDist) {
			closestCenterDist = centerDist;
			centerDrink = drinkEl;
		}
	}

	if (!centerDrink) throw new Error("No drink selected");
	const data = createdDrinks.get(centerDrink);
	if (!data) throw new Error("No config for selected drink");
	return data;
}

/**
 * @param {string} filter
 */
export function setDrinkFilter(filter) {
	filter = filter.toLocaleLowerCase().trim();
	for (const drinkData of createdDrinks.values()) {
		let visible = true;
		if (filter) {
			visible = drinkData.drinkDisplay.name.toLocaleLowerCase().includes(filter);
		}
		drinkData.drinkDisplay.setVisible(visible);
	}
}

/**
 * Adds the drink to the end of the recent drink list and updates the order
 * of drinks.
 * @param {string} drinkName
 */
export function markRecentDrink(drinkName) {
	const existingIndex = recentDrinks.indexOf(drinkName);
	if (existingIndex !== -1) {
		recentDrinks.splice(existingIndex, 1);
	}
	recentDrinks.push(drinkName);
	localStorage.recentDrinks = JSON.stringify(recentDrinks);
	sortDrinkElements();
}

export function scrollToRandomAvailableDrink() {
	const availableDrinks = Array.from(createdDrinks.values()).filter(drink => drink.allIngredientsAvailable && !drink.isCustomDrink);
	const drink = randFromArray(availableDrinks);
	if (drink) {
		drink.drinkDisplay.el.scrollIntoView({
			inline: "center",
			behavior: "smooth",
		});
	}
}
