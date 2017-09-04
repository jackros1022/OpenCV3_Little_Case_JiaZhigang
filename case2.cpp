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
	src = imread("case/2.jpg", IMREAD_GRAYSCALE);
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	//namedWindow(output_lines, WINDOW_KEEPRATIO);
	Rect roi = Rect(10, 10, src.cols - 20, src.rows - 20);	//手动切除边缘部分
	roiImage = src(roi);
	//createTrackbar("threshold:", output_lines, &threshold_value, max_count, detectLines);
	//detectLines(0, 0);
	morhpologyLines(0, 0);

	waitKey(0);
	return 0;
}

void detectLines(int, void*) {
	Canny(roiImage, dst, threshold_value, threshold_value * 2, 3, false);
	//threshold(roiImage, dst, 0, 255, THRESH_BINARY | THRESH_OTSU);	//阈值也不可以解决
	vector<Vec4i> lines;	//线点
	HoughLinesP(dst, lines, 1, CV_PI / 180.0, 30, 30.0, 0);
	cvtColor(dst, dst, COLOR_GRAY2BGR);		//GRAY2BGR干什么，为什么？ 绘制线出现颜色
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(dst, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(output_lines, dst);
}

void morhpologyLines(int, void*) {
	// binary image
	Mat binaryImage, morhpImage_open, morhpImage_dilate;
	threshold(roiImage, binaryImage, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);	//二值化取反

	// morphology operation（腐蚀去字体）
	Mat kernel = getStructuringElement(MORPH_RECT, Size(50, 1), Point(-1, -1));		//kernel大小尺寸
	morphologyEx(binaryImage, morhpImage_open, MORPH_OPEN, kernel, Point(-1, -1));

	// dilate image（加强直线）
	kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	dilate(morhpImage_open, morhpImage_dilate, kernel);

	// hough lines
	vector<Vec4i> lines;
	HoughLinesP(morhpImage_dilate, lines, 1, CV_PI / 180.0, 30, 250, 0);
	Mat resultImage = roiImage.clone();
	cvtColor(resultImage, resultImage, COLOR_GRAY2BGR);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(resultImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 4, 8, 0);
	}
	imshow(output_lines, resultImage);
	return;
}