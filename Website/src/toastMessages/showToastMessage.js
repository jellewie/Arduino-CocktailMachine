/**
 * @param {string} message
 */
export async function showToastMessage(message) {
	const el = document.createElement("div");
	el.classList.add("toast-message", "hidden");
	el.textContent = message;
	document.body.appendChild(el);
	// trigger reflow
	el.offsetHeight;
	el.classList.remove("hidden");
	await new Promise(resolve => setTimeout(resolve, 5_000));
	el.classList.add("hidden");
	await new Promise(resolve => setTimeout(resolve, 500));
	el.remove();
}
