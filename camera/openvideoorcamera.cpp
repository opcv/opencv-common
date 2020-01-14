#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

const char* keys =
{
	"{help h usage ? | | print this message}"
    "{@video | | Video file, if not defined then use webcam}"
};

int main( int argc, const char** argv )
{
	CommandLineParser parser(argc, argv, keys);
    parser.about("Camera");
    if (parser.has("help"))
	{
	    parser.printMessage();
	    return 0;
	}

	String videoFile= parser.get<String>(0);

	if (!parser.check())
	{
	    parser.printErrors();
	    return 0;
	}

	VideoCapture cap;
	if(videoFile != "")
		cap.open(videoFile);
	else
		cap.open(0);
    if(!cap.isOpened())
        return -1;

    namedWindow("Video",1);
    for(;;)
    {
        Mat frame;
        cap >> frame;
        if(frame.empty())
        	return 0;
        imshow("Video", frame);
        if(waitKey(30) >= 0) break;
    }
    cap.release();

	return 0;
}