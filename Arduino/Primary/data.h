/*
  Written by JelleWho
  This file stores all data about all the types of drinks
*/
//Ingredients can NOT have a "." or "," or "-" in them!
enum IngredientE { UNK,
                   TRIPLE_SEC,
                   GIN,
                   LIME_JUICE,
                   LEMON_JUICE,
                   LEMONADE,
                   CRANBERRY_JUICE,
                   VODKA,
                   PEACH_TREE,
                   WHITE_RUM,
                   DARK_RUM,
                   TEQUILA,
                   GRENADINE_SYRUP,
                   COLA,
                   SPRITE,
                   ORANGE_JUICE,
                   PINEAPPLE_JUICE };
String IngredientS[] = { "UNK", "TRIPLE_SEC", "GIN", "LIME_JUICE", "LEMON_JUICE", "LEMONADE", "CRANBERRY_JUICE", "VODKA", "PEACH_TREE", "WHITE_RUM", "DARK_RUM", "TEQUILA", "GRENADINE_SYRUP", "COLA", "SPRITE", "ORANGE_JUICE", "PINEAPPLE_JUICE" };
enum COMMANDS { UNKNOWN,
                ADOPT,
                DISPENSE,
                CALIBRATEMSPERML,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR,
                DISPENSERSTATUS,
};