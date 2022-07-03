import { actionNames, ingredientNames } from "../drinksConfig.js";
import { sendMixRequest } from "../sendMixRequest.js";
import { CustomizableIngredient } from "./CustomizableIngredient.js";

const drinkCustomizationDialog = /** @type {HTMLDialogElement} */ (document.getElementById("drinkCustomizationDialog"));
const drinkCustomizationTitle = /** @type {HTMLHeadingElement} */ (document.getElementById("drinkCustomizationTitle"));
const drinkCustomizationActionsList = /** @type {HTMLUListElement} */ (document.getElementById("drinkCustomizationActionsList"));
const addIngredientContainerEl = /** @type {HTMLDivElement} */ (document.getElementById("addIngredientContainer"));

/** @type {CustomizableIngredient[]} */
let currentCustomizableIngredients = [];

const addIngredientSelect = document.createElement("select");
addIngredientSelect.classList.add("add-ingredient");
addIngredientContainerEl.appendChild(addIngredientSelect);

const addIngredientTextOption = document.createElement("option");
addIngredientTextOption.textContent = "Add an ingredient";
addIngredientTextOption.value = "placeholder";
addIngredientTextOption.disabled = true;
addIngredientTextOption.selected = true;
addIngredientTextOption.style.display = "none";
addIngredientSelect.appendChild(addIngredientTextOption);

const actionsGroup = document.createElement("optgroup");
actionsGroup.label = "Actions";
for (const action of actionNames) {
	const option = document.createElement("option");
	option.value = action;
	option.text = action;
	actionsGroup.appendChild(option);
}
addIngredientSelect.appendChild(actionsGroup);

const ingredientsGroup = document.createElement("optgroup");
ingredientsGroup.label = "Ingredients";
for (const [id, name] of ingredientNames) {
	const option = document.createElement("option");
	option.value = id;
	option.text = name;
	ingredientsGroup.appendChild(option);
}
addIngredientSelect.appendChild(ingredientsGroup);

addIngredientSelect.addEventListener("change", () => {
	const castValueAction = /** @type {import("../drinksConfig.js").Actions} */ (addIngredientSelect.value);
	const castValueIngredient = /** @type {import("../drinksConfig.js").Ingredients} */ (addIngredientSelect.value);
	if (actionNames.includes(castValueAction)) {
		addIngredient({
			action: castValueAction,
		});
	} else if (ingredientNames.has(castValueIngredient)) {
		addIngredient({
			ingredient: castValueIngredient,
			amount: 0,
		});
	}
	addIngredientSelect.value = "placeholder";
});

/**
 * @param {import("../drinksConfig.js").DrinkIngredientOrAction} action
 */
function addIngredient(action) {
	const customizableIngredient = new CustomizableIngredient();
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
	drinkCustomizationActionsList.appendChild(customizableIngredient.el);
}

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
		addIngredient(action);
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
