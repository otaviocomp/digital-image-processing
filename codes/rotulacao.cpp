#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv){
	Mat image;
	int obj=0,bolhas=0;
	CvPoint p;
	image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data){
		printf("erro ao carregar a imagem!\n");
		return(-1);
	}

	// pré processamento para retirar os objetos das bordas
	for(int i=0; i<image.rows; i++){
		if(image.at<uchar>(i, 0) == 255){
			p.x=0;
			p.y=i;
			floodFill(image,p,0);
		}
		if(image.at<uchar>(i,image.cols - 1) == 255){
			p.x=image.cols - 1;
			p.y=i;
			floodFill(image,p,0);
		}
	}
	for(int i=0; i<image.cols; i++){
		if(image.at<uchar>(0,i) == 255){
			p.x=i;
			p.y=0;
			floodFill(image,p,0);
		}
		if(image.at<uchar>(image.rows - 1, i) == 255){
			p.x=i;
			p.y=image.rows - 1;
			floodFill(image,p,0);
		}
	}

	// muda a cor de fundo
	p.x=0;
	p.y=0;
	floodFill(image,p,254);

	// rotulação e contagem 
	for(int i=0; i<image.rows; i++)
		for(int j=0; j<image.cols; j++){
			if(image.at<uchar>(i, j) == 255){
				p.x=j;
				p.y=i;
				floodFill(image,p,++obj);
			}
			if(image.at<uchar>(i,j) == 0){
				bolhas++;
				p.x=j;
				p.y=i;
				floodFill(image,p,128);
			}
		}
	imshow("resultado",image);
	imwrite("../images/resultado3.png",image);
	printf("numero de objetos:%d\nnumero de bolhas:%d\n",obj,bolhas);	
}
