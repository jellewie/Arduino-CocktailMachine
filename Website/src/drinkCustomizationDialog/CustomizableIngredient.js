export class CustomizableIngredient {
	#grabHandleEl;
	#isAction;
	#amountEl;
	#amountPlaceholderEl;
	#textEl;
	/** @type {Set<() => void>} */
	#onDeleteClickCbs;
	#deleteButtonEl;

	constructor() {
		this.el = document.createElement("li");

		this.#grabHandleEl = document.createElement("div");
		this.#grabHandleEl.classList.add("grab-handle");
		this.el.appendChild(this.#grabHandleEl);

		this.#isAction = false;

		this.#amountEl = document.createElement("input");
		this.#amountEl.type = "number";
		this.#amountEl.min = "0";
		this.#amountEl.max = "1000";
		this.el.appendChild(this.#amountEl);

		this.#amountPlaceholderEl = document.createElement("span");
		this.el.appendChild(this.#amountPlaceholderEl);

		this.#textEl = document.createElement("span");
		this.el.appendChild(this.#textEl);

		this.#onDeleteClickCbs = new Set();

		this.#deleteButtonEl = document.createElement("button");
		this.#deleteButtonEl.textContent = "X";
		this.#deleteButtonEl.type = "button";
		this.#deleteButtonEl.addEventListener("click", () => {
			this.#onDeleteClickCbs.forEach(cb => cb());
		});
		this.el.appendChild(this.#deleteButtonEl);

		this.#updateIsAction();
	}

	get text() {
		return this.#textEl.textContent;
	}

	set text(value) {
		this.#textEl.textContent = value;
	}

	get isAction() {
		return this.#isAction;
	}

	set isAction(value) {
		this.#isAction = value;
		this.#updateIsAction();
	}

	#updateIsAction() {
		this.#amountEl.style.display = this.#isAction ? "none" : "";
		this.#amountPlaceholderEl.style.display = this.#isAction ? "" : "none";
	}

	get amount() {
		return Number(this.#amountEl.value);
	}

	set amount(value) {
		this.#amountEl.value = String(value);
	}

	/**
	 * @param {() => void} cb
	 */
	onDeleteClick(cb) {
		this.#onDeleteClickCbs.add(cb);
	}
}
