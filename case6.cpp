#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat src = imread("D:/gloomyfish/case6.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input image", CV_WINDOW_AUTOSIZE);
	imshow("input image", src);

	Mat blurImage;
	GaussianBlur(src, blurImage, Size(15, 15), 0, 0);
	imshow("blur", blurImage);

	Mat gray_src, binary;
	cvtColor(blurImage, gray_src, COLOR_BGR2GRAY);
	threshold(gray_src, binary, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);
	imshow("binary", binary);

	// 形态学操作
	Mat morphImage;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, morphImage, MORPH_CLOSE, kernel, Point(-1, -1), 2);
	imshow("morphology", morphImage);

	// 获取最大轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(morphImage, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	Mat connImage = Mat::zeros(src.size(), CV_8UC3);
	for (size_t t = 0; t < contours.size(); t++) {
		Rect rect = boundingRect(contours[t]);
		if (rect.width < src.cols / 2) continue;
		if (rect.width > (src.cols - 20)) continue;
		double area = contourArea(contours[t]);
		double len = arcLength(contours[t], true);
		drawContours(connImage, contours, static_cast<int>(t), Scalar(0, 0, 255), 1, 8, hireachy);
		printf("area  of star could : %f\n", area);
		printf("length  of star could : %f\n", len);
	}
	imshow("result", connImage);

	waitKey(0);
	return 0;
}