#ifndef IMAGERECOGNITION_H
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;

class ImageRecognition {
public:
	// constructor that does nothing
	ImageRecognition();	
	// detects the lines in an image
	int detectLines(const char* infile, const char* outfile);
	// detects the circles in an image
	int detectCircles(const char* infile, const char* outfile);
	// gets diameter of the widest end of a thread
	int getThreadPeakDiameter(const char* infile, const char* outfile);
	// gets the number of pixels between two peaks on the same side of the screw's profile
	int getPixelsBetweenThreads(const char* infile, const char* outfile);

private: // helpers for the functions above
	void* imgptr;
	// gets the leftmost white pixel in a black-and-white image
	Point getLeftMostPixel(Mat img);
	// gets the rightmost white pixel in a black-and-white image
	Point getRightMostPixel(Mat img);
	// gets the distance between two points
	double getDistance(Point p1, Point p2);
	// gets the average distance between every pair of adjacent points in a list of points
	double getAverageDistance(vector<Point> points);
	// click handler for getPixelsBetweenThreads
	void onMouse(int event, int x, int y);
	static void onMouse(int event, int x, int y, void* userdata);
};

#endif