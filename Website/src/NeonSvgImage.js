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

		/** @private */
		this.visible = false;
		const observer = new IntersectionObserver(entries => {
			for (const entry of entries) {
				if (entry.target === this.el) {
					this.visible = entry.isIntersecting;
					for (const path of this.flickeringPaths) {
						path.enabled = this.visible;
					}
				}
			}
		});
		observer.observe(this.el);

		this.loadNodes();
	}

	async loadNodes() {
		const nodes = await getSvg(this.id);
		this.el.appendChild(nodes);

		for (const node of nodes.querySelectorAll("path, line, polygon, polyline, ellipse, rect")) {
			const flickeringPath = new FlickeringSvgElement(node);
			this.flickeringPaths.push(flickeringPath);
			if (this.visible) {
				flickeringPath.enabled = true;
			}
		}
	}
}
