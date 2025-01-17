#include "GCode.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <unordered_set>
#include <fstream>

using namespace cv;

GCode::GCode(std::vector<std::vector<Point>> contours, double depth, double height) {
    paths = contours;
    carveDepth = depth;
    travelHeight = height;
}
double GCode::getCurrFeed() {
    return currFeed;
}

std::string GCode::getCurrCode() {
    return currCode;
}

std::string GCode::compCode(std::string cmd){
    if (cmd == currCode) {
        return "";
    }
    currCode = cmd;
    return currCode;
}

std::string GCode::compFeed(double newFeed){
    if (currFeed == newFeed) {
        return "";
    }
    currFeed = newFeed;
    return "F" + std::to_string(currFeed);
}

void GCode::setDepth(double depth) {
    carveDepth = depth;
}

std::string GCode::generateRapidPos(double x, double y){
    return compCode("G00") + "X" + std::to_string(x) + "Y" + std::to_string(y) + "\n";
}

std::string GCode::generateLinInterp(double x, double y, double feed){
    return compCode("G01") + "X" + std::to_string(x) + "Y" + std::to_string(y) + compFeed(feed) + "\n";
}

std::string GCode::generateClocCirInterp(double x, double y, double xRad, double yRad, double feed){
    return compCode("G03") + "X" + std::to_string(x) + "Y" + std::to_string(y)
           + "I" + std::to_string(xRad) + "J" + std::to_string(yRad) + compFeed(feed) + "\n";
}
std::string GCode::generateMoveZ(double z, double feed){
    return compCode("G01") + "Z" + std::to_string(z) + compFeed(feed) + "\n";
}

std::string GCode::generateCounterClocCirInterp(double x, double y, double xRad, double yRad, double feed){
    return compCode("G03") + "X" + std::to_string(x) + "Y" + std::to_string(y)
           + "I" + std::to_string(xRad) + "J" + std::to_string(yRad) + compFeed(feed) + "\n";
}

std::vector<std::vector<Point>> GCode::optimizePathOrder() {
    return paths;
}

std::vector<std::vector<Point>> GCode::curveApproximation() {
    return paths;
}

int GCode::saveToFile(std::string str, std::string filename){
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1; // Exit with an error
    }
    outFile << str;
    outFile.close();
    std::cout << "G-code saved to my_gcode.txt\n";
    return 0;
}

std::string GCode::simpleGCodeGeneration(){
    std::string gcode;
    for (auto &path : paths) {
        Point start = path[0];
        gcode += generateRapidPos(start.x, start.y);
        // move down;
        gcode += generateMoveZ(carveDepth, currFeed);
        for (auto &pt : path){
            gcode += generateLinInterp(pt.x, pt.y, currFeed);
        }
        // move up;
        gcode += generateMoveZ(travelHeight, currFeed);
    }
    // Probably you want to return `gcode`, not `currCode`.
    return gcode;
}
