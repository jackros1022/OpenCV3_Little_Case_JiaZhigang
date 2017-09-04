#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat gray_src, binary, dst;
	Mat src = imread("case/4-2.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input image", CV_WINDOW_AUTOSIZE);
	imshow("input image", src);
	cvtColor(src, gray_src, COLOR_BGR2GRAY);

	// 二值分割
		// threshold尝试参数：
		//1.	THRESH_OTSU (效果不理想，双峰的效果较好)
		//2.	THRESH_TRIANGLE（单峰）
	threshold(gray_src, binary, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);

	// 形态学操作
		// 单使用这一步，尝试调整使用dilate参数@param iterations：3、6、9、12、13
		// 
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(binary, binary, kernel, Point(-1, -1), 1);
	imshow("dilate image", binary);

	// 距离变换
		//作用：
	Mat dist;
	bitwise_not(binary, binary);	// 取反
	distanceTransform(binary, dist, CV_DIST_L2, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);		//若不采用呢？
	imshow("dist image", dist);

	// 阈值化二值分割
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);	//转换为8U
	adaptiveThreshold(dist_8u, dist_8u, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 85, 0.0);
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(dist_8u, dist_8u, kernel, Point(-1, -1), 2);
	imshow("dist-binary", dist_8u);

	// 连通区域计数
	vector<vector<Point>> contours;
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// draw result
	Mat markers = Mat::zeros(src.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t t = 0; t < contours.size(); t++) {
		drawContours(markers, contours, static_cast<int>(t), 
			Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)),
			-1, 8, Mat());
	}
	printf("number of corns : %d", contours.size());
	imshow("Final result", markers);

	waitKey(0);
	return 0;
}