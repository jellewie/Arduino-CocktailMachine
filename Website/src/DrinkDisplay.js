/**
* @typedef DrinkDisplayIngredient
* @property {string} name The name of the ingredient to display.
* @property {boolean} available Whether the ingredient is available.
*/

import { NeonSvgImage } from "./NeonSvgImage.js";
import { randInt } from "./util.js";

export class DrinkDisplay {
	#drinkIconEl;
	#drinkIcon;
	#nameEl;
	#ingredientsListEl;
	#available = true;

	constructor() {
		this.el = document.createElement("div");
		this.el.classList.add("drink-display");

		this.#drinkIconEl = document.createElement("div");
		this.#drinkIconEl.classList.add("drink-icon-container");
		this.el.appendChild(this.#drinkIconEl);

		this.#drinkIcon = new NeonSvgImage("drink" + randInt(1, 6));
		this.#drinkIconEl.appendChild(this.#drinkIcon.el);

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

	get available() {
		return this.#available;
	}

	set available(value) {
		this.#available = value;
		this.#drinkIconEl.classList.toggle("unavailable", !value);
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
