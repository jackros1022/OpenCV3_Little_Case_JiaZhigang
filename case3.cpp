#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat src, binary, dst;
int main(int argc, char** argv) {
	src = imread("D:/gloomyfish/case3.png", IMREAD_GRAYSCALE);
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input image", CV_WINDOW_AUTOSIZE);
	imshow("input image", src);

	// 二值化
	threshold(src, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("binary image", binary);

	// 形态学操作
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, dst, MORPH_CLOSE, kernel, Point(-1, -1));
	imshow("close image", dst);

	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(dst, dst, MORPH_OPEN, kernel, Point(-1, -1));
	imshow("open image", dst);

	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(dst, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());

	Mat resultImage = Mat::zeros(src.size(), CV_8UC3);
	Point cc;
	for (size_t t = 0; t < contours.size(); t++) {
		// 面积过滤
		double area = contourArea(contours[t]);
		if (area < 100) continue;
		// 横纵比过滤
		Rect rect = boundingRect(contours[t]);
		float ratio = float(rect.width) / float(rect.height);
		
		if (ratio < 1.1 && ratio > 0.9) {
			drawContours(resultImage, contours, t, Scalar(0, 0, 255), -1, 8, Mat(), 0, Point());
			printf("circle area : %f\n", area);
			printf("circle length : %f\n", arcLength(contours[t], true));
			int x = rect.x + rect.width / 2;
			int y = rect.y + rect.height / 2;
			cc = Point(x, y);
			circle(resultImage, cc, 2, Scalar(0, 0, 255), 2, 8, 0);
		}
	}
	imshow("Result", resultImage);

	Mat circleImage = src.clone();
	cvtColor(circleImage, circleImage, COLOR_GRAY2BGR);
	circle(circleImage, cc, 2, Scalar(0, 0, 255), 2, 8, 0);
	imshow("Final Result", circleImage);

	// detect circle
	/*vector<Vec3f> myCircles;
	Mat gray_result;
	cvtColor(resultImage, gray_result, COLOR_BGR2GRAY);
	HoughCircles(gray_result, myCircles, HOUGH_GRADIENT, 1, 7, 50, 20, 23, 100);

	Mat circleImage = src.clone();
	cvtColor(circleImage, circleImage, COLOR_GRAY2BGR);
	for (int i = 0; i < myCircles.size(); i++) {
		Vec3f circleInfo = myCircles[i];
		circle(circleImage, Point(circleInfo[0], circleInfo[1]), circleInfo[2], Scalar(0, 0, 255), 1, 8, 0);
	}
	
	*/
	waitKey(0);
	return 0;
}
