/**
 * @typedef DispenserSettingItemOptionsDropdown
 * @property {"dropdown"} type
 * @property {Object.<string, string>} values A mapped object where the key is the
 * dropdown value and the value is the text to show in the dropdown ui.
 * @property {string} initialValue
 */

/**
 * @typedef DispenserSettingItemOptionsNumber
 * @property {"number"} type
 * @property {number} initialValue
 */

/**
 * @typedef DispenserSettingItemOptionsBase
 * @property {string} label
 * @property {() => void} onChange
 */

/**
 * @typedef {DispenserSettingItemOptionsBase & (DispenserSettingItemOptionsDropdown | DispenserSettingItemOptionsNumber)} DispenserSettingItemOptions
 */

export class DispenserSettingItem {
	/**
	 * @param {DispenserSettingItemOptions} options
	 */
	constructor(options) {
		this.el = document.createElement("label");
		this.el.classList.add("dispenser-setting");

		const labelTextEl = document.createElement("span");
		labelTextEl.classList.add("dispenser-setting-label-text");
		labelTextEl.textContent = options.label;
		this.el.appendChild(labelTextEl);

		if (options.type == "dropdown") {
			const select = document.createElement("select");
			for (const [value, text] of Object.entries(options.values)) {
				const option = document.createElement("option");
				option.value = value;
				option.textContent = text;
				select.appendChild(option);
			}
			select.value = options.initialValue;
			this.el.appendChild(select);
		} else if (options.type == "number") {
			const input = document.createElement("input");
			input.type = "number";
			input.value = String(options.initialValue);
			this.el.appendChild(input);
		}
	}

	getValue() {
		return "";
	}
}
