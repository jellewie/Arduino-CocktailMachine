import { getConfig } from "../configLoader.js";
import { dispensersDialog } from "../globalElements.js";
import { handleRequestWithToast } from "../handleRequestWithToast.js";
import { DispenserSettingsItem } from "./DispenserSettingsItem.js";

const dispensersList = /** @type {HTMLUListElement} */ (document.getElementById("dispensersList"));

export function showModal() {
	dispensersDialog.showModal();
	updateDispensersList();
}

async function updateDispensersList() {
	const config = await getConfig();

	while (dispensersList.firstChild) {
		dispensersList.removeChild(dispensersList.firstChild);
	}

	for (const [i, dispenser] of config.dispensers.entries()) {
		const dispenserId = i + 1;
		const settingsItem = new DispenserSettingsItem(dispenserId, dispenser, config.ingredients);
		dispensersList.appendChild(settingsItem.el);
		settingsItem.onDispenserChange(async (dispenserConfig, changedSettingType) => {
			const url = new URL("/set", window.location.href);
			url.searchParams.set("di", String(dispenserId));
			if (changedSettingType == "x") {
				url.searchParams.set("dx", String(dispenserConfig.x));
			} else if (changedSettingType == "y") {
				url.searchParams.set("dy", String(dispenserConfig.y));
			} else if (changedSettingType == "timeMsMl") {
				url.searchParams.set("dl", String(dispenserConfig.timeMsMl));
			} else if (changedSettingType == "delayAir") {
				url.searchParams.set("do", String(dispenserConfig.delayAir));
			} else  if (changedSettingType == "ingredient") {
				let ingredientId = config.ingredients.indexOf(dispenserConfig.ingredient);
				if (ingredientId < 0) ingredientId = 0;
				url.searchParams.set("dn", String(ingredientId));
			} else if (changedSettingType == "fluidLevel") {
				url.searchParams.set("df", String(dispenserConfig.fluidLevel));
			}
			await handleRequestWithToast(url, {
				successMessage: "Dispenser updated.",
				fallbackErrorMessage: "Failed to update dispenser.",
				toastOptions: {
					parent: dispensersDialog,
					location: "top",
				},
			});
		});
	}
}
