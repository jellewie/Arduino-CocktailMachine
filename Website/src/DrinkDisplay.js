// @ts-ignore
import drinkDisplaySheet from "./DrinkDisplay.style.css" assert {type: "css"}

/**
 * @typedef DrinkDisplayIngredient
 * @property {string} name The name of the ingredient to display.
 * @property {boolean} available Whether the ingredient is available.
 */

export class DrinkDisplay extends HTMLElement {
	#mixerContainerEl;
	#mixerAccentsImageEl;
	#nameEl;
	#ingredientsListEl;

	constructor() {
		super();

		const shadow = this.attachShadow({mode: "open"});
		shadow.adoptedStyleSheets = [drinkDisplaySheet];

		this.#mixerContainerEl = document.createElement("div");
		this.#mixerContainerEl.classList.add("mixer-container");
		shadow.appendChild(this.#mixerContainerEl);

		const mixerOutlineEl = document.createElement("div");
		mixerOutlineEl.classList.add("mixer-outline");
		this.#mixerContainerEl.appendChild(mixerOutlineEl);

		this.#mixerAccentsImageEl = document.createElement("div");
		this.#mixerAccentsImageEl.classList.add("mixer-accents");
		this.#mixerContainerEl.appendChild(this.#mixerAccentsImageEl);

		this.#nameEl = document.createElement("h2");
		this.#nameEl.classList.add("name");
		shadow.appendChild(this.#nameEl);

		this.#ingredientsListEl = document.createElement("ul");
		this.#ingredientsListEl.classList.add("ingredients-list");
		shadow.appendChild(this.#ingredientsListEl);
	}

	get name() {
		return this.#nameEl.textContent;
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
}
