/**
 * @typedef DrinkConfig
 * @property {string} name
 * @property {string} cssColor
 * @property {(DrinkConfigIngredient | DrinkConfigAction)[]} actions
 */

/**
 * @typedef DrinkConfigIngredient
 * @property {Ingredients} ingredient The ingredient to use.
 * @property {number} amount The amount in ml.
 */

/**
 * @typedef DrinkConfigAction
 * @property {Actions} action
 */

/** @typedef {"vodka" | "peachTree" | "orangeJuice" | "cranberryJuice" | "tripleSec" | "limeJuice" | "pineappleJuice" | "gin" | "lemonade" | "lemonJuice" | "whiteRum" | "rum" | "tequila" | "simpleSyrup" | "tonicWater" | "gingerAle" | "grenadineSyrup" | "cola" | "watermelonVodka" | "milk" | "baileys" | "sprite" | "lemonSoda"} Ingredients */

/** @typedef {"ice" | "shake" | "stir" | "crushedIce" | "blend" | "strain" | "2TheeSpoonsOfSugar" | "6LeavesOfMind"} Actions */

/** @type {DrinkConfig[]} */
export const drinksConfig = [
	{
		name: "Sex on the Beach",
		cssColor: "#bab34",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 45},
			{ingredient: "peachTree", amount: 45},
			{ingredient: "orangeJuice", amount: 135},
			{action: "shake"},
			{ingredient: "cranberryJuice", amount: 30},
		],
	}, {
		name: "Screwdriver",
		cssColor: "#e8d86d",
		actions: [
			{ingredient: "vodka", amount: 45},
			{ingredient: "orangeJuice", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Cosmopolitan Classic",
		cssColor: "#e81058",
		actions: [
			{ingredient: "vodka", amount: 23},
			{ingredient: "tripleSec", amount: 15},
			{ingredient: "cranberryJuice", amount: 30},
			{ingredient: "limeJuice", amount: 15},
			{action: "shake"},
		],
	}, {
		name: "Bay Breeze",
		cssColor: "#ff8400",
		actions: [
			{ingredient: "vodka", amount: 45},
			{ingredient: "pineappleJuice", amount: 30},
			{ingredient: "cranberryJuice", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Hole in One",
		cssColor: "#e3c532",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 45},
			{ingredient: "cranberryJuice", amount: 90},
			{action: "stir"},
			{ingredient: "orangeJuice", amount: 30},
		],
	}, {
		name: "Gimlet",
		cssColor: "#d5f5dd",
		actions: [
			{action: "crushedIce"},
			{ingredient: "gin", amount: 90},
			{ingredient: "limeJuice", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Black widow",
		cssColor: "#800101",
		actions: [
			{ingredient: "vodka", amount: 30},
			{ingredient: "cranberryJuice", amount: 30},
			{ingredient: "lemonade", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Gin Sidecar",
		cssColor: "#ced190",
		actions: [
			{action: "crushedIce"},
			{ingredient: "gin", amount: 60},
			{ingredient: "tripleSec", amount: 30},
			{ingredient: "lemonJuice", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Little Devil",
		cssColor: "#db4f40",
		actions: [
			{action: "crushedIce"},
			{ingredient: "whiteRum", amount: 45},
			{ingredient: "tripleSec", amount: 30},
			{ingredient: "lemonJuice", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Hoola Hoop",
		cssColor: "#e3bc5b",
		actions: [
			{ingredient: "gin", amount: 45},
			{ingredient: "orangeJuice", amount: 30},
			{ingredient: "pineappleJuice", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Hawaiian Cocktail",
		cssColor: "#edc766",
		actions: [
			{ingredient: "gin", amount: 60},
			{ingredient: "tripleSec", amount: 15},
			{ingredient: "pineappleJuice", amount: 15},
			{action: "shake"},
		],
	}, {
		name: "Alpine Lemonade",
		cssColor: "#f5e4bc",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 30},
			{ingredient: "gin", amount: 30},
			{ingredient: "rum", amount: 30},
			{ingredient: "lemonade", amount: 60},
			{ingredient: "cranberryJuice", amount: 60},
			{action: "blend"},
		],
	}, {
		name: "Hula Hula",
		cssColor: "#baa165",
		actions: [
			{action: "crushedIce"},
			{ingredient: "gin", amount: 60},
			{ingredient: "orangeJuice", amount: 30},
			{ingredient: "tripleSec", amount: 5},
			{action: "stir"},
			{action: "strain"},
		],
	}, {
		name: "Judge J Cocktail",
		cssColor: "#ed775f",
		actions: [
			{action: "crushedIce"},
			{ingredient: "gin", amount: 60},
			{ingredient: "whiteRum", amount: 60},
			{ingredient: "lemonJuice", amount: 30},
			{ingredient: "cranberryJuice", amount: 10},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Rock Bottom",
		cssColor: "#f27157",
		actions: [
			{ingredient: "vodka", amount: 60},
			{ingredient: "pineappleJuice", amount: 10},
			{ingredient: "gin", amount: 60},
			{ingredient: "lemonJuice", amount: 10},
			{ingredient: "limeJuice", amount: 10},
			{action: "shake"},
			{ingredient: "tequila", amount: 60},
		],
	}, {
		name: "Derby Daiquiri",
		cssColor: "#f7dd59",
		actions: [
			{action: "crushedIce"},
			{ingredient: "whiteRum", amount: 90},
			{ingredient: "orangeJuice", amount: 90},
			{ingredient: "limeJuice", amount: 60},
			{ingredient: "simpleSyrup", amount: 30},
			{action: "blend"},
		],
	}, {
		name: "The Huntsman",
		cssColor: "#c9f097",
		actions: [
			{ingredient: "vodka", amount: 45},
			{ingredient: "rum", amount: 15},
			{ingredient: "limeJuice", amount: 23},
			{ingredient: "simpleSyrup", amount: 8},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Vodka Tonic",
		cssColor: "#f4f5f2",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 60},
			{ingredient: "tonicWater", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Beach Bum Shake",
		cssColor: "#e3a452",
		actions: [
			{action: "crushedIce"},
			{ingredient: "tequila", amount: 30},
			{ingredient: "orangeJuice", amount: 60},
			{ingredient: "vodka", amount: 8},
			{ingredient: "tripleSec", amount: 5},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Gin Buck",
		cssColor: "#f0ede9",
		actions: [
			{action: "ice"},
			{ingredient: "gin", amount: 60},
			{ingredient: "lemonJuice", amount: 30},
			{ingredient: "gingerAle", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Pussy popper",
		cssColor: "#db5740",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 90},
			{ingredient: "tripleSec", amount: 60},
			{ingredient: "cranberryJuice", amount: 60},
			{ingredient: "orangeJuice", amount: 60},
			{ingredient: "grenadineSyrup", amount: 5},
			{action: "shake"},
		],
	}, {
		name: "Brass Monkey",
		cssColor: "#f5b922",
		actions: [
			{ingredient: "rum", amount: 15},
			{ingredient: "vodka", amount: 15},
			{action: "stir"},
			{ingredient: "orangeJuice", amount: 120},
			{action: "shake"},
		],
	}, {
		name: "Legspreader",
		cssColor: "#f0ecd5",
		actions: [
			{ingredient: "tequila", amount: 30},
			{ingredient: "vodka", amount: 30},
			{ingredient: "gin", amount: 30},
			{ingredient: "rum", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Leap Frog Highball",
		cssColor: "#f0ebd1",
		actions: [
			{action: "ice"},
			{ingredient: "gin", amount: 60},
			{ingredient: "lemonJuice", amount: 45},
			{ingredient: "gingerAle", amount: 60},
			{action: "stir"},
		],
	}, {
		name: "Vodka Sour",
		cssColor: "#edeadd",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 45},
			{ingredient: "lemonJuice", amount: 23},
			{ingredient: "simpleSyrup", amount: 13},
			{action: "shake"},
		],
	}, {
		name: "Orange Buck",
		cssColor: "#e8d997",
		actions: [
			{action: "ice"},
			{ingredient: "gin", amount: 60},
			{ingredient: "orangeJuice", amount: 60},
			{ingredient: "limeJuice", amount: 30},
			{action: "shake"},
			{ingredient: "gingerAle", amount: 60},
			{action: "stir"},
		],
	}, {
		name: "Liquid Bubble Gum",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "vodka", amount: 60},
			{ingredient: "peachTree", amount: 60},
			{ingredient: "cola", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Desert Sunrise",
		cssColor: "#FFFFFF",
		actions: [
			{action: "crushedIce"},
			{ingredient: "vodka", amount: 38},
			{ingredient: "orangeJuice", amount: 45},
			{ingredient: "pineappleJuice", amount: 45},
			{ingredient: "grenadineSyrup", amount: 5},
			{action: "stir"},
		],
	}, {
		name: "Absolute Vacation",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "cranberryJuice", amount: 30},
			{ingredient: "orangeJuice", amount: 30},
			{ingredient: "pineappleJuice", amount: 30},
			{ingredient: "vodka", amount: 30},
			{action: "shake"},
		],
	}, {
		name: "Hawaiian Seduction",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "tequila", amount: 30},
			{ingredient: "limeJuice", amount: 180},
			{ingredient: "vodka", amount: 30},
			{action: "stir"},
		],
	}, {
		name: "Hawaiian Sea Breeze",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 45},
			{ingredient: "pineappleJuice", amount: 135},
			{ingredient: "cranberryJuice", amount: 10},
			{action: "stir"},
		],
	}, {
		name: "Asian Cum Shot",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "watermelonVodka", amount: 90},
			{ingredient: "orangeJuice", amount: 180},
			{action: "stir"},
		],
	}, {
		name: "Island Punch",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "whiteRum", amount: 30},
			{ingredient: "grenadineSyrup", amount: 5},
			{ingredient: "orangeJuice", amount: 120},
			{ingredient: "pineappleJuice", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Jamaican Sunrise",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "orangeJuice", amount: 135},
			{ingredient: "vodka", amount: 60},
			{ingredient: "peachTree", amount: 60},
			{action: "shake"},
			{ingredient: "cranberryJuice", amount: 30},
		],
	}, {
		name: "Montana Sunrise",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "grenadineSyrup", amount: 23},
			{ingredient: "orangeJuice", amount: 60},
			{ingredient: "vodka", amount: 120},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Pimp Daddy",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "tequila", amount: 60},
			{ingredient: "vodka", amount: 60},
			{ingredient: "lemonJuice", amount: 30},
			{ingredient: "cranberryJuice", amount: 90},
			{action: "stir"},
		],
	}, {
		name: "Adam Cocktail",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "rum", amount: 60},
			{ingredient: "lemonJuice", amount: 30},
			{ingredient: "grenadineSyrup", amount: 30},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Long Island Taxi",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 60},
			{ingredient: "gin", amount: 30},
			{ingredient: "tequila", amount: 30},
			{ingredient: "rum", amount: 30},
			{ingredient: "orangeJuice", amount: 60},
			{action: "shake"},
			{action: "strain"},
		],
	}, {
		name: "Caribe Cosmopolitan",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "rum", amount: 45},
			{ingredient: "cranberryJuice", amount: 30},
			{ingredient: "tripleSec", amount: 30},
			{ingredient: "limeJuice", amount: 15},
			{action: "shake"},
		],
	}, {
		name: "Key West Screwdriver",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "vodka", amount: 60},
			{ingredient: "limeJuice", amount: 30},
			{ingredient: "orangeJuice", amount: 150},
			{action: "stir"},
		],
	}, {
		name: "Shark Tank",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "lemonade", amount: 120},
			{ingredient: "vodka", amount: 120},
			{ingredient: "grenadineSyrup", amount: 60},
		],
	}, {
		name: "A Gilligans Island",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "vodka", amount: 30},
			{ingredient: "peachTree", amount: 30},
			{ingredient: "orangeJuice", amount: 90},
			{ingredient: "cranberryJuice", amount: 90},
			{action: "shake"},
		],
	}, {
		name: "Baileys White Russian",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "milk", amount: 120},
			{ingredient: "baileys", amount: 60},
			{ingredient: "vodka", amount: 60},
			{action: "stir"},
		],
	}, {
		name: "Blind Bat",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "vodka", amount: 120},
			{ingredient: "cola", amount: 120},
			{ingredient: "whiteRum", amount: 10},
			{action: "stir"},
		],
	}, {
		name: "Hot Sex Cocktail",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "tripleSec", amount: 75},
			{ingredient: "orangeJuice", amount: 120},
			{action: "stir"},
			{ingredient: "grenadineSyrup", amount: 10},
		],
	}, {
		name: "Bloody Nightmare",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 30},
			{ingredient: "limeJuice", amount: 60},
			{ingredient: "cranberryJuice", amount: 210},
			{action: "stir"},
		],
	}, {
		name: "Snoop Dogg",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "gin", amount: 60},
			{ingredient: "orangeJuice", amount: 90},
			{action: "stir"},
		],
	}, {
		name: "Gin Citric",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "gin", amount: 60},
			{ingredient: "grenadineSyrup", amount: 5},
			{ingredient: "lemonJuice", amount: 30},
			{ingredient: "limeJuice", amount: 30},
			{ingredient: "orangeJuice", amount: 60},
			{action: "stir"},
		],
	}, {
		name: "Cat Cocktail",
		cssColor: "#FFFFFF",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 45},
			{ingredient: "tripleSec", amount: 15},
			{ingredient: "cranberryJuice", amount: 120},
			{action: "stir"},
		],
	}, {
		name: "Hard Dick",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "vodka", amount: 30},
			{ingredient: "gin", amount: 30},
			{ingredient: "whiteRum", amount: 30},
			{action: "stir"},
			{ingredient: "sprite", amount: 90},
		],
	}, {
		name: "Vodka soda",
		cssColor: "#964B00",
		actions: [
			{action: "ice"},
			{ingredient: "vodka", amount: 60},
			{ingredient: "cola", amount: 240},
		],
	}, {
		name: "Rum and coke",
		cssColor: "#964B00",
		actions: [
			{ingredient: "cola", amount: 120},
			{ingredient: "whiteRum", amount: 50},
			{ingredient: "limeJuice", amount: 10},
		],
	}, {
		name: "Long island iced tea",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "simpleSyrup", amount: 30},
			{ingredient: "tequila", amount: 15},
			{ingredient: "whiteRum", amount: 15},
			{ingredient: "lemonJuice", amount: 30},
			{ingredient: "tripleSec", amount: 15},
			{ingredient: "cola", amount: 45},
			{ingredient: "gin", amount: 15},
			{ingredient: "vodka", amount: 15},
		],
	}, {
		name: "Daiquiri",
		cssColor: "#FFFFFF",
		actions: [
			{ingredient: "whiteRum", amount: 45},
			{ingredient: "limeJuice", amount: 30},
			{ingredient: "simpleSyrup", amount: 15},
		],
	}, {
		name: "Margarita",
		cssColor: "#42F595",
		actions: [
			{ingredient: "tripleSec", amount: 30},
			{ingredient: "limeJuice", amount: 30},
			{ingredient: "tequila", amount: 60},
		],
	}, {
		name: "Mojito",
		cssColor: "#9DFF85",
		actions: [
			{ingredient: "whiteRum", amount: 45},
			{ingredient: "limeJuice", amount: 30},
			{action: "2TheeSpoonsOfSugar"},
			{action: "6LeavesOfMind"},
			{ingredient: "lemonSoda", amount: 105},
		],
	}, {
		name: "Gin and tonic",
		cssColor: "#D4D2D2",
		actions: [
			{ingredient: "gin", amount: 30},
			{ingredient: "tonicWater", amount: 90},
		],
	}
];
