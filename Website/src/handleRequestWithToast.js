import { showToastMessage } from "./toastMessages/showToastMessage.js";

/**
 * @param {URL} url
 * @param {Object} options
 * @param {string} options.successMessage
 * @param {string} options.fallbackErrorMessage
 * @param {import("./toastMessages/showToastMessage.js").ShowToastMessageOptions} [options.toastOptions]
 */
export async function handleRequestWithToast(url, {
	successMessage,
	fallbackErrorMessage,
	toastOptions,
}) {
	const response = await fetch(url.href);
	if (response.ok) {
		showToastMessage(successMessage, toastOptions);
	} else {
		const text = await response.text();
		showToastMessage(text || fallbackErrorMessage, toastOptions);
	}
}
