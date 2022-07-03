/**
 * @param {Object} options
 * @param {string} options.name The name to display on the device.
 * @param {import("./drinksConfig.js").DrinkIngredientOrAction[]} options.actions
 */
export async function sendMixRequest({
	name,
	actions,
}) {
	const url = new URL("/set", window.location.href);
	url.searchParams.set("m", name);
	for (const [i, action] of actions.entries()) {
		const queryIndex = i;
		if ("ingredient" in action) {
			url.searchParams.set("i" + queryIndex, action.ingredient);
			url.searchParams.set("m" + queryIndex, String(action.amount));
		} else {
			url.searchParams.set("a" + queryIndex, "#" + action.action);
		}
	}
	const response = await fetch(url.href);
	if (!response.ok) {
		// TODO: Show error notification
	}
}
