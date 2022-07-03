import { ingredientNames } from "../drinksConfig.js";
import { sendMixRequest } from "../sendMixRequest.js";
import { CustomizableIngredient } from "./CustomizableIngredient.js";

const drinkCustomizationDialog = /** @type {HTMLDialogElement} */ (document.getElementById("drinkCustomizationDialog"));
const drinkCustomizationTitle = /** @type {HTMLHeadingElement} */ (document.getElementById("drinkCustomizationTitle"));
const drinkCustomizationActionsList = /** @type {HTMLUListElement} */ (document.getElementById("drinkCustomizationActionsList"));

/** @type {CustomizableIngredient[]} */
let currentCustomizableIngredients = [];

/**
 * @param {import("../drinksConfig.js").DrinkConfig} param0
 */
export function showModal({
	name,
	actions,
}) {
	drinkCustomizationTitle.textContent = name;

	while (drinkCustomizationActionsList.firstChild) {
		drinkCustomizationActionsList.removeChild(drinkCustomizationActionsList.firstChild);
	}
	currentCustomizableIngredients = [];

	for (const action of actions) {
		const customizableIngredient = new CustomizableIngredient();
		drinkCustomizationActionsList.appendChild(customizableIngredient.el);
		if ("action" in action) {
			customizableIngredient.isAction = true;
			customizableIngredient.text = action.action;
		} else {
			const text = ingredientNames.get(action.ingredient);
			customizableIngredient.text = text || action.ingredient;
			customizableIngredient.amount = action.amount;
		}
		customizableIngredient.onDeleteClick(() => {
			drinkCustomizationActionsList.removeChild(customizableIngredient.el);
			const index = currentCustomizableIngredients.indexOf(customizableIngredient);
			if (index >= 0) {
				currentCustomizableIngredients.splice(index, 1);
			}
		});
		currentCustomizableIngredients.push(customizableIngredient);
	}

	drinkCustomizationDialog.showModal();
}

function getCurrentActions() {
	/** @type {import("../drinksConfig.js").DrinkIngredientOrAction[]} */
	const actions = [];
	for (const customizableIngredient of currentCustomizableIngredients) {
		if (customizableIngredient.isAction) {
			actions.push({
				action: /** @type {import("../drinksConfig.js").Actions} */ (customizableIngredient.text),
			});
		} else {
			actions.push({
				ingredient: /** @type {import("../drinksConfig.js").Ingredients} */ (customizableIngredient.text),
				amount: customizableIngredient.amount,
			});
		}
	}
	return actions;
}

drinkCustomizationDialog.addEventListener("close", () => {
	if (drinkCustomizationDialog.returnValue == "mix") {
		const actions = getCurrentActions();

		sendMixRequest({
			name: drinkCustomizationTitle.textContent || "Drink",
			actions,
		})
	}
});
