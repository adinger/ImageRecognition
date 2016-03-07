#include "imageRecognition.h"

#define NUM_PEAKS 5

ImageRecognition::ImageRecognition() {/*Constructor that does nothing*/}

// a container for the five peaks on the same profile of a thread
vector<Point> fivePeaks;


// click handler for getPixelsBetweenThreads
void ImageRecognition::onMouse(int event, int x, int y/*, void* imgptr*/) {
	if (event != 1) return;     // only draw on left mouse down
	cout << "Mouse clicked at (" << x << "," << y << ")" << endl;
	Mat & img = (*(Mat*)(this->imgptr)); // first cast, then deref
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
 * Gets the number of pixels between two peaks on the same profile of a thread
 * Assumptions: Profile image of ONLY THE THREADS of a male part.
 * @param infile - the image to process
 * @param outfile - the outputted image
 * @return The number of pixels between two thread peaks if successful, -1 if unable to open the infile
 */
int ImageRecognition::getPixelsBetweenThreads(const char* infile, const char* outfile) {
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

	// http://stackoverflow.com/questions/25748404/how-to-use-cvsetmousecallback-in-class
	this->imgptr = &blackAndWhite;
	setMouseCallback("DRAW ON HERE", ImageRecognition::onMouse, this);
	char k = waitKey(0);
	if (k == 'ESC') {
		return 0;
	}
}

/*
 * Gets diameter of the widest end of a thread
 * Assumptions: Profile image of ONLY THE THREADS of a male part.
 * @param infile - the image to process
 * @param outfile - the outputted image
 * @return The number of pixels between two thread peaks if successful, -1 if unable to open the infile
 */
int ImageRecognition::getThreadPeakDiameter(const char* infile, const char* outfile) {
	Mat src = imread(infile, 0);

	if (src.empty()) {
		cout << "can not open " << infile << endl;
		return -1;
	}

	Mat dst, cdst;
	Canny(src, dst, 0, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	Point leftMostPeak = getLeftMostPixel(dst);
	Point rightMostPeak = getRightMostPixel(dst);
	Size s = dst.size();

	line(src, leftMostPeak, rightMostPeak, Scalar(255,0,0), 2, CV_AA);
	imshow("colored", src);
	cout << "Thread Peak Diameter: " << getDistance(leftMostPeak, rightMostPeak) << " pixels";
	waitKey(0);
}

int ImageRecognition::detectCircles(const char* infile, const char* outfile) {
	Mat src, edges, gray, blurred;

	/// Read the image
	src = imread(infile, 1);
	imshow("Original", src);

	if (!src.data) return -1;
	
	cvtColor(src, gray, CV_BGR2GRAY);
	// smooth it, otherwise a lot of false circles may be detected
	GaussianBlur(gray, gray, Size(9,9), 2, 2);
	vector<Vec3f> circles;
	Canny(src, edges, 100, 200, 3, false); imshow("Canny", edges);
	//HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, 1, 100, 200, 0, 1000);
	HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, 10, 200, 100, 0, 1000);	// gets innermost circle very well
	// HoughCircles(image, circles, method, dp, minDist, cannyHighThresh, minVotes, minRadius=0, maxRadius=0 )

	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(src, center, radius, Scalar(0, 0, 255), 2, 8, 0);
		//circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
	}

	// Show results
	namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
	imshow("Hough Circle Transform Demo", src);
	imwrite(outfile, src);
	waitKey(0);
	return 0;
}

int ImageRecognition::detectLines(const char* infile, const char* outfile) {
	Mat src = imread(infile, 0);

	if (src.empty()) {
		cout << "can not open " << infile << endl;
		return -1;
	}

	Mat dst, cdst;
	Canny(src, dst, 0, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
#endif
	imshow("source", src);
	imshow("detected lines", cdst);
	imwrite(outfile, cdst);

	waitKey();

	return 0;
}

///////////////////////// HELPER FUNCTIONS /////////////////////////////

Point ImageRecognition::getLeftMostPixel(Mat img) {
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

Point ImageRecognition::getRightMostPixel(Mat img) {
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

double ImageRecognition::getDistance(Point p1, Point p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double ImageRecognition::getAverageDistance(vector<Point> points) {
	int numPoints = points.size();
	if (numPoints != 5) return -1;	// get 5 points
	double totalDistance = 0;
	Point prev = points[0];

	for (Point p : points) {
		totalDistance += getDistance(prev, p);
		prev = p;
	}

	return totalDistance / (numPoints-1);
}






