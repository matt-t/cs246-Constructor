#ifndef __CONSTANTS_H_ //should this be CONSTANTS_H
#define __CONSTANTS_H_

#include <string>
#include <vector>
#include <map>
#include "enums.h"

const std::vector<Color> COLOR_ORDER{Color::Blue, Color::Red, Color::Orange, Color::Yellow};

const int WINNING_POINTS = 10;

const int NUM_TILES = 19;
const int NUM_ROADS = 72;
const int NUM_VERTICES = 54;
const int NUM_PLAYERS = 4;

const std::string RESIDENCE_BASEMENT_STRING = "Basement";
const std::string RESIDENCE_HOUSE_STRING = "House";
const std::string RESIDENCE_TOWER_STRING = "Tower";

//
const std::string RESOURCE_BRICK_STRING = "BRICK";
const std::string RESOURCE_ENERGY_STRING = "ENERGY";
const std::string RESOURCE_GLASS_STRING = "GLASS";
const std::string RESOURCE_HEAT_STRING = "HEAT";
const std::string RESOURCE_WIFI_STRING = "WIFI";
const std::string RESOURCE_PARK_STRING = "PARK";

//
const std::string COLOR_BLUE_STRING = "Blue";
const std::string COLOR_RED_STRING = "Red";
const std::string COLOR_ORANGE_STRING = "Orange";
const std::string COLOR_YELLOW_STRING = "Yellow";
const std::string COLOR_NONE_STRING = "None";

const std::string GOOSE_STRING = "GOOSE";

const std::vector<int> ROLL_NUMBER_LIST = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
const std::vector<int> RESOURCE_LIST = {0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5};

const std::vector<std::vector<int>> VerticesRoads = { 
    {0, 1}, {0, 2}, {3, 5}, {1, 3, 6}, {2, 4, 7}, {4, 8}, //vertices 0 - 5
    {9, 12}, {5, 9, 13}, {6, 10, 14}, {7, 10, 15}, {8, 11, 16}, {11, 17}, //6-11
    {12, 20}, {13, 18, 21}, {14, 18, 22}, {15, 19, 23}, {16, 19, 24}, {17, 25},//12-17
    {20, 26, 29}, {21, 26, 30}, {22, 27, 31}, {23, 27, 32}, {24, 28, 33}, {25, 28, 34}, //18-23
    {29, 37}, {30, 35, 38}, {31, 35, 39}, {32, 36, 40}, {33, 36, 41}, {34, 42}, //24-29
    {37, 43, 46}, {38, 43, 47}, {39, 44, 48}, {40, 44, 49}, {41, 45, 50}, {42, 45, 51}, //30-35
    {46, 54}, {47, 52, 55}, {48, 52, 56}, {49, 53, 57}, {50, 53, 58}, {51, 59}, //36-41
    {54, 60}, {55, 60, 63}, {56, 61, 64}, {57, 61, 65}, {58, 62, 66}, {59, 62}, //42-47
    {63, 67}, {64, 67, 69}, {65, 68, 70}, {66, 68}, {69, 71}, {70, 71} //48-53
    }; //map the vertices to a vector of their roads
const std::vector<std::vector<int>> RoadsVertices = { 
    {0, 1}, {0, 3}, {1, 4}, {2, 3}, {4, 5}, {2, 7}, {3, 8}, {4, 9}, {5, 10}, //roads 0-8
    {6, 7}, {8, 9}, {10, 11}, {6, 12}, {7, 13}, {8, 14}, {9, 15}, {10, 16}, {11, 17},//9-17
    {13, 14}, {15, 16}, {12, 18}, {13, 19}, {14, 20}, {15, 21}, {16, 22}, {17, 23}, //18-25
    {18, 19}, {20, 21}, {22, 23}, {18, 24}, {19, 25}, {20, 26}, {21, 27}, {22, 28}, {23, 29}, //26-34
    {25, 26}, {27, 28}, {24, 30}, {25, 31}, {26, 32}, {27, 33}, {28, 34}, {29, 35}, //35-42
    {30, 31}, {32, 33}, {34, 35}, {30, 36}, {31, 37}, {32, 38}, {33, 39}, {34, 40}, {35, 41}, //43-51
    {37, 38}, {39, 40}, {36, 42}, {37, 43}, {38, 44}, {39, 45}, {40, 46}, {41, 47}, //52-59
    {42, 43}, {44, 45}, {46, 47}, {43, 48}, {44, 49}, {45, 50}, {46, 51}, {48, 49}, {50, 51}, {49, 52}, {50, 53}, {52, 53}//60-71
}; //to map the roads to a vector of their vertices
const std::vector<std::vector<int>> TilesVertices = {
    {0, 1, 3, 4, 8, 9}, {2, 3, 7, 8, 13, 14}, {4, 5, 9, 10, 15, 16}, {6, 7, 12, 13, 18, 19}, {8, 9, 14, 15, 20, 21}, {10, 11, 16, 17, 22, 23}, //0-5
    {13, 14, 19, 20, 25, 26}, {15, 16, 21, 22, 27, 28}, {18, 19, 24, 25, 30, 31}, {20, 21, 26, 27, 32, 33}, {22, 23, 28, 29, 34, 35}, {25, 26, 31, 32, 37, 38}, //6-11
    {27, 28, 33, 34, 39, 40}, {30, 31, 36, 37, 42, 43}, {32, 33, 38, 39, 44, 45}, {34, 35, 40, 41, 46, 47}, //12-15
    {37, 38, 43, 44, 48, 49}, {39, 40, 45, 46, 50, 51}, {44, 45, 49, 50, 52, 53} //16-18
};
const std::vector<std::vector<int>> TilesRoads = {
    {0, 1, 2, 6, 7, 10}, {3, 5, 6, 13, 14, 18}, {4, 7, 8, 15, 16, 19}, {9, 12, 13, 20, 21, 26}, {10, 14, 15, 22, 23, 27}, {11, 16, 17, 24, 25, 28},//0-5
    {18, 21, 22, 30, 31, 35}, {19, 23, 24, 32, 33, 36}, {26, 29, 30, 37, 38, 43}, {27, 31, 32, 39, 40, 44}, {28, 33, 34, 41, 42, 45}, {35, 38, 39, 47, 48, 52}, //6-11
    {36, 40, 41, 49, 50, 53}, {43, 46, 47, 54, 55, 60}, {44, 48, 49, 56, 67, 61}, {45, 50, 51, 58, 59, 62}, //12-15
    {52, 55, 56, 63, 64, 67}, {53, 57, 58, 65, 66, 68}, {61, 64, 65, 69, 70, 71} //16-18
};

const int MAX_DICE_ROLL = 6;
const int MIN_DICE_ROLL = 1;

const int UNINITIALIZED_GEESE = -1;
const int GEESE_ROLL = 7;
const int GEESE_STEAL_FROM = 10;

const int MARKET_TRADE_RATE = 4;

const int TOP_LEFT_VERTEX = 0;
const int TOP_RIGHT_VERTEX = 1;
const int MIDDLE_LEFT_VERTEX = 2;
const int MIDDLE_RIGHT_VERTEX = 3;
const int BOTTOM_LEFT_VERTEX = 4;
const int BOTTOM_RIGHT_VERTEX = 5;

const int TOP_MIDDLE_ROAD = 0;
const int TOP_LEFT_ROAD = 1;
const int TOP_RIGHT_ROAD = 2;
const int BOTTOM_LEFT_ROAD = 3;
const int BOTTOM_RIGHT_ROAD = 4;
const int BOTTOM_MIDDLE_ROAD = 5;

const std::string DEFAULT_SAVE_FILE = "backup.sv";

const std::map<Color, char> COLOR_TO_CHAR = {
    { Color::Blue, 'B' }, 
    { Color::Red, 'R' },
    { Color::Yellow, 'Y' },
    { Color::Orange, 'O' },
    { Color::None, 'N'}
};

const std::map<char, Residence> CHAR_TO_RESIDENCE = {
    { 'B', Residence::Basement },
    { 'H', Residence::House},
    { 'T', Residence::Tower },
};

const std::map<Residence, int> RESIDENCE_TO_POINTS = {
    { Residence::Basement, 1 },
    { Residence::House, 2},
    { Residence::Tower, 3},
};

const std::map<Residence, char> RESIDENCE_TO_CHAR = {
    { Residence::Basement, 'B' },
    { Residence::House, 'H'},
    { Residence::Tower, 'T'},
    { Residence::None, '?'}
};

const std::map<Resource, int> BASEMENT_COST = {
    { Resource::Brick, 1 },
    { Resource::Energy, 1 },
    { Resource::Glass, 1 },
    { Resource::Wifi, 1 }
};

const std::map<Resource, int> HOUSE_COST = {
    { Resource::Glass, 2 },
    { Resource::Heat, 3 }
};

const std::map<Resource, int> TOWER_COST = {
    { Resource::Brick, 3 },
    { Resource::Energy, 2 },
    { Resource::Glass, 2 },
    { Resource::Wifi, 1 },
    { Resource::Heat, 2 }
};

const std::map<Resource, int> ROAD_COST = {
    { Resource::Heat, 1 },
    { Resource::Wifi, 1 }
};

const std::map<std::string, Color> STRING_TO_COLOR = {
    { "B", Color::Blue}, 
    { "BLUE", Color::Blue}, 
    { "R", Color::Red },
    { "RED", Color::Red },
    { "Y", Color::Yellow},
    { "YELLOW", Color::Yellow},
    { "O", Color::Orange},
    { "ORANGE", Color::Orange}
};

const std::map<Color, std::string> COLOR_TO_STRING = {
    {Color::Blue, "Blue"}, 
    {Color::Red, "Red"}, 
    {Color::Yellow, "Yellow"},
    {Color::Orange, "Orange"}
};
const std::map<std::string, Resource> STRING_TO_RESOURCE = {
    { "BRICK", Resource::Brick}, 
    { "ENERGY", Resource::Energy}, 
    { "GLASS", Resource::Glass},
    { "HEAT", Resource::Heat},
    { "WIFI", Resource::Wifi}
};

const std::map<Resource, std::string> RESOURCE_TO_STRING = {
    {Resource::Brick, "Brick"}, 
    {Resource::Energy, "Energy"}, 
    {Resource::Glass, "Glass"},
    {Resource::Heat, "Heat"},
    {Resource::Wifi, "Wifi"},
    {Resource::Park, "Park"}
};


const std::map<Resource, int> RESOURCE_TO_INT = {
    {Resource::Brick, 0}, 
    {Resource::Energy, 1}, 
    {Resource::Glass, 2},
    {Resource::Heat, 3},
    {Resource::Wifi, 4},
    {Resource::Park, 5},
};
const std::map<int, Resource> INT_TO_RESOURCE = {
    {0, Resource::Brick}, 
    {1, Resource::Energy}, 
    {2, Resource::Glass},
    {3, Resource::Heat},
    {4, Resource::Wifi},
    {5, Resource::Park},
};

#endif
