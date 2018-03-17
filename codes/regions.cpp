#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

struct ponto{
	int l;
	int c;
};

int main(int argc, char **argv){
	struct ponto p1, p2;
	Mat image;

	image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
		printf("erro ao abrir a imagem\n");

	printf("tamanho da imagem: %dx%d\n\n", image.rows, image.cols);	
	
	printf("P1:\n linha do ponto P1: ");
	scanf("%d", &p1.l);
	printf(" coluna do ponto P1: ");
	scanf("%d", &p1.c);
	printf("P2:\n linha do ponto P2: ");
	scanf("%d", &p2.l);
	printf(" coluna do ponto P2: ");
	scanf("%d", &p2.c);

	if(((p1.l || p2.l) > image.rows) || ((p1.c || p2.c) > image.cols))
		printf("ponto(s) fora da imagem!\n");
	else{
		if(p1.l <= p2.l && p1.c <= p2.c)
			for(int i = p1.l; i < p2.l; i++)
				for(int j = p1.c; j < p2.c; j++)
					image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
		else if(p1.l <= p2.l && p1.c > p2.c)
			for(int i = p1.l; i < p2.l; i++)
				for(int j = p2.c; j < p1.c; j++)
					image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
		else if(p1.l >= p2.l && p1.c <= p2.c)
			for(int i = p2.l; i < p1.l; i++)
				for(int j = p1.c; j < p2.c; j++)
					image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
		else 
			for(int i = p2.l; i < p1.l; i++)
				for(int j = p2.c; j < p1.c; j++)
					image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
	}
	imshow("janela", image);
	waitKey();
}
