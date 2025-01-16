#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unordered_set>

using namespace cv;

int lowThreshold = 20;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;

// void processImage(img){
//     // split image
//     Mat channel[3];
//     split(img,channel);
//     imshow("r",channel[0]);
//     imshow("b",channel[1]);
//     imshow("g",channel[2]);
    
// }

Mat detectEdgesSplit(Mat img) {
    Mat detected_edges;
    Mat grey;
    Mat grey_blurred;
    Mat r_blurred;
    Mat b_blurred;
    Mat g_blurred;
    Mat b_detected_edges;
    Mat g_detected_edges;
    Mat r_detected_edges;
    imshow( "test_img", img );

    Mat channel[3];
    split(img,channel);
    imshow("b",channel[0]);
    imshow("g",channel[1]);
    imshow("r",channel[2]);

    GaussianBlur(channel[0], b_blurred, Size(11, 11), 1.4, 0);
    GaussianBlur(channel[1], g_blurred, Size(11, 11), 1.4, 0);
    GaussianBlur(channel[2], r_blurred, Size(11, 11), 1.4, 0);
    Canny( b_blurred, b_detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    Canny( g_blurred, g_detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    Canny( r_blurred, r_detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    Mat dst_b = Mat::zeros(grey.size(), img.type());
    Mat dst_g = Mat::zeros(grey.size(), img.type());
    Mat dst_r = Mat::zeros(grey.size(), img.type());
    
    img.copyTo( dst_b, b_detected_edges);
    img.copyTo( dst_g, g_detected_edges);
    img.copyTo( dst_r, r_detected_edges);

    imshow( "dst_b", dst_b );
    imshow( "dst_g", dst_g );
    imshow( "dst_r", dst_r );

    cvtColor(img, grey, COLOR_BGR2GRAY);
    GaussianBlur(grey, grey_blurred, Size(11, 11), 1.4, 0);
    imshow( "grey blurred", grey_blurred );
    
    Mat dst = Mat::zeros(grey.size(), img.type());

    imshow( "test", dst );
    waitKey(0);
    return detected_edges;

}
Mat detectEdges(Mat img) {
    Mat detected_edges;
    Mat grey;
    Mat grey_blurred;

    std::cout << "Hello, World! first c++ project!" << std::endl;
    // Mat img = imread("/Users/jennali/Documents/Projects/cnc/ponder_headshot.jpg");
    imshow( "test_img", img );
 
    cvtColor(img, grey, COLOR_BGR2GRAY);
    GaussianBlur(grey, grey_blurred, Size(11, 11), 1.4, 0);
    imshow( "grey blurred", grey_blurred );
    Canny( grey_blurred, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    
    Mat dst = Mat::zeros(grey.size(), img.type());
    img.copyTo( dst, detected_edges);

    imshow( "orig edges", dst );
    return detected_edges;
 
}
 std::unordered_multiset<int> getZeroHierarchy(std::vector<Vec4i> hierarchy){
    std::unordered_multiset<int> outermost;
    int i = 0;
    bool notFound = true;
    while ((i != -1 || notFound) && (i < hierarchy.size())){
        if (notFound == false) {
            outermost.insert(i);
            i = hierarchy[i][0];
        }
        else if (hierarchy[i][3] == -1) {
            notFound = false;
            outermost.insert(i);
            i = hierarchy[i][0];
        }
        else{
            i +=1;
        }
    }
    return outermost; 
}

 std::unordered_multiset<int> getZeroHierarchyAndDraw
 (std::vector<Vec4i> hierarchy,std::vector<std::vector<Point>>contours, Mat img )
 {
    std::unordered_multiset<int> outermost;
    // Mat img = Mat::zeros( edges.size(), CV_8UC3 );
    int i = 0;
    bool notFound = true;
    while ((i != -1 || notFound) && (i < hierarchy.size())){
        if (notFound == false) {
            outermost.insert(i);
            Scalar color = Scalar(90,90,0);
            drawContours( img, contours, i, color, 2, LINE_8);
            i = hierarchy[i][0];

        }
        else if (hierarchy[i][3] == -1) {
            notFound = false;
            outermost.insert(i);
            Scalar color = Scalar(90,90,0);
            drawContours( img, contours, i, color, 2, LINE_8);
            i = hierarchy[i][0];
        }
        else{
            i +=1;
        }
    }
    imshow("only zero hierarchy", img);

    return outermost; 
}

// int[4] leastBoundingBox(Mat img){
//     int[4] boundingBox;
//     int totalWhite = countNonZero(img);
//     int width = img.size();
//     int height = img[0].size();

//     int top = 0;
//     int left = 0;
//     int right = 0;
//     int bottom = 0;

//     left = find(0, width/2, 1,true);
//     right = find(width, width/2, -2,true);
//     top = find(0, height/2, 1,false);
//     bottom = find(height, height/2, -1,false)


//     int find(int start, int end, int sign, bool isX){
//         if (isX){


//         }
//         else{

//         }
//     }
// }


std::tuple<std::vector<std::vector<Point>>, std::vector<Vec4i>> getContours(Mat edges) {
    RNG rng(12345);
    std::vector<Vec4i> hierarchy;
    std::vector<std::vector<Point>> contours;
    findContours( edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    Mat drawing = Mat::zeros( edges.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
    }
    std::cout << "Number of hierarchy elements: " << hierarchy.size() << std::endl;
    imshow( "Contours", drawing );
    return std::make_tuple(contours,hierarchy);
}
std::vector<std::vector<Point>> getBoundingBox(Mat img){

    int top = img.size().height-1;
    int bottom = 0;
    int left = img.size().width -1;
    int right = 0;

    for (size_t i = 0; i < img.size().height; i++) {
        for (size_t w = 0; w < img.size().width; w++) {
            uchar &pixel = img.at<uchar>(i, w);

            if (pixel == 255 && i < top) {
                top = i;
            }

            if (pixel == 255 && w < left) {
                left = w;
            }

            if (pixel == 255 && w > right) {
                right = w;
            }

            if (pixel== 255 && i > bottom) {
                bottom = i;
            }
        }
    }


    cv::Point lt(left, top);
    cv::Point rt(right, top);
    cv::Point lb(left, bottom);
    cv::Point rb(right, bottom);

    std::vector<cv::Point> contourPts = { lt, rt, rb, lb };

    std::vector<std::vector<cv::Point>> contours;
    contours.push_back(contourPts);
    // // may delete following two lines later.
    cv::drawContours(img, contours, 0, cv::Scalar(45,99,166), LINE_8);
    imshow("bounding box", img);
    return {contourPts};

}
int main() {
    Mat img = imread("/Users/jennali/Documents/Projects/cnc/ponder_headshot.jpg");
    RNG rng(12345);
    Mat edges = detectEdges(img);
    dilate(edges,edges,31);
    imshow("dilated edges", edges);
    auto [contours,hierarchy] = getContours(edges);

    std::unordered_multiset<int> zeroHierarchyIndices = getZeroHierarchy(hierarchy);
    Mat onlyHierarchyDrawing = Mat::zeros( edges.size(), CV_8UC3 );
    getZeroHierarchyAndDraw(hierarchy, contours, onlyHierarchyDrawing);
    getBoundingBox(edges);
    // imshow( "only heirarchy", onlyHierarchyDrawing );
    waitKey(0);
}