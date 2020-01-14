#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <ctime>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;

const char *keys =
    {
        "{help h usage ? | | print this message}"
        "{@video | | Video file, if not defined then use webcam}"
    };

void createAlphaMat(Mat &mat)
{
    CV_Assert(mat.channels() == 4);
    for (int i = 0; i < mat.rows; ++i)
    {
        for (int j = 0; j < mat.cols; ++j)
        {
            Vec4b &bgra = mat.at<Vec4b>(i, j);
            bgra[0] = UCHAR_MAX;                                                                   // Blue
            bgra[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
            bgra[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
            bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2]));                             // Alpha
        }
    }
}

int main(int argc, const char **argv)
{
    CommandLineParser parser(argc, argv, keys);
    parser.about("Camera");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String videoFile = parser.get<String>(0);

    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    VideoCapture cap;
    if (videoFile != "")
        cap.open(videoFile);
    else
        cap.open(0);
    if (!cap.isOpened())
        return -1;

    namedWindow("Video", 1);
    Mat mat(480, 640, CV_8UC4);
    createAlphaMat(mat);
    std::vector<int> compression_params;
    // compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    // compression_params.push_back(16);
    // compression_params.push_back(9);
    for (;;)
    {
        Mat frame;
        cap >> frame;
        try {
            auto t = std::time(nullptr);
            t = t / 5 * 5;
            std::cout << t << std::endl;
            std::string img = std::to_string(t);
            imwrite(img + ".png", frame);
        }
        catch (std::runtime_error& ex) {
            fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
            return 1;
        }
        if (frame.empty())
            return 0;
        imshow("Video", frame);
        if (waitKey(30) >= 0)
            break;
    }
    cap.release();

    return 0;
}

// #include "opencv2/opencv.hpp"

// using namespace cv;

// int main(int, char**)
// {
//     VideoCapture cap(0);
//     if(!cap.isOpened())
//         return -1;

//     Mat edges;
//     namedWindow("edges",1);
//     for(;;)
//     {
//         Mat frame;
//         cap >> frame;
//         cvtColor(frame, edges, CV_BGR2GRAY);
//         GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
//         Canny(edges, edges, 0, 30, 3);
//         imshow("edges", edges);
//         if(waitKey(30) >= 0) break;
//     }
//     return 0;
// }