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
	url.searchParams.set("n", name);
	for (const [i, action] of actions.entries()) {
		const queryIndex = i + 1;
		if ("ingredient" in action) {
			url.searchParams.set("ai" + queryIndex, action.ingredient);
			url.searchParams.set("am" + queryIndex, String(action.amount));
		} else {
			url.searchParams.set("aa" + queryIndex, "#" + action.action);
		}
	}
	const response = await fetch(url.href);
	if (!response.ok) {
		// TODO: Show error notification
	}
}
