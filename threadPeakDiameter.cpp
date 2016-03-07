#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;

#define NUM_PEAKS 5

int get_thread_peak_diameter(const char* infile, const char* outfile);
Point get_left_most_pixel(Mat img);
Point get_right_most_pixel(Mat img);
double getDistance(Point p1, Point p2);


int get_thread_peak_diameter(const char* infile, const char* outfile) {
	Mat src = imread(infile, 0);

	if (src.empty()) {
		cout << "can not open " << infile << endl;
		return -1;
	}

	Mat dst, cdst;
	Canny(src, dst, 0, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	Point leftMostPeak = get_left_most_pixel(dst);
	Point rightMostPeak = get_right_most_pixel(dst);
	Size s = dst.size();

	line(src, leftMostPeak, rightMostPeak, Scalar(255, 0, 0), 2, CV_AA);
	imshow("colored", src);
	cout << "Thread Peak Diameter: " << getDistance(leftMostPeak, rightMostPeak) << " pixels";
	waitKey(0);
}

Point get_left_most_pixel(Mat img) {
	Size s = img.size();
	int leftMostCol = s.width;
	int row = 0;

	for (int c = 0; c < s.width; c++) {
		for (int r = 0; r < s.height; r++) {
			if (img.at<uchar>(r, c) == 255) {	// only look at white pixels
				if (c < leftMostCol) {
					leftMostCol = c;
					row = r;
				}
			}
		}
	}
	return Point(leftMostCol, row);
}

Point get_right_most_pixel(Mat img) {
	Size s = img.size();
	int rightMostCol = 0;
	int row = 0;

	for (int c = 0; c < s.width; c++) {
		for (int r = 0; r < s.height; r++) {
			if (img.at<uchar>(r, c) == 255) {	// only look at white pixels
				if (c > rightMostCol) {
					rightMostCol = c;
					row = r;
				}
			}
		}
	}
	return Point(rightMostCol, row);
}

double getDistance(Point p1, Point p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}