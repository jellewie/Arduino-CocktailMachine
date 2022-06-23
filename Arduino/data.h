/*
  Written by JelleWho
  This file stores all data about all the types of drinks
*/
//Ingredients can NOT have a "." or "," or "-" in them!
enum IngredientE {UNK, VODKA, GIN, ORANGE_JUICE, RUM, CRANBERRY_JUICE, LEMONADE, TRIPLE_SEC, LEMON_JUICE, COLA, SPRITE, TEQUILA, GRENADINE_SYRUP, MILK, PINEAPPLE_JUICE, LIME_JUICE, PEACH_TREE, WHITE_RUM, TONIC_WATER, WATERMELON_VODKA, GINGER_ALE, SIMPLE_SYRUP};
String IngredientS[] = {"UNK", "VODKA", "GIN", "ORANGE_JUICE", "RUM", "CRANBERRY_JUICE", "LEMONADE", "TRIPLE_SEC", "LEMON_JUICE", "COLA", "SPRITE", "TEQUILA", "GRENADINE_SYRUP", "MILK", "PINEAPPLE_JUICE", "LIME_JUICE", "PEACH_TREE", "WHITE_RUM", "TONIC_WATER", "WATERMELON_VODKA", "GINGER_ALE", "SIMPLE_SYRUP"};

Drink Drinks[] = {
  {"Sex on the Beach", {{0, "#ice", 0}, {1, "", 45}, {17, "", 45}, {3, "", 135}, {0, "#shake", 0}, {5, "", 30}}},
  {"Screwdriver", {{1, "", 45}, {3, "", 120}, {0, "#stir", 0}}},
  {"Cosmopolitan Classic", {{1, "", 23}, {7, "", 15}, {5, "", 30}, {16, "", 15}, {0, "#shake", 0}}},
  {"Bay Breeze", {{1, "", 45}, {15, "", 30}, {5, "", 30}, {0, "#stir", 0}}},
  {"Hole in One", {{0, "#ice", 0}, {1, "", 45}, {5, "", 90}, {0, "#stir", 0}, {3, "", 30}}},
  {"Gimlet", {{0, "#crushed ice", 0}, {2, "", 90}, {16, "", 30}, {0, "#shake", 0}}},
  {"Black widow", {{1, "", 30}, {5, "", 30}, {6, "", 30}, {0, "#stir", 0}}},
  {"Gin Sidecar", {{0, "#crushed ice", 0}, {2, "", 60}, {7, "", 30}, {8, "", 30}, {0, "#shake", 0}}},
  {"Little Devil", {{0, "#crushed ice", 0}, {2, "", 60}, {18, "", 45}, {7, "", 30}, {8, "", 30}, {0, "#shake", 0}}},
  {"Hoola Hoop", {{2, "", 45}, {3, "", 30}, {15, "", 30}, {0, "#stir", 0}}},
  {"Hawaiian Cocktail", {{2, "", 60}, {7, "", 15}, {15, "", 15}, {0, "#shake", 0}}},
  {"Alpine Lemonade", {{0, "#ice", 0}, {1, "", 30}, {2, "", 30}, {4, "", 30}, {6, "", 60}, {5, "", 60}, {0, "#blend", 0}}},
  {"Hula-Hula", {{0, "#crushed ice", 0}, {2, "", 60}, {3, "", 30}, {7, "", 5}, {0, "#stir", 0}, {0, "#strain", 0}}},
  {"Judge, Jr. Cocktail", {{0, "#crushed ice", 0}, {2, "", 60}, {18, "", 60}, {8, "", 30}, {5, "", 10}, {0, "#shake", 0}, {0, "#strain", 0}}},
  {"Rock Bottom", {{1, "", 60}, {15, "", 10}, {2, "", 60}, {8, "", 10}, {16, "", 10}, {0, "#shake", 0}, {11, "", 60}}},
  {"Derby Daiquiri", {{0, "#crushed ice", 0}, {18, "", 90}, {3, "", 90}, {16, "", 60}, {22, "", 30}, {0, "#blend", 0}}},
  {"The Huntsman", {{1, "", 45}, {4, "", 15}, {16, "", 23}, {22, "", 8}, {0, "#shake", 0}, {0, "#strain", 0}}},
  {"Vodka Tonic", {{0, "#ice", 0}, {1, "", 60}, {19, "", 120}, {0, "#stir", 0}}},
  {"Beach Bum Shake", {{0, "#crushed ice", 0}, {11, "", 30}, {3, "", 60}, {1, "", 8}, {7, "", 5}, {0, "#shake", 0}, {0, "#strain", 0}}},
  {"Gin Buck", {{0, "#ice", 0}, {2, "", 60}, {8, "", 30}, {21, "", 30}, {0, "#stir", 0}}},
  {"Pussy popper", {{0, "#ice", 0}, {1, "", 90}, {7, "", 60}, {5, "", 60}, {3, "", 60}, {12, "", 5}, {0, "#shake", 0}}},
  {"Brass Monkey", {{4, "", 15}, {1, "", 15}, {0, "#stir", 0}, {3, "", 120}, {0, "#shake", 0}}},
  {"Legspreader", {{11, "", 30}, {1, "", 30}, {2, "", 30}, {4, "", 30}, {0, "#shake", 0}}},
  {"Leap Frog Highball", {{0, "#ice", 0}, {2, "", 60}, {8, "", 45}, {21, "", 60}, {0, "#stir", 0}}},
  {"Vodka Sour", {{0, "#ice", 0}, {1, "", 45}, {8, "", 23}, {22, "", 13}, {0, "#shake", 0}}},
  {"Orange Buck", {{0, "#ice", 0}, {2, "", 60}, {3, "", 60}, {16, "", 30}, {0, "#shake", 0}, {21, "", 60}, {0, "#stir", 0}}},
  {"Liquid Bubble Gum", {{1, "", 60}, {17, "", 60}, {9, "", 120}, {0, "#stir", 0}}},
  {"Desert Sunrise", {{0, "#crushed ice", 0}, {1, "", 38}, {3, "", 45}, {15, "", 45}, {12, "", 5}, {0, "#stir", 0}}},
  {"Absolute Vacation", {{0, "#ice", 0}, {5, "", 30}, {3, "", 30}, {15, "", 30}, {1, "", 30}, {0, "#shake", 0}}},
  {"Hawaiian Seduction", {{0, "#ice", 0}, {11, "", 30}, {16, "", 180}, {1, "", 30}, {0, "#stir", 0}}},
  {"Hawaiian Sea Breeze", {{0, "#ice", 0}, {1, "", 45}, {15, "", 135}, {5, "", 10}, {0, "#stir", 0}}},
  {"Asian Cum Shot", {{0, "#ice", 0}, {20, "", 90}, {3, "", 180}, {0, "#stir", 0}}},
  {"Island Punch", {{0, "#ice", 0}, {18, "", 30}, {12, "", 5}, {3, "", 120}, {15, "", 120}, {0, "#stir", 0}}},
  {"Jamaican Sunrise", {{3, "", 135}, {1, "", 60}, {17, "", 60}, {0, "#shake", 0}, {5, "", 30}}},
  {"Montana Sunrise", {{0, "#ice", 0}, {12, "", 23}, {3, "", 60}, {1, "", 120}, {0, "#shake", 0}, {0, "#strain", 0}}},
  {"Pimp Daddy", {{0, "#ice", 0}, {11, "", 60}, {1, "", 60}, {8, "", 30}, {5, "", 90}, {0, "#stir", 0}}},
  {"Adam Cocktail", {{0, "#ice", 0}, {4, "", 60}, {8, "", 30}, {12, "", 30}, {0, "#shake", 0}, {0, "#strain", 0}}},
  {"Long Island Taxi", {{0, "#ice", 0}, {1, "", 60}, {2, "", 30}, {11, "", 30}, {4, "", 30}, {3, "", 60}, {0, "#shake", 0}, {0, "#strain",}}},
  {"Caribe Cosmopolitan", {{0, "#ice", 0}, {4, "", 45}, {5, "", 30}, {7, "", 30}, {16, "", 15}, {0, "#shake", 0}}},
  {"Key West Screwdriver", {{1, "", 60}, {16, "", 30}, {3, "", 150}, {0, "#stir", 0}}},
  {"Shark Tank", {{6, "", 120}, {1, "", 120}, {12, "", 60}}},
  {"A Gilligan's Island", {{1, "", 30}, {17, "", 30}, {3, "", 90}, {5, "", 90}, {0, "#shake", 0}}},
  {"Bailey's White Russian", {{0, "#ice", 0}, {0, "Milk ", 120}, {0, "Bailey's", 60}, {1, "", 60}, {0, "#stir", 0}}},
  {"Blind Bat", {{1, "", 120}, {9, "", 120}, {18, "", 10}, {0, "#stir", 0}}},
  {"Hot Sex Cocktail", {{0, "#ice", 0}, {7, "", 75}, {3, "", 120}, {0, "#stir", 0}, {12, "", 10}}},
  {"Bloody Nightmare", {{0, "#ice", 0}, {1, "", 30}, {16, "", 60}, {5, "", 210}, {0, "#stir", 0}}},
  {"Snoop Dogg", {{0, "Gin ", 60}, {3, "", 90}, {0, "#stir", 0}}},
  {"Gin Citric", {{0, "#ice", 0}, {2, "", 60}, {12, "", 5}, {8, "", 30}, {16, "", 30}, {3, "", 60}, {0, "#stir", 0}}},
  {"Cat Cocktail", {{0, "#ice", 0}, {1, "", 45}, {7, "", 15}, {5, "", 120}, {0, "#stir", 0}}},
  {"Hard Dick", {{1, "", 30}, {2, "", 30}, {18, "", 30}, {0, "#stir", 0}, {10, "", 90}}}
};

Dispenser Dispensers[Dispensers_Amount] = {
  //Type        , X,     Y,   Z  , MSml, MSoff, IngredientID
  {PUMP         , 3000 , 0  , 1  , 500 , 500  , COLA},
  {PUMP         , 3000 , 100, 2  , 500 , 500  , ORANGE_JUICE},
  {SHOTDispenser, 6000 , 0  , 50 , 100 , 1000 , VODKA},
  {SHOTDispenser, 9000 , 0  , 50 , 100 , 1000 , GIN},
  {SHOTDispenser, 11000, 0  , 50 , 100 , 1000 , 4},
  {SHOTDispenser, 14000, 0  , 50 , 100 , 1000 , 5},
  {SHOTDispenser, 17000, 0  , 50 , 100 , 1000 , 6},
  {SHOTDispenser, 20000, 0  , 50 , 100 , 1000 , 7},
  {SHOTDispenser, 23000, 0  , 50 , 100 , 1000 , 8},
  {SHOTDispenser, 26000, 0  , 50 , 100 , 1000 , 17}
};
