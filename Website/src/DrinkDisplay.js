// @ts-ignore
import drinkDisplaySheet from "./DrinkDisplay.style.css" assert {type: "css"}

/**
 * @typedef DrinkDisplayIngredient
 * @property {string} name The name of the ingredient to display.
 * @property {boolean} available Whether the ingredient is available.
 */

export class DrinkDisplay extends HTMLElement {
	#imageEl;
	#nameEl;
	#ingredientsListEl;

	constructor() {
		super();

		const shadow = this.attachShadow({mode: "open"});
		shadow.adoptedStyleSheets = [drinkDisplaySheet];

		this.#imageEl = document.createElement("div");
		this.#imageEl.classList.add("image");
		shadow.appendChild(this.#imageEl);

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
		return this.#imageEl.style.backgroundColor;
	}

	set color(value) {
		this.#imageEl.style.backgroundColor = value;
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
		}
	}
}
