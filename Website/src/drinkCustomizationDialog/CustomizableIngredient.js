export class CustomizableIngredient {
	#grabHandleEl;
	#isAction;
	#amountEl;
	#amountPlaceholderEl;
	#textEl;
	/** @type {Set<() => void>} */
	#onDeleteClickCbs;
	#deleteButtonEl;

	#isDragging = false;
	#dragStart = 0;
	/** @type {Set<() => boolean>} */
	#onDragStartCbs = new Set();
	/** @type {Set<() => void>} */
	#onDragMoveCbs = new Set();
	/** @type {Set<() => void>} */
	#onDragEndCbs = new Set();

	constructor() {
		this.el = document.createElement("li");

		this.#grabHandleEl = document.createElement("div");
		this.#grabHandleEl.classList.add("grab-handle");
		this.el.appendChild(this.#grabHandleEl);

		this.#isAction = false;

		this.#amountEl = document.createElement("input");
		this.#amountEl.classList.add("amount-space");
		this.#amountEl.type = "number";
		this.#amountEl.min = "0";
		this.#amountEl.max = "1000";
		this.el.appendChild(this.#amountEl);

		this.#amountPlaceholderEl = document.createElement("span");
		this.#amountPlaceholderEl.classList.add("amount-space");
		this.el.appendChild(this.#amountPlaceholderEl);

		this.#textEl = document.createElement("span");
		this.#textEl.classList.add("text");
		this.el.appendChild(this.#textEl);

		this.#onDeleteClickCbs = new Set();

		this.#deleteButtonEl = document.createElement("button");
		this.#deleteButtonEl.classList.add("delete-button");
		this.#deleteButtonEl.type = "button";
		this.#deleteButtonEl.ariaLabel = "Delete ingredient"
		this.#deleteButtonEl.addEventListener("click", () => {
			this.#onDeleteClickCbs.forEach(cb => cb());
		});
		this.el.appendChild(this.#deleteButtonEl);

		this.boundPointerDown = this.#onPointerDown.bind(this);
		this.boundPointerMove = this.#onPointerMove.bind(this);
		this.boundPointerUp = this.#onPointerUp.bind(this);

		this.#grabHandleEl.addEventListener("pointerdown", this.boundPointerDown, {
			passive: false,
		});

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

	/**
	 * @param {PointerEvent} e
	 */
	#onPointerDown(e) {
		if (this.#isDragging) return;
		let allowed = true;
		for (const cb of this.#onDragStartCbs) {
			allowed = allowed && cb();
		}
		if (!allowed) return;
		e.preventDefault();
		this.#isDragging = true;
		this.#dragStart = e.clientY;
		this.el.classList.add("grabbing");

		window.addEventListener("pointermove", this.boundPointerMove, {
			passive: false,
		});
		window.addEventListener("pointerup", this.boundPointerUp, {
			passive: false,
		});
	}

	/**
	 * @param {PointerEvent} e
	 */
	#onPointerMove(e) {
		if (!this.#isDragging) return;
		e.preventDefault();
		this.el.style.transform = `translateY(${e.clientY - this.#dragStart}px)`;
		this.#onDragMoveCbs.forEach(cb => cb());
	}

	/**
	 * @param {PointerEvent} e
	 */
	#onPointerUp(e) {
		if (!this.#isDragging) return;
		e.preventDefault();
		this.#isDragging = false;
		this.el.style.transform = "";
		this.el.classList.remove("grabbing");

		window.removeEventListener("pointermove", this.boundPointerMove);
		window.removeEventListener("pointerup", this.boundPointerUp);
		this.#onDragEndCbs.forEach(cb => cb());
	}

	/**
	 * @param {() => boolean} cb
	 */
	onDragStart(cb) {
		this.#onDragStartCbs.add(cb);
	}

	/**
	 * @param {() => void} cb
	 */
	onDragMove(cb) {
		this.#onDragMoveCbs.add(cb);
	}

	/**
	 * @param {() => void} cb
	 */
	onDragEnd(cb) {
		this.#onDragEndCbs.add(cb);
	}
}
