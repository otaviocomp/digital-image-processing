#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv)
{
	Mat image;
	int num_obj = 0, num_obj_bolhas = 0, temp1, temp2 = 0;
	bool aux = 0;
	CvPoint p;
	image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
	{
		printf("erro ao carregar a imagem!\n");
		return(-1);
	}

	// pré processamento para retirar as bolhas das bordas
	for(int i = 0; i < image.rows; i++)
	{
		if(image.at<uchar>(i, 0) == 255)
		{
			p.x = 0;
			p.y = i;
			floodFill(image, p, 0);
		}
		if(image.at<uchar>(i, image.cols - 1) == 255)
		{
			p.x = image.cols - 1;
			p.y = i;
			floodFill(image, p, 0);
		}
	}
	for(int i = 0; i < image.cols; i++)
	{
		if(image.at<uchar>(0, i) == 255)
		{
			p.x = i;
			p.y = 0;
			floodFill(image, p, 0);
		}
		if(image.at<uchar>(image.rows - 1, i) == 255)
		{
			p.x = i;
			p.y = image.rows - 1;
			floodFill(image, p, 0);
		}
	}

	// rotulação e contagem de bolhas
	for(int i = 0; i < image.rows; i++)
	{
		for(int j = 0; j < image.cols; j++)
		{
			if(image.at<uchar>(i, j) == 255)
			{
				p.x = j;
				p.y = i;
				floodFill(image, p, ++num_obj);
			}
			temp1 = image.at<uchar>(i, j);
			if(temp1 != 0)
				temp2 = temp1;
			if(temp1 == 0 && temp2 != 0)
				aux = 1;
			if(temp1 - temp2 == 1)
			{
				temp2 = 0;
				aux = 0;
			}
			if(temp1 != 0 && temp2 == temp1 && aux == 1)
			{
				num_obj_bolhas++;
				p.x = j;
				p.y = i;
				floodFill(image, p, 0);
				aux = 0;
				temp2 = 0;
			}
		}
	}

	printf("numero de bolhas:%d\nnumero de bolhas furadas:%d\n", num_obj, num_obj_bolhas);	
	imshow("image", image);
	waitKey();
}
		
