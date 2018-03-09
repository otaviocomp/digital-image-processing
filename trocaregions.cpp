#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv)
{
	Mat image, altimage;
	image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	altimage = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
		printf("erro ao abrir a image\n");
	for(int i = 0; i < image.rows/2; i++)
		for(int j = 0; j < image.cols/2; j++)
		{
			altimage.at<uchar>(i, j) = image.at<uchar>(image.rows/2 + i, image.cols/2 + j);
			altimage.at<uchar>(i, image.cols/2 + j) = image.at<uchar>(image.rows/2 + i, j);
			altimage.at<uchar>(image.rows/2 + i, j) = image.at<uchar>(i, image.cols/2 + j);
			altimage.at<uchar>(image.rows/2 + i, image.cols/2 + j) = image.at<uchar>(i, j);
		}
	imshow("imagem trocada", altimage);
	waitKey();
}
