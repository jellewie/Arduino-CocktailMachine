/**
* @typedef DrinkDisplayIngredient
* @property {string} name The name of the ingredient to display.
* @property {boolean} available Whether the ingredient is available.
*/

export class DrinkDisplay {
	#mixerContainerEl;
	#mixerAccentsImageEl;
	#nameEl;
	#ingredientsListEl;
	#available = true;

	constructor() {
		this.el = document.createElement("div");
		this.el.classList.add("drink-display");

		this.#mixerContainerEl = document.createElement("div");
		this.#mixerContainerEl.classList.add("mixer-container");
		this.el.appendChild(this.#mixerContainerEl);

		const mixerOutlineEl = document.createElement("div");
		mixerOutlineEl.classList.add("mixer-outline");
		this.#mixerContainerEl.appendChild(mixerOutlineEl);

		this.#mixerAccentsImageEl = document.createElement("div");
		this.#mixerAccentsImageEl.classList.add("mixer-accents");
		this.#mixerContainerEl.appendChild(this.#mixerAccentsImageEl);

		this.#nameEl = document.createElement("h2");
		this.#nameEl.classList.add("name");
		this.el.appendChild(this.#nameEl);

		this.#ingredientsListEl = document.createElement("ul");
		this.#ingredientsListEl.classList.add("ingredients-list");
		this.el.appendChild(this.#ingredientsListEl);
	}

	get name() {
		return this.#nameEl.textContent || "";
	}

	set name(value) {
		this.#nameEl.textContent = value;
	}

	get color() {
		return this.#mixerAccentsImageEl.style.backgroundColor;
	}

	set color(value) {
		this.#mixerAccentsImageEl.style.backgroundColor = value;
	}

	get available() {
		return this.#available;
	}

	set available(value) {
		this.#available = value;
		this.#mixerContainerEl.classList.toggle("unavailable", !value);
	}

	/**
	 * @param {DrinkDisplayIngredient[]} value
	 */
	setIngredients(value) {
		while(this.#ingredientsListEl.firstChild) {
			this.#ingredientsListEl.removeChild(this.#ingredientsListEl.firstChild);
		}

		for (const ingredient of value) {
			const li = document.createElement("li");
			li.textContent = ingredient.name;
			this.#ingredientsListEl.appendChild(li);
			li.classList.toggle("available", ingredient.available);
		}
	}

	/**
	 * @param {boolean} visible
	 */
	setVisible(visible) {
		this.el.style.display = visible ? "" : "none";
	}
}
