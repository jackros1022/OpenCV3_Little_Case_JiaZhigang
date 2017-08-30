#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat src, gray_src, dst;
int threshold_value = 100;
int max_level = 255;
const char* output_win = "Contours Result";
const char* roi_win = "Final Result";
void FindROI(int, void*);
void Check_Skew(int, void*);
int main(int argc, char** argv) {
	src = imread("D:/gloomyfish/case1r.png");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input image", CV_WINDOW_AUTOSIZE);
	imshow("input image", src);
	namedWindow(output_win, CV_WINDOW_AUTOSIZE);
	Check_Skew(0, 0);
	// namedWindow(roi_win, CV_WINDOW_AUTOSIZE);
	//createTrackbar("Threshold:", output_win, &threshold_value, max_level, FindROI);
	// FindROI(0, 0);

	waitKey(0);
	return 0;
}

void Check_Skew(int, void*) {
	Mat canny_output;
	cvtColor(src, gray_src, COLOR_BGR2GRAY);
	Canny(gray_src, canny_output, threshold_value, threshold_value * 2, 3, false);

	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(canny_output, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawImg = Mat::zeros(src.size(), CV_8UC3);
	float maxw = 0;
	float maxh = 0;
	double degree = 0;
	for (size_t t = 0; t < contours.size(); t++) {
		RotatedRect minRect = minAreaRect(contours[t]);
		degree = abs(minRect.angle);
		if (degree > 0) {
			maxw = max(maxw, minRect.size.width);
			maxh = max(maxh, minRect.size.height);
		}
	}
	RNG rng(12345);
	for (size_t t = 0; t < contours.size(); t++) {
		RotatedRect minRect = minAreaRect(contours[t]);
		if (maxw == minRect.size.width && maxh == minRect.size.height) {
			degree = minRect.angle;
			Point2f pts[4];
			minRect.points(pts);
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			for (int i = 0; i < 4; i++) {
				line(drawImg, pts[i], pts[(i + 1) % 4], color, 2, 8, 0);
			}
		}
	}
	printf("max contours width : %f\n", maxw);
	printf("max contours height : %f\n", maxh);
	printf("max contours angle : %f\n", degree);
	imshow(output_win, drawImg);

	Point2f center(src.cols / 2, src.rows / 2);
	Mat rotm = getRotationMatrix2D(center, degree, 1.0);
	Mat dst;
	warpAffine(src, dst, rotm, src.size(), INTER_LINEAR, 0, Scalar(255, 255, 255));
	imshow("Correct Image", dst);
}

void FindROI(int, void*) {
	cvtColor(src, gray_src, COLOR_BGR2GRAY);
	Mat canny_output;
	Canny(gray_src, canny_output, threshold_value, threshold_value * 2, 3, false);

	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(canny_output, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	int minw = src.cols*0.75;
	int minh = src.rows*0.75;
	RNG rng(12345);
	Mat drawImage = Mat::zeros(src.size(), CV_8UC3);
	Rect bbox;
	for (size_t t = 0; t < contours.size(); t++) {
		RotatedRect minRect = minAreaRect(contours[t]);
		float degree = abs(minRect.angle);
		if (minRect.size.width > minw && minRect.size.height > minh && minRect.size.width < (src.cols-5)) {
			printf("current angle : %f\n", degree);
			Point2f pts[4];
			minRect.points(pts);
			bbox = minRect.boundingRect();
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			for (int i = 0; i < 4; i++) {
				line(drawImage, pts[i], pts[(i + 1)%4], color, 2, 8, 0);
			}
		}
	}
	imshow(output_win, drawImage);

	if (bbox.width > 0 && bbox.height > 0) {
		Mat roiImg = src(bbox);
		imshow(roi_win, roiImg);
	}
	return;
}
