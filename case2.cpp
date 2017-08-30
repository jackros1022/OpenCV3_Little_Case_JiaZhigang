#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

int max_count = 255;
int threshold_value = 100;
const char* output_lines = "Hough Lines";
Mat src, roiImage, dst;
void detectLines(int, void*);
void morhpologyLines(int, void*);
int main(int argc, char** argv) {
	src = imread("D:/gloomyfish/case2.png", IMREAD_GRAYSCALE);
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input image", CV_WINDOW_AUTOSIZE);
	imshow("input image", src);
	namedWindow(output_lines, CV_WINDOW_AUTOSIZE);
	Rect roi = Rect(10, 10, src.cols - 20, src.rows - 20);
	roiImage = src(roi);
	imshow("ROI image", roiImage);
	// createTrackbar("threshold:", output_lines, &threshold_value, max_count, detectLines);
	// detectLines(0, 0);
	morhpologyLines(0, 0);

	waitKey(0);
	return 0;
}

void detectLines(int, void*) {
	Canny(roiImage, dst, threshold_value, threshold_value * 2, 3, false);
	//threshold(roiImage, dst, 0, 255, THRESH_BINARY | THRESH_OTSU);
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180.0, 30, 30.0, 0);
	cvtColor(dst, dst, COLOR_GRAY2BGR);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(dst, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(output_lines, dst);
}

void morhpologyLines(int, void*) {
	// binary image
	Mat binaryImage, morhpImage;
	threshold(roiImage, binaryImage, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	imshow("binary", binaryImage);

	// morphology operation
	Mat kernel = getStructuringElement(MORPH_RECT, Size(20, 1), Point(-1, -1));
	morphologyEx(binaryImage, morhpImage, MORPH_OPEN, kernel, Point(-1, -1));
	imshow("morphology result", morhpImage);

	// dilate image
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(morhpImage, morhpImage, kernel);
	imshow("morphology lines", morhpImage);

	// hough lines
	vector<Vec4i> lines;
	HoughLinesP(morhpImage, lines, 1, CV_PI / 180.0, 30, 20.0, 0);
	Mat resultImage = roiImage.clone();
	cvtColor(resultImage, resultImage, COLOR_GRAY2BGR);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(resultImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(output_lines, resultImage);
	return;
}