const domParser = new DOMParser();

/** @type {Map<string, Promise<SVGElement>>} */
const cachedNodes = new Map();

/**
 * In development this fetches the svg file and converts to dom nodes.
 * But in production this will return the dom nodes from the html which have
 * been added by the bundler.
 * @param {string} id
 */
export async function getSvg(id) {
	let promise = cachedNodes.get(id);
	if (!promise) {
		promise = (async () => {
			if (DEBUG_BUILD) {
				const response = await fetch(`../svg/${id}.svg`);
				if (!response.ok) {
					throw new Error(`Failed to fetch svg: ${id}`);
				}
				const text = await response.text();
				const dom = domParser.parseFromString(text, "image/svg+xml");
				const childNode = dom.children[0];
				if (!(childNode instanceof SVGElement)) throw new Error("Assertion failed, not an svg node");
				return childNode;
			} else {
				const templateEl = document.getElementById(id);
				if (!(templateEl instanceof HTMLTemplateElement)) throw new Error(`Assertion failed, ${id} is not a template`);
				const svgEl = templateEl.content.children[0];
				if (!svgEl) throw new Error("Assertion failed, template has no children");
				if (!(svgEl instanceof SVGElement)) throw new Error("Assertion failed, not an svg node");
				return svgEl;
			}
		})();
		cachedNodes.set(id, promise);
	}
	const node = await promise;
	const cloned = node.cloneNode(true);
	if (!(cloned instanceof SVGElement)) throw new Error("Assertion failed, not an svg node");
	return cloned;
}
