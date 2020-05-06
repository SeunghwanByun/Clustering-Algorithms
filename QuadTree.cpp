#include <iostream>
#include <vector>
#include <ctime>

#include "QuadTree.h"
#include "opencv.h"

using namespace std;

cv::Mat QuadTree_Cluster(cv::Mat, int minArea, float threshold);

int main()
{
	// Input Image의 파일명
	string CLUSTER[5] = { "12003.jpg", "25098.jpg", "97017.jpg", "151087.jpg", "181018.jpg" };
	string path = "testImage/";

	// QuadTree Clustering
	for (int i = 0; i < 5; i++) {
		cv::Mat img = cv::imread(path + CLUSTER[i]);
		cv::Mat result;
		int minArea = 10;
		float threshold = 5.0;

		result = QuadTree_Cluster(img, minArea, threshold); // src, minArea, threshold
		cv::imwrite(path + "Result_Quad/" + CLUSTER[i], result);
	}


	return 0;
}

cv::Mat QuadTree_Cluster(cv::Mat src, int minArea, float threshold) {
	cv::Mat result;

	QuadTree clustering(src, minArea, threshold);

	result = clustering.getResultMat();

	return result;
}
