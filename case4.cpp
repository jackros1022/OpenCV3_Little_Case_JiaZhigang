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

	// ��ֵ�ָ�
		// threshold���Բ�����
		//1.	THRESH_OTSU (Ч�������룬˫���Ч���Ϻ�)
		//2.	THRESH_TRIANGLE�����壩
	threshold(gray_src, binary, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);

	// ��̬ѧ����
		// ��ʹ����һ�������Ե���ʹ��dilate����@param iterations��3��6��9��12��13
		// 
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(binary, binary, kernel, Point(-1, -1), 1);
	imshow("dilate image", binary);

	// ����任
		//���ã�
	Mat dist;
	bitwise_not(binary, binary);	// ȡ��
	distanceTransform(binary, dist, CV_DIST_L2, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);		//���������أ�
	imshow("dist image", dist);

	// ��ֵ����ֵ�ָ�
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);	//ת��Ϊ8U
	adaptiveThreshold(dist_8u, dist_8u, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 85, 0.0);
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(dist_8u, dist_8u, kernel, Point(-1, -1), 2);
	imshow("dist-binary", dist_8u);

	// ��ͨ�������
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