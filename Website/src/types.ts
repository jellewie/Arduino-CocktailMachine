interface ShadowRoot {
	adoptedStyleSheets: readonly CSSStyleSheet[];
}

interface Document {
	adoptedStyleSheets: readonly CSSStyleSheet[];
}

// This makes Sets and Maps more strict by forcing their default parameters to `never`.
interface SetConstructor {
    new(): Set<never>;
}

interface MapConstructor {
    new(): Map<never, never>;
}
