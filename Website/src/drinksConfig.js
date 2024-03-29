// This file is automatically generated.
// To make modifications update the values in 'The Big cocktail mixer machine sheet'
// and click tools -> export drinksConfig.js
  
/**
 * @typedef DrinkConfig
 * @property {string} name
 * @property {DrinkIngredientOrAction[]} actions
 */

/** @typedef {DrinkConfigIngredient | DrinkConfigAction} DrinkIngredientOrAction */

/**
 * @typedef DrinkConfigIngredient
 * @property {Ingredients} ingredient The ingredient to use.
 * @property {number} amount The amount in ml.
 */

/**
 * @typedef DrinkConfigAction
 * @property {Actions} action
 */

/** @typedef {"VODKA" | "PEACH_TREE" | "ORANGE_JUICE" | "CRANBERRY_JUICE" | "TRIPLE_SEC" | "LIME_JUICE" | "PINEAPPLE_JUICE" | "GIN" | "LEMONADE" | "LEMON_JUICE" | "WHITE_RUM" | "DARK_RUM" | "TEQUILA" | "GRENADINE_SYRUP" | "COLA" | "SPRITE"} Ingredients */

/** @type {Map<Ingredients, string>} */
export const ingredientNames = new Map([
	["VODKA", "Vodka"],
	["PEACH_TREE", "Peach Tree"],
	["ORANGE_JUICE", "Orange juice"],
	["CRANBERRY_JUICE", "Cranberry juice"],
	["TRIPLE_SEC", "Triple Sec"],
	["LIME_JUICE", "Lime juice"],
	["PINEAPPLE_JUICE", "Pineapple juice"],
	["GIN", "Gin"],
	["LEMONADE", "Lemonade"],
	["LEMON_JUICE", "Lemon juice"],
	["WHITE_RUM", "White rum"],
	["DARK_RUM", "Dark rum"],
	["TEQUILA", "Tequila"],
	["GRENADINE_SYRUP", "Grenadine syrup"],
	["COLA", "Cola"],
	["SPRITE", "Sprite"],
]);

/** @typedef {"ice" | "shake" | "stir" | "crushed ice" | "blend" | "strain"} Actions */

/** @type {Actions[]} */
export const actionNames = [
	"ice",
	"shake",
	"stir",
	"crushed ice",
	"blend",
	"strain",
];

/** @type {DrinkConfig[]} */
export const drinksConfig = [
	{
		name: "Sex on the Beach",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 45},
			{ingredient: "PEACH_TREE", amount: 45},
			{ingredient: "ORANGE_JUICE", amount: 135},
			{action: "shake"},
			{ingredient: "CRANBERRY_JUICE", amount: 30},
		],
	}, {
		name: "Screwdriver",
		actions: [
			{ingredient: "VODKA", amount: 45},
			{ingredient: "ORANGE_JUICE", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Cosmopolitan Classic",
		actions: [
			{ingredient: "VODKA", amount: 23},
			{ingredient: "TRIPLE_SEC", amount: 15},
			{ingredient: "CRANBERRY_JUICE", amount: 30},
			{ingredient: "LIME_JUICE", amount: 15},
			{action: "shake"},
		],
	}, {
		name: "Bay Breeze",
		actions: [
			{ingredient: "VODKA", amount: 45},
			{ingredient: "PINEAPPLE_JUICE", amount: 30},
			{ingredient: "CRANBERRY_JUICE", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Hole in One",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 45},
			{ingredient: "CRANBERRY_JUICE", amount: 90},
			{action: "stir"},
			{ingredient: "ORANGE_JUICE", amount: 30},
		],
	}, {
		name: "Gimlet",
		actions: [
			{action: "crushed ice"},
			{ingredient: "GIN", amount: 90},
			{ingredient: "LIME_JUICE", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Black widow",
		actions: [
			{ingredient: "VODKA", amount: 30},
			{ingredient: "CRANBERRY_JUICE", amount: 30},
			{ingredient: "LEMONADE", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Gin Sidecar",
		actions: [
			{action: "crushed ice"},
			{ingredient: "GIN", amount: 60},
			{ingredient: "TRIPLE_SEC", amount: 30},
			{ingredient: "LEMON_JUICE", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Little Devil",
		actions: [
			{action: "crushed ice"},
			{ingredient: "GIN", amount: 60},
			{ingredient: "WHITE_RUM", amount: 45},
			{ingredient: "TRIPLE_SEC", amount: 30},
			{ingredient: "LEMON_JUICE", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Hoola Hoop",
		actions: [
			{ingredient: "GIN", amount: 45},
			{ingredient: "ORANGE_JUICE", amount: 30},
			{ingredient: "PINEAPPLE_JUICE", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Hawaiian Cocktail",
		actions: [
			{ingredient: "GIN", amount: 60},
			{ingredient: "TRIPLE_SEC", amount: 15},
			{ingredient: "PINEAPPLE_JUICE", amount: 15},
			{action: "shake"},
		],
	}, {
		name: "Alpine Lemonade",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 30},
			{ingredient: "GIN", amount: 30},
			{ingredient: "DARK_RUM", amount: 30},
			{ingredient: "LEMONADE", amount: 60},
			{ingredient: "CRANBERRY_JUICE", amount: 60},
			{action: "blend"},
		],
	}, {
		name: "Hula Hula",
		actions: [
			{action: "crushed ice"},
			{ingredient: "GIN", amount: 60},
			{ingredient: "ORANGE_JUICE", amount: 30},
			{ingredient: "TRIPLE_SEC", amount: 5},
			{action: "stir"},
			{action: "strain"},
		],
	}, {
		name: "Judge J Cocktail",
		actions: [
			{action: "crushed ice"},
			{ingredient: "GIN", amount: 60},
			{ingredient: "WHITE_RUM", amount: 60},
			{ingredient: "LEMON_JUICE", amount: 30},
			{ingredient: "CRANBERRY_JUICE", amount: 10},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Rock Bottom",
		actions: [
			{ingredient: "VODKA", amount: 60},
			{ingredient: "PINEAPPLE_JUICE", amount: 10},
			{ingredient: "GIN", amount: 60},
			{ingredient: "LEMON_JUICE", amount: 10},
			{ingredient: "LIME_JUICE", amount: 10},
			{action: "shake"},
			{ingredient: "TEQUILA", amount: 60},
		],
	}, {
		name: "Beach Bum Shake",
		actions: [
			{action: "crushed ice"},
			{ingredient: "TEQUILA", amount: 30},
			{ingredient: "ORANGE_JUICE", amount: 60},
			{ingredient: "VODKA", amount: 8},
			{ingredient: "TRIPLE_SEC", amount: 5},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Pussy popper",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 90},
			{ingredient: "TRIPLE_SEC", amount: 60},
			{ingredient: "CRANBERRY_JUICE", amount: 60},
			{ingredient: "ORANGE_JUICE", amount: 60},
			{ingredient: "GRENADINE_SYRUP", amount: 5},
			{action: "shake"},
		],
	}, {
		name: "Brass Monkey",
		actions: [
			{ingredient: "DARK_RUM", amount: 15},
			{ingredient: "VODKA", amount: 15},
			{action: "stir"},
			{ingredient: "ORANGE_JUICE", amount: 120},
			{action: "shake"},
		],
	}, {
		name: "Legspreader",
		actions: [
			{ingredient: "TEQUILA", amount: 30},
			{ingredient: "VODKA", amount: 30},
			{ingredient: "GIN", amount: 30},
			{ingredient: "DARK_RUM", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Liquid Bubble Gum",
		actions: [
			{ingredient: "VODKA", amount: 60},
			{ingredient: "PEACH_TREE", amount: 60},
			{ingredient: "COLA", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Desert Sunrise",
		actions: [
			{action: "crushed ice"},
			{ingredient: "VODKA", amount: 38},
			{ingredient: "ORANGE_JUICE", amount: 45},
			{ingredient: "PINEAPPLE_JUICE", amount: 45},
			{ingredient: "GRENADINE_SYRUP", amount: 5},
			{action: "stir"},
		],
	}, {
		name: "Absolute Vacation",
		actions: [
			{action: "ice"},
			{ingredient: "CRANBERRY_JUICE", amount: 30},
			{ingredient: "ORANGE_JUICE", amount: 30},
			{ingredient: "PINEAPPLE_JUICE", amount: 30},
			{ingredient: "VODKA", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Hawaiian Seduction",
		actions: [
			{action: "ice"},
			{ingredient: "TEQUILA", amount: 30},
			{ingredient: "LIME_JUICE", amount: 180},
			{ingredient: "VODKA", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Hawaiian Sea Breeze",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 45},
			{ingredient: "PINEAPPLE_JUICE", amount: 135},
			{ingredient: "CRANBERRY_JUICE", amount: 10},
			{action: "stir"},
		],
	}, {
		name: "Island Punch",
		actions: [
			{action: "ice"},
			{ingredient: "WHITE_RUM", amount: 30},
			{ingredient: "GRENADINE_SYRUP", amount: 5},
			{ingredient: "ORANGE_JUICE", amount: 120},
			{ingredient: "PINEAPPLE_JUICE", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Jamaican Sunrise",
		actions: [
			{ingredient: "ORANGE_JUICE", amount: 135},
			{ingredient: "VODKA", amount: 60},
			{ingredient: "PEACH_TREE", amount: 60},
			{action: "shake"},
			{ingredient: "CRANBERRY_JUICE", amount: 30},
		],
	}, {
		name: "Montana Sunrise",
		actions: [
			{action: "ice"},
			{ingredient: "GRENADINE_SYRUP", amount: 23},
			{ingredient: "ORANGE_JUICE", amount: 60},
			{ingredient: "VODKA", amount: 120},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Pimp Daddy",
		actions: [
			{action: "ice"},
			{ingredient: "TEQUILA", amount: 60},
			{ingredient: "VODKA", amount: 60},
			{ingredient: "LEMON_JUICE", amount: 30},
			{ingredient: "CRANBERRY_JUICE", amount: 90},
			{action: "stir"},
		],
	}, {
		name: "Adam Cocktail",
		actions: [
			{action: "ice"},
			{ingredient: "DARK_RUM", amount: 60},
			{ingredient: "LEMON_JUICE", amount: 30},
			{ingredient: "GRENADINE_SYRUP", amount: 30},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Long Island Taxi",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 60},
			{ingredient: "GIN", amount: 30},
			{ingredient: "TEQUILA", amount: 30},
			{ingredient: "DARK_RUM", amount: 30},
			{ingredient: "ORANGE_JUICE", amount: 60},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Caribe Cosmopolitan",
		actions: [
			{action: "ice"},
			{ingredient: "DARK_RUM", amount: 45},
			{ingredient: "CRANBERRY_JUICE", amount: 30},
			{ingredient: "TRIPLE_SEC", amount: 30},
			{ingredient: "LIME_JUICE", amount: 15},
			{action: "shake"},
		],
	}, {
		name: "Key West Screwdriver",
		actions: [
			{ingredient: "VODKA", amount: 60},
			{ingredient: "LIME_JUICE", amount: 30},
			{ingredient: "ORANGE_JUICE", amount: 150},
			{action: "stir"},
		],
	}, {
		name: "Shark Tank",
		actions: [
			{ingredient: "LEMONADE", amount: 120},
			{ingredient: "VODKA", amount: 120},
			{ingredient: "GRENADINE_SYRUP", amount: 60},
		],
	}, {
		name: "A Gilligans Island",
		actions: [
			{ingredient: "VODKA", amount: 30},
			{ingredient: "PEACH_TREE", amount: 30},
			{ingredient: "ORANGE_JUICE", amount: 90},
			{ingredient: "CRANBERRY_JUICE", amount: 90},
			{action: "shake"},
		],
	}, {
		name: "Blind Bat",
		actions: [
			{ingredient: "VODKA", amount: 120},
			{ingredient: "COLA", amount: 120},
			{ingredient: "WHITE_RUM", amount: 10},
			{action: "stir"},
		],
	}, {
		name: "Hot Sex Cocktail",
		actions: [
			{action: "ice"},
			{ingredient: "TRIPLE_SEC", amount: 75},
			{ingredient: "ORANGE_JUICE", amount: 120},
			{action: "stir"},
			{ingredient: "GRENADINE_SYRUP", amount: 10},
		],
	}, {
		name: "Bloody Nightmare",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 30},
			{ingredient: "LIME_JUICE", amount: 60},
			{ingredient: "CRANBERRY_JUICE", amount: 210},
			{action: "stir"},
		],
	}, {
		name: "Snoop Dogg",
		actions: [
			{ingredient: "GIN", amount: 60},
			{ingredient: "ORANGE_JUICE", amount: 90},
			{action: "stir"},
		],
	}, {
		name: "Gin Citric",
		actions: [
			{action: "ice"},
			{ingredient: "GIN", amount: 60},
			{ingredient: "GRENADINE_SYRUP", amount: 5},
			{ingredient: "LEMON_JUICE", amount: 30},
			{ingredient: "LIME_JUICE", amount: 30},
			{ingredient: "ORANGE_JUICE", amount: 60},
			{action: "stir"},
		],
	}, {
		name: "Cat Cocktail",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 45},
			{ingredient: "TRIPLE_SEC", amount: 15},
			{ingredient: "CRANBERRY_JUICE", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Hard Dick",
		actions: [
			{ingredient: "VODKA", amount: 30},
			{ingredient: "GIN", amount: 30},
			{ingredient: "WHITE_RUM", amount: 30},
			{action: "stir"},
			{ingredient: "SPRITE", amount: 90},
		],
	}, {
		name: "Vodka soda",
		actions: [
			{action: "ice"},
			{ingredient: "VODKA", amount: 60},
			{ingredient: "COLA", amount: 240},
		],
	}, {
		name: "Rum and coke",
		actions: [
			{ingredient: "COLA", amount: 120},
			{ingredient: "WHITE_RUM", amount: 50},
			{ingredient: "LIME_JUICE", amount: 10},
		],
	}, {
		name: "Margarita",
		actions: [
			{ingredient: "TRIPLE_SEC", amount: 30},
			{ingredient: "LIME_JUICE", amount: 30},
			{ingredient: "TEQUILA", amount: 60},
		],
	}
];