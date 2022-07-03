/**
 * Linearly interpolates between two values.
 * If `t` is 0, returns the first value.
 * If `t` is 1, returns the second value.
 * If `t` is anything in between, returns the value at that point in the interpolation.
 * If `t` is outside of the range 0 to 1, the value is extrapolated. Meaning you will
 * get values outside of the `a` and `b` bounds.
 * @param {number} a The first value.
 * @param {number} b The second value.
 * @param {number} t A number between 0 and 1 that is used for interpolation.
 */
export function lerp(a, b, t) {
	return a + t * (b - a);
}

/**
 * The inverse of `lerp`, maps two values back to the 0-1 range.
 * If `t` is outside of the `a` and `b` bounds, the returned value will be outside of 0-1.
 * @param {number} a
 * @param {number} b
 * @param {number} t
 */
export function iLerp(a, b, t) {
	return (t - a) / (b - a);
}

/**
 * Clamps a value `v` between `min` and `max`.
 * @param {number} v
 * @param {number} min
 * @param {number} max
 */
export function clamp(v, min, max) {
	return Math.max(min, Math.min(max, v));
}

/**
 * Clamps a value between 0 and 1.
 * @param {number} v
 */
export function clamp01(v) {
	return clamp(v, 0, 1);
}

/**
 * Maps `value` from `fromMin` to `toMin` and from `fromMax` to `toMax`.
 * @param {number} fromMin
 * @param {number} fromMax
 * @param {number} toMin
 * @param {number} toMax
 * @param {number} val The value to map.
 * @param {boolean} performClamp Whether to clamp the result between `toMin` and `toMax`.
 * @returns {number}
 */
export function mapValue(fromMin, fromMax, toMin, toMax, val, performClamp = false) {
	let lerpedVal = iLerp(fromMin, fromMax, val);
	if (performClamp) lerpedVal = clamp01(lerpedVal);
	return lerp(toMin, toMax, lerpedVal);
}

/**
 * Returns int between min and max, max is exclusive (returns value between min and max - 1)
 * @param {number} min
 * @param {number} max
 */
export function randInt(min, max) {
	return Math.floor(randRange(min, max));
}

/**
 * Returns int between min and max, max is exclusive (returns value between min and max - 1)
 * @param {number} min
 * @param {number} max
 * @param {number} seed The seed to use for the RNG.
 */
export function randIntSeed(min, max, seed) {
	return Math.floor(randRangeSeed(min, max, seed));
}

/**
 * Returns number between min and max, max is exclusive, meaning this will never return
 * a value that is exactly equal to `max`.
 * @param {number} min
 * @param {number} max
 */
export function randRange(min, max) {
	return Math.random() * (max - min) + min;
}

/**
 * Returns number between min and max, max is exclusive, meaning this will never return
 * a value that is exactly equal to `max`.
 * @param {number} min
 * @param {number} max
 * @param {number} seed The seed to use for the RNG.
 */
 export function randRangeSeed(min, max, seed) {
	return randSeed(seed) * (max - min) + min;
}

/**
 * A RNG that uses a seed. The distribution is not perfect, but good enough
 * in most cases.
 * @param {number} seed
 * @returns
 */
export function randSeed(seed) {
	const x = Math.sin(seed++) * 10000;
	return x - Math.floor(x);
}
