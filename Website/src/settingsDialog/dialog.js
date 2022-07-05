import { getConfig } from "../configLoader.js";

const drinkCustomizationDialog = /** @type {HTMLDialogElement} */ (document.getElementById("settingsDialog"));
const settingsListEl = /** @type {HTMLUListElement} */ (document.getElementById("settingsList"));

export function showModal() {
	drinkCustomizationDialog.showModal();
	initSettingsList();
}

let initSettingsListCalled = false;
async function initSettingsList() {
	if (initSettingsListCalled) return;
	initSettingsListCalled = true;
	const config = await getConfig();
	for (const [key, value] of Object.entries(config.settings)) {
		const li = document.createElement("li");
		settingsListEl.appendChild(li);

		const label = document.createElement("label");
		li.appendChild(label);

		const textEl = document.createElement("span");
		textEl.textContent = key;
		label.appendChild(textEl);

		const input = document.createElement("input");
		label.appendChild(input);
		const isBoolean = typeof value === "boolean";
		if (isBoolean) {
			input.type = "checkbox";
			input.checked = value;
		} else {
			input.type = "number";
			input.value = String(value);
		}
		input.addEventListener("change", async () => {
			let value;
			if (isBoolean) {
				value = String(input.checked);
			} else {
				value = input.value;
			}
			sendChangeSetting(key, value);
		});
	}
}

/**
 * @param {string} key
 * @param {string} value
 */
async function sendChangeSetting(key, value) {
	const url = new URL("/set", window.location.href);
	url.searchParams.set(key, value);
	const response = await fetch(url);
	if (!response.ok) {
		// TODO: Show error notification
	}
}
