import { ingredientNames } from "../drinksConfig.js";
import { showToastMessage } from "../toastMessages/showToastMessage.js";
import { DispenserSettingItem } from "./DispenserSettingItem.js";

/** @typedef {(config: import("../configLoader.js").DispenserConfig) => void} OnDispenserChangeCallback */

/**
 * @param {string} label
 */
function createSettingHelper(label) {
	const labelEl = document.createElement("label");
	labelEl.classList.add("dispenser-setting");

	const labelTextEl = document.createElement("span");
	labelTextEl.classList.add("dispenser-setting-label-text");
	labelTextEl.textContent = label;
	labelEl.appendChild(labelTextEl);

	return labelEl;
}

/**
 * @template {string} TValue
 * @param {Object} options
 * @param {string} options.label
 * @param {Object.<TValue, string>} options.availableValues
 * @param {TValue} options.initialValue
 * @param {() => void} options.onChange
 */
function createSelectSetting({
	label,
	availableValues,
	initialValue,
	onChange,
}) {
	const labelEl = createSettingHelper(label);

	const select = document.createElement("select");
	for (const [value, text] of Object.entries(availableValues)) {
		const option = document.createElement("option");
		option.value = value;
		option.textContent = text;
		select.appendChild(option);
	}
	select.value = initialValue;
	select.addEventListener("change", () => {
		onChange();
	});
	labelEl.appendChild(select);

	return {
		el: labelEl,
		getValue: () => {
			return /** @type {TValue} */ (select.value);
		},
	}
}

/**
 * @param {Object} options
 * @param {string} options.label
 * @param {number} options.initialValue
 * @param {() => void} options.onChange
 */
function createNumberSetting({
	label,
	initialValue,
	onChange,
}) {
	const labelEl = createSettingHelper(label);

	const input = document.createElement("input");
	input.type = "number";
	input.value = String(initialValue);
	input.addEventListener("change", () => {
		onChange();
	});
	labelEl.appendChild(input);

	return {
		el: labelEl,
		getValue: () => {
			return Number(input.value);
		},
	}
}

export class DispenserSettingsItem {
	/**
	 * @param {number} index
	 * @param {import("../configLoader.js").DispenserConfig} dispenserConfig
	 * @param {import("../drinksConfig.js").Ingredients[]} availableIngredients
	 */
	constructor(index, dispenserConfig, availableIngredients) {
		this.el = document.createElement("li");
		this.el.classList.add("list-setting-item");

		const numberEl = document.createElement("span");
		numberEl.textContent = String(index);
		numberEl.classList.add("dispenser-setting-number");
		this.el.appendChild(numberEl);

		/** @private @type {Set<OnDispenserChangeCallback>} */
		this.onDispenserChangeCbs = new Set();

		const boundFireDispenserChangeCbs = this.fireDispenserChangeCbs.bind(this);

		/** @type {Object.<import("../drinksConfig.js").Ingredients, string>} */
		const ingredientValues = {};
		for (const ingredient of availableIngredients) {
			ingredientValues[ingredient] = ingredientNames.get(ingredient) || ingredient;
		}
		const ingredientSetting = createSelectSetting({
			label: "Ingredient",
			availableValues: ingredientValues,
			initialValue: dispenserConfig.ingredient,
			onChange: boundFireDispenserChangeCbs,
		});
		this.el.appendChild(ingredientSetting.el);
		/** @private */
		this.getIngredientSetting = ingredientSetting.getValue;

		const typeSetting = createSelectSetting({
			label: "Type",
			availableValues: {
				"0": "Unspecified",
				"1": "Shot",
				"2": "Pump",
			},
			initialValue: String(dispenserConfig.typeId),
			onChange: boundFireDispenserChangeCbs,
		});
		this.el.appendChild(typeSetting.el);
		/** @private */
		this.getTypeSetting = typeSetting.getValue;

		const xSetting = createNumberSetting({
			label: "x",
			initialValue: dispenserConfig.x,
			onChange: boundFireDispenserChangeCbs,
		});
		this.el.appendChild(xSetting.el);
		/** @private */
		this.getXSetting = xSetting.getValue;

		const ySetting = createNumberSetting({
			label: "y",
			initialValue: dispenserConfig.y,
			onChange: boundFireDispenserChangeCbs,
		});
		this.el.appendChild(ySetting.el);
		/** @private */
		this.getYSetting = ySetting.getValue;

		const zSetting = createNumberSetting({
			label: "z",
			initialValue: dispenserConfig.z,
			onChange: boundFireDispenserChangeCbs,
		});
		this.el.appendChild(zSetting.el);
		/** @private */
		this.getZSetting = zSetting.getValue;

		const msMlSetting = createNumberSetting({
			label: "ms/ml",
			initialValue: dispenserConfig.msMl,
			onChange: boundFireDispenserChangeCbs,
		});
		this.el.appendChild(msMlSetting.el);
		/** @private */
		this.getMsMlSetting = msMlSetting.getValue;

		const msOffSetting = createNumberSetting({
			label: "ms off",
			initialValue: dispenserConfig.msOff,
			onChange: boundFireDispenserChangeCbs,
		});
		this.el.appendChild(msOffSetting.el);
		/** @private */
		this.getMsOffSetting = msOffSetting.getValue;

		const goToButton = document.createElement("button");
		goToButton.textContent = "Go to";
		goToButton.classList.add("text-button");
		goToButton.type = "button";
		goToButton.addEventListener("click", async () => {
			const url = new URL("/set", window.location.href);
			url.searchParams.set("x", String(this.getXSetting()));
			url.searchParams.set("y", String(this.getYSetting()));
			const response = await fetch(url);
			if (response.ok) {
				showToastMessage("Moving to dispenser...");
			} else {
				showToastMessage("Failed to go to dispenser.");
			}
		});
		this.el.appendChild(goToButton);
	}

	/**
	 * @param {OnDispenserChangeCallback} cb
	 */
	onDispenserChange(cb) {
		this.onDispenserChangeCbs.add(cb);
	}

	/**
	 * @private
	 */
	fireDispenserChangeCbs() {
		/** @type {import("../configLoader.js").DispenserConfig} */
		const config = {
			ingredient: this.getIngredientSetting(),
			typeId: parseInt(this.getTypeSetting()),
			x: this.getXSetting(),
			y: this.getYSetting(),
			z: this.getZSetting(),
			msMl: this.getMsMlSetting(),
			msOff: this.getMsOffSetting(),
		}

		this.onDispenserChangeCbs.forEach(cb => cb(config));
	}
}
