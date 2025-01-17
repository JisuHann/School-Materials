#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <math.h>       /* exp */
#define IM_TYPE	CV_8UC3

using namespace cv;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat laplacianfilter(const Mat input);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat output;


	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);
	output = laplacianfilter(input);

	namedWindow("Laplacian Filter 4��", WINDOW_AUTOSIZE);
	imshow("Laplacian Filter 4��", output);


	waitKey(0);

	return 0;
}


Mat laplacianfilter(const Mat input) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int n = 1; // Sobel Filter Kernel N

	// Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
	//Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)
	Mat input2;
	cvtColor(input, input2, CV_RGB2GRAY);
	Mat output = Mat::zeros(row, col, input2.type());
	int lap_filter[3][3] = { {0,1,0},{1,-4,1},{0,1,0} };

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			float sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
			for (int a = -n; a <= n; a++) {
				for (int b = -n; b <= n; b++) {
					// Use mirroring boundary process
					// Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy ) 

					int tempa, tempb;

					if (i + a > row - 1) {  //mirroring for the border pixels
						tempa = i - a;
					}
					else if (i + a < 0) {
						tempa = -(i + a);
					}
					else {
						tempa = i + a;
					}
					if (j + b > col - 1) {
						tempb = j - b;
					}
					else if (j + b < 0) {
						tempb = -(j + b);
					}
					else {
						tempb = j + b;
					}
					sum_r += (float)lap_filter[n + a][n + b] * (float)(input.at<C>(tempa, tempb)[0]);
					sum_g += (float)lap_filter[n + a][n + b] * (float)(input.at<C>(tempa, tempb)[1]);
					sum_b += (float)lap_filter[n + a][n + b] * (float)(input.at<C>(tempa, tempb)[2]);
				}

			}
			if (sum_r < 0)sum_r = -sum_r;
			if (sum_r > 255) sum_r = 255;

			if (sum_g < 0)sum_g = -sum_g;
			if (sum_g > 255) sum_g = 255;

			if (sum_b < 0)sum_b = -sum_b;
			if (sum_b > 255) sum_b = 255;
			output.at<G>(i, j) = (G)((sum_r+sum_g+sum_b)/3)*4;
		}
	}
	return output;
}