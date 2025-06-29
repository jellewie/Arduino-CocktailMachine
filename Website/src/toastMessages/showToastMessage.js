/**
 * @typedef ShowToastMessageOptions
 * @property {HTMLElement} [parent]
 * @property {"top" | "bottom"} [location]
 */

/**
 * @param {string} message
 * @param {ShowToastMessageOptions} options
 */
export async function showToastMessage(message, {
	parent = document.body,
	location = "bottom",
} = {}) {
	const el = document.createElement("div");
	el.classList.add("toast-message", "hidden", location);
	el.textContent = message;
	parent.appendChild(el);
	// trigger reflow
	el.offsetHeight;
	el.classList.remove("hidden");
	await new Promise(resolve => setTimeout(resolve, 5_000));
	el.classList.add("hidden");
	await new Promise(resolve => setTimeout(resolve, 500));
	el.remove();
}
