/**
 * @typedef FlickeringSvgElementOptions
 * @property {boolean} [warmUp] Whether the element should flicker a bit at the start.
 * @property {number} [onDurationMin] The minimum random time in ms before the light turns off.
 * @property {number} [onDurationMax] The maximum random time in ms before the light turns off.
 * @property {number} [offDurationMin] The minimum random time in ms before the light turns on again.
 * @property {number} [offDurationMax] The maximum random time in ms before the light turns on again.
 */

import { mapValue, randRange } from "./util.js";

export class FlickeringSvgElement {
	/**
	 * @param {Element} element
	 */
	constructor(element, {
		warmup = true,
		onDurationMin = 10_000,
		onDurationMax = 60_000,
		offDurationMin = 20,
		offDurationMax = 60,
	} = {}) {
		if (!(element instanceof SVGElement)) throw new Error("Assertion failed, element is not an svg node");
		this.element = element;

		this.onDurationMin = onDurationMin;
		this.onDurationMax = onDurationMax;
		this.offDurationMin = offDurationMin;
		this.offDurationMax = offDurationMax;

		this.originalColor = element.style.stroke;
		this.lightOn = true;
		this.isWarmingUp = warmup;
		this.warmUpDuration = randRange(1000, 2000);
		this.warmUpStartTime = 0;
		this.currentTimeout = -1;

		if (warmup) {
			this.isWarmingUp = true;
			this.warmUpStartTime = performance.now();
			this.toggleLight(false);
		}

		this.startNewTimeout();
	}

	startNewTimeout() {
		let timeout;
		if (this.isWarmingUp) {
			const t = performance.now() - this.warmUpStartTime;
			if (t >= this.warmUpDuration) {
				this.isWarmingUp = false;
				this.toggleLight(true);
				timeout = randRange(0, this.onDurationMax);
			} else {
				if (this.lightOn) {
					timeout = randRange(20, 60);
				} else {
					const min = mapValue(0, this.warmUpDuration, 500, 0, t);
					const max = mapValue(0, this.warmUpDuration, 1000, 0, t);
					timeout = randRange(min, max);
				}
			}
		} else {
			if (this.lightOn) {
				timeout = randRange(this.onDurationMin, this.onDurationMax);
			} else {
				timeout = randRange(this.offDurationMin, this.offDurationMax);
			}
		}
		this.currentTimeout = setTimeout(() => {
			this.toggleLight();
			this.startNewTimeout();
		}, timeout);
	}

	/**
	 * @param {boolean} [forcedValue]
	 */
	toggleLight(forcedValue) {
		if (forcedValue != undefined) {
			this.lightOn = forcedValue;
		} else {
			this.lightOn = !this.lightOn;
		}
		this.element.style.stroke = this.lightOn ? this.originalColor : "black";
	}
}
