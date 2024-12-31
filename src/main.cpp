#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
int main() {
    Mat detected_edges;
    Mat grey;
    Mat grey_blurred;
    Mat r_blurred;
    Mat b_blurred;
    Mat g_blurred;
    Mat b_detected_edges;
    Mat g_detected_edges;
    Mat r_detected_edges;
    std::cout << "Hello, World! first c++ project!" << std::endl;
    Mat img = imread("/Users/jennali/Documents/Projects/cnc/ponder_headshot.jpg");
    imshow( "test_img", img );
    // processImage(img);
    // split image
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
    Canny( grey_blurred, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    
    Mat dst = Mat::zeros(grey.size(), img.type());
    img.copyTo( dst, detected_edges);

    imshow( "test", dst );
    waitKey(0);
 
    


    // step 1: canny edges (just carve the outline)
    // step 2: use different depths for shading

    //  GOAL: use as little black box machine leanring models as possible. Do as much custom 3d
    // reconstruciton as possible. 
    // technique 2: Understand forground and background. (probably some machine learning model). 
    // carve out the background (further in) Probably need to do some 3d reconstrution of faces. 
}