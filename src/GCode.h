#ifndef GCODE_H
#define GCODE_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>

using namespace cv;

class GCode {
private:
    double currFeed;
    double carveDepth;
    double travelHeight;

    std::string currCode;
    std::vector<std::vector<cv::Point>> paths;

    std::string compCode(std::string cmd);
    std::string compFeed(double newFeed);

public:
    GCode(std::vector<std::vector<Point>> contours, double depth, double height);

    double getCurrFeed();
    void setDepth(double depth);
    std::string getCurrCode();
    std::string generateMoveZ(double z, double feed);
    std::string generateRapidPos(double x, double y);
    std::string generateLinInterp(double x, double y, double feed);
    std::string generateClocCirInterp(double x, double y, double xRad, double yRad, double feed);
    std::string generateCounterClocCirInterp(double x, double y, double xRad, double yRad, double feed);
    int saveToFile(std::string str, std::string filename);
    std::vector<std::vector<cv::Point>> optimizePathOrder();
    std::vector<std::vector<cv::Point>> curveApproximation();

    std::string simpleGCodeGeneration();
};

#endif // GCODE_H
