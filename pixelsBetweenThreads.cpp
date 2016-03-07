#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;

#define NUM_PEAKS 5

int get_pixels_between_threads(const char* infile, const char* outfile);
double getAverageDistance(vector<Point> points);
double getDistance(Point p1, Point p2);

int main(int argc, char** argv) {
	get_pixels_between_threads("inputImages/tpi_just_thread.jpg", "outputImages/tpi_just_thread.jpg");
}

vector<Point> fivePeaks;

static void onMouse(int event, int x, int y, int, void* imgptr) {
	if (event != 1) return;     // only draw on lmouse down
	cout << "Mouse clicked at (" << x << "," << y << ")" << endl;
	Mat & img = (*(Mat*)imgptr); // first cast, then deref
	Point pt1 = Point(x, y);
	fivePeaks.push_back(pt1);
	circle(img, pt1, 3, Scalar(0, 0, 255), -1, 8, 0);

	imshow("RESULT", img);
	if (fivePeaks.size() == NUM_PEAKS) {
		double avgDistanceBetweenPeaks = getAverageDistance(fivePeaks);
		double threadsPerPixels = 1 / avgDistanceBetweenPeaks;
		cout << "\nCalculated Average Distance Between Peaks: " << avgDistanceBetweenPeaks << " pixels" << endl;
		cout << "Threads / Pixels: " << threadsPerPixels << endl;
	}
	waitKey(1);
}

/*
* Takes in a PROFILE image of the threaded end of the male screw and outputs the distance between 2 peaks.
* Assumptions: Profile image of ONLY THE THREADS of a male part.
* @param infile - the image to process
* @param outfile - the outputted image
* @return The number of pixels between two thread peaks if successful, -1 if unable to open the infile
*/
int get_pixels_between_threads(const char* infile, const char* outfile) {
	Mat src = imread(infile, 0);

	if (src.empty()) {
		cout << "can not open " << infile << endl;
		return -1;
	}

	Mat blackAndWhite, cdst;
	Canny(src, blackAndWhite, 0, 200, 3);
	cvtColor(blackAndWhite, cdst, CV_GRAY2BGR);

	namedWindow("DRAW ON HERE", CV_WINDOW_AUTOSIZE);
	imshow("DRAW ON HERE", blackAndWhite);

	setMouseCallback("DRAW ON HERE", onMouse, &blackAndWhite);

	char k = waitKey(0);
	if (k == 'ESC') {
		return 0;
	}
}





///////////////////////// HELPER FUNCTIONS /////////////////////////////



double getDistance(Point p1, Point p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double getAverageDistance(vector<Point> points) {
	int numPoints = points.size();
	if (numPoints != 5) return -1;	// get 5 points
	double totalDistance = 0;
	Point prev = points[0];

	for (Point p : points) {
		totalDistance += getDistance(prev, p);
		prev = p;
	}

	return totalDistance / (numPoints - 1);
}




