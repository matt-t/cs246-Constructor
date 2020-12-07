#ifndef __BOARD_H_ //should this be CONSTANTS_H
#define __BOARD_H_

#include <string>
#include <vector>

const std::string RESIDENCE_BASEMENT_STRING = "Basement";
const std::string RESIDENCE_HOUSE_STRING = "House";
const std::string RESIDENCE_TOWER_STRING = "Tower";

//
const std::string RESOURCE_BRICK_STRING = "Brick";
const std::string RESOURCE_ENERGY_STRING = "Energy";
const std::string RESOURCE_GLASS_STRING = "Glass";
const std::string RESOURCE_HEAT_STRING = "Heat";
const std::string RESOURCE_WIFI_STRING = "Wifi";

//
const std::string COLOR_BLUE_STRING = "Blue";
const std::string COLOR_RED_STRING = "Red";
const std::string COLOR_ORANGE_STRING = "Orange";
const std::string COLOR_YELLOW_STRING = "Yellow";
const std::string COLOR_NONE_STRING = "None";

const std::vector<std::vector<int>> VerticesRoads = {}; //to be implemented to map the vertices to a vector of their roads
const std::vector<std::vector<int>> RoadsVertices = {}; //to be implemented to map the roads to a vector of their vertices

const std::vector<std::vector<int>> TilesVertices = {};
const std::vector<std::vector<int>> TilesRoads = {};

#endif
