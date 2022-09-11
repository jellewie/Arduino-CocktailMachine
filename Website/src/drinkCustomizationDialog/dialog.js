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

const ingredientsGroup = document.createElement("optgroup");
ingredientsGroup.label = "Ingredients";
for (const [id, name] of ingredientNames) {
	const option = document.createElement("option");
	option.value = id;
	option.text = name;
	ingredientsGroup.appendChild(option);
}
addIngredientSelect.appendChild(ingredientsGroup);

const actionsGroup = document.createElement("optgroup");
actionsGroup.label = "Actions";
for (const action of actionNames) {
	const option = document.createElement("option");
	option.value = action;
	option.text = action;
	actionsGroup.appendChild(option);
}
addIngredientSelect.appendChild(actionsGroup);

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

/** @type {CustomizableIngredient?} */
let currentlyDraggingIngredient = null;
/** @type {Map<CustomizableIngredient, number>?} */
let computedIngredientPositionsBeforeDrag = null;
const ingredientHeight = 30;

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

	customizableIngredient.onDragStart(() => {
		if (currentlyDraggingIngredient) return false;
		computeIngredientBounds();
		currentlyDraggingIngredient = customizableIngredient;
		drinkCustomizationActionsList.classList.add("rearranging");
		return true;
	});
	customizableIngredient.onDragMove(() => {
		if (currentlyDraggingIngredient != customizableIngredient) return;
		updateDraggingIngredientTransforms();
	});
	customizableIngredient.onDragEnd(() => {
		if (currentlyDraggingIngredient != customizableIngredient) return;
		const {sortedIngredientsAfterDrag} = getSortedIngredients();

		while (drinkCustomizationActionsList.firstChild) {
			drinkCustomizationActionsList.removeChild(drinkCustomizationActionsList.firstChild);
		}
		for (const ingredient of sortedIngredientsAfterDrag) {
			ingredient.el.style.transform = "";
			drinkCustomizationActionsList.appendChild(ingredient.el);
		}
		currentCustomizableIngredients = [...sortedIngredientsAfterDrag];

		drinkCustomizationActionsList.classList.remove("rearranging");
		currentlyDraggingIngredient = null;
	});

	drinkCustomizationActionsList.appendChild(customizableIngredient.el);
}

function computeIngredientBounds() {
	if (currentlyDraggingIngredient) {
		throw new Error("Assertion failed, an element is already being dragged");
	}
	computedIngredientPositionsBeforeDrag = new Map();
	for (const ingredient of currentCustomizableIngredients) {
		const pos = computeIngredientYPos(ingredient);
		computedIngredientPositionsBeforeDrag.set(ingredient, pos);
	}
}

/**
 * @param {CustomizableIngredient} ingredient
 */
function computeIngredientYPos(ingredient) {
	const bounds = ingredient.el.getBoundingClientRect();
	return bounds.top + bounds.bottom / 2;
}

/**
 * @param {Map<CustomizableIngredient, number>} ingredientPositions
 */
function sortIngredientsByPosition(ingredientPositions) {
	return Array.from(ingredientPositions).sort((a, b) => {
		return a[1] - b[1];
	}).map(entry => entry[0]);
}

/**
 * Gets all ingredients and sorts them by position from before and after dragging.
 */
function getSortedIngredients() {
	if (!currentlyDraggingIngredient) {
		throw new Error("Assertion failed, no element is being dragged");
	}
	if (!computedIngredientPositionsBeforeDrag) {
		throw new Error("Assertion failed, bounds have not been computed");
	}

	const sortedIngredientsBeforeDrag = sortIngredientsByPosition(computedIngredientPositionsBeforeDrag);

	const positionsAfterDrag = new Map(computedIngredientPositionsBeforeDrag.entries());
	let draggingPosition = computeIngredientYPos(currentlyDraggingIngredient);
	// To allow elements to move a little sooner, we shift the dragging position
	// down by half the ingredientHeight. Otherwise you'd have to drag the element
	// all the way past the element below it.
	draggingPosition += ingredientHeight / 2;
	positionsAfterDrag.set(currentlyDraggingIngredient, draggingPosition);

	const sortedIngredientsAfterDrag = sortIngredientsByPosition(positionsAfterDrag);
	return {
		sortedIngredientsBeforeDrag,
		sortedIngredientsAfterDrag,
	}
}

function updateDraggingIngredientTransforms() {
	const {sortedIngredientsBeforeDrag, sortedIngredientsAfterDrag} = getSortedIngredients();

	for (const ingredient of currentCustomizableIngredients) {
		if (ingredient == currentlyDraggingIngredient) continue;

		const indexBeforeDrag = sortedIngredientsBeforeDrag.indexOf(ingredient);
		const indexAfterDrag = sortedIngredientsAfterDrag.indexOf(ingredient);
		const delta = indexAfterDrag - indexBeforeDrag;

		ingredient.el.style.transform = `translateY(${delta * ingredientHeight}px)`;
	}
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

	for (const ingredient of currentCustomizableIngredients) {
		ingredient.disabled = true;
	}
	addIngredientSelect.disabled = true;
	drinkCustomizationDialog.showModal();
	for (const ingredient of currentCustomizableIngredients) {
		ingredient.disabled = false;
	}
	addIngredientSelect.disabled = false;
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
