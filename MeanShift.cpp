#include <iostream>
#include <vector>
#include <ctime>

#include "opencv342.h"

using namespace std;

cv::Mat MeanShift_Cluster(cv::Mat);

static void floodFillPostprocess(cv::Mat& img, const cv::Scalar& colorDiff = cv::Scalar::all(1)) {
	CV_Assert(!img.empty());
	cv::RNG rng = cv::theRNG();
	cv::Mat mask(img.rows + 2, img.cols + 2, CV_8UC1, cv::Scalar::all(0));

	/////////////////////////
	// 추가한 부분
	cv::Mat _mask = mask.clone(), diff = mask.clone();
	cv::Mat cpy = img.clone();
	float val1, val2, val3;
	int nPoints, iTemp, i, j;
	////////////////////////

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			if (mask.at<uchar>(y + 1, x + 1) == 0) {
				mask.copyTo(_mask); // 추가한 부분
				cv::Scalar newVal(rng(256), rng(256), rng(256));
				// img 대신 cpy 로 변경
				cv::floodFill(cpy, mask, cv::Point(x, y), newVal, 0, colorDiff, colorDiff);
				diff = mask - _mask; // 추가한 부분

				/////////////////////////////
				// 추가한 부분
				nPoints = 0;
				val1 = val2 = val3 = 0.0;
				for (i = 0; i < img.rows; i++) {
					for (j = 0; j < img.cols; j++) {
						if (diff.at<uchar>(i + 1, j + 1)) {
							val1 += img.at<cv::Vec3b>(i, j)[0];
							val2 += img.at<cv::Vec3b>(i, j)[1];
							val3 += img.at<cv::Vec3b>(i, j)[2];
							nPoints++;
						}
					}
				}
				for (i = 0; i < img.rows; i++) {
					for (j = 0; j < img.cols; j++) {
						if (diff.at<uchar>(i + 1, j + 1)) {
							iTemp = cvRound(val1 / (float)nPoints);
							iTemp = iTemp > 255 ? 255 : iTemp < 0 ? 0 : iTemp;
							img.at<cv::Vec3b>(i, j)[0] = (uchar)iTemp;

							iTemp = cvRound(val2 / (float)nPoints);
							iTemp = iTemp > 255 ? 255 : iTemp < 0 ? 0 : iTemp;
							img.at<cv::Vec3b>(i, j)[1] = (uchar)iTemp;

							iTemp = cvRound(val3 / (float)nPoints);
							iTemp = iTemp > 255 ? 255 : iTemp < 0 ? 0 : iTemp;
							img.at<cv::Vec3b>(i, j)[2] = (uchar)iTemp;
						}
					}
				}
			}
		}
	}
}

int main() {
	string CLUSTER[5] = { "12003.jpg", "25098.jpg", "97017.jpg", "151087.jpg", "181018.jpg" };
	string path = "testImage/";

	// Mean Shift Clustering
	cout << "Start MeanShift Clustering." << endl;
	for (int i = 0; i < 5; i++) {
		cv::Mat img = cv::imread(path + CLUSTER[i]);
		cv::Mat result;
		result = MeanShift_Cluster(img);
		cv::imwrite(path + "Result_MeanShift/" + CLUSTER[i], result);
	}
	cout << "End MeanShift Clustering." << endl;
}

cv::Mat MeanShift_Cluster(cv::Mat src) {
	cv::Mat result;

	double sp, sr;

	sp = 10; // the spatial window radius
	sr = 10; // the color window radius
	cv::pyrMeanShiftFiltering(src, result, sp, sr);
	floodFillPostprocess(result, cv::Scalar::all(2));

	return result;
}
