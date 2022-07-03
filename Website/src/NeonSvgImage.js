import { FlickeringSvgElement } from "./FlickeringSvgElement.js";
import { getSvg } from "./svgFetching.js";

export class NeonSvgImage {
	/**
	 * @param {string} id
	 */
	constructor(id) {
		this.id = id;
		this.el = document.createElement("div");

		/** @type {FlickeringSvgElement[]} */
		this.flickeringPaths = [];

		this.loadNodes();
	}

	async loadNodes() {
		const nodes = await getSvg(this.id);
		this.el.appendChild(nodes);

		for (const node of nodes.querySelectorAll("path, line, polygon, polyline, ellipse, rect")) {
			const flickeringPath = new FlickeringSvgElement(node);
			this.flickeringPaths.push(flickeringPath);
		}
	}
}
