import { getConfig, onConfigLoaded, refreshConfig } from "../configLoader.js";
import { settingsDialog } from "../globalElements.js";
import { showToastMessage } from "../toastMessages/showToastMessage.js";

const settingsListEl = /** @type {HTMLUListElement} */ (document.getElementById("settingsList"));

/** @type {Map<string, HTMLInputElement>} */
const createdSettings = new Map();

export function showModal() {
	settingsDialog.showModal();
	initSettingsList();
	refreshConfig();
}

let initSettingsListCalled = false;
let settingsInitialized = false;
async function initSettingsList() {
	if (initSettingsListCalled) return;
	initSettingsListCalled = true;
	const config = await getConfig();
	for (const [key, value] of Object.entries(config.settings)) {
		const li = document.createElement("li");
		li.classList.add("list-setting-item");
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
			sendChangeSetting(key.replaceAll(" ", ""), value);
		});
		createdSettings.set(key, input);
	}
	settingsInitialized = true;
}

onConfigLoaded(config => {
	if (!settingsInitialized) return;
	for (const [key, value] of Object.entries(config.settings)) {
		const inputEl = createdSettings.get(key);
		if (inputEl) {
			if (typeof value === "boolean") {
				inputEl.checked = value;
			} else {
				inputEl.value = String(value);
			}
		}
	}
});

/**
 * @param {string} key
 * @param {string} value
 */
async function sendChangeSetting(key, value) {
	const url = new URL("/set", window.location.href);
	url.searchParams.set(key, value);
	const response = await fetch(url);
	const message = response.ok ? "Setting updated." : "Failed to update setting.";
	showToastMessage(message, {
		parent: settingsDialog,
		location: "top",
	});
}
