#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;

int detect_lines(const char* infile, const char* outfile);
int detect_circles(const char* infile, const char* outfile);

int main(int argc, char** argv)
{
	//detect_lines("inputImages/lines-easy.jpg", "outputImages/lines-easy.jpg");
	//detect_lines("inputImages/lines-medium.jpg", "outputImages/lines-medium.jpg");
	//detect_circles("inputImages/circles-easy.jpg", "outputImages/circles-easy.jpg");
	detect_lines("inputImages/tpi1.jpg", "outputImages/tpi1.jpg");
	detect_lines("inputImages/tpi2.jpg", "outputImages/tpi2.jpg");

	// 1242060 (big part)
	//detect_circles("inputImages/1242060-easy.jpg", "outputImages/1242060-easy.jpg");
	//detect_circles("inputImages/1242060-medium.jpg", "outputImages/1242060-medium.jpg");
	//detect_circles("inputImages/1242060_hard.jpg", "outputImages/1242060_hard.jpg");

	// 1463408, 1463414 (threaded parts)
	//detect_circles("inputImages/1463408-carpet.jpg", "outputImages/1463408-carpet.jpg");
	//detect_circles("inputImages/1463414-carpet.jpg", "outputImages/1463414-carpet.jpg");

	//detect_circles("inputImages/1498476-easy.jpg", "outputImages/1498476-easy.jpg");


	// detect_circles("inputImages/multiple-hard.jpg", "outputImages/multiple-hard.jpg"); // fail

}


int detect_circles(const char* infile, const char* outfile) {
	Mat src, src_gray;

	/// Read the image
	src = imread(infile, 1);

	if (!src.data) return -1;

	/// Convert it to gray
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Reduce the noise so we avoid false circle detection
	GaussianBlur(src_gray, src_gray, Size(9,9), 2, 2);

	vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, 5, 
				200, 100, 0, 0);
	// void HoughCircles(InputArray image, OutputArray circles, int method, double dp, double minDist, 
	//						double param1=100, double param2=100, int minRadius=0, int maxRadius=0 )

	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	/// Show your results
	namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
	imshow(outfile, src);
	imwrite(outfile, src);

	waitKey(0);
	return 0;
}


int detect_lines(const char* infile, const char* outfile) {
	Mat src = imread(infile, 0);

	if (src.empty()) {
		cout << "can not open " << infile << endl;
		return -1;
	}

	Mat dst, cdst;
	Canny(src, dst, 50, 200, 3);
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


