#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#define RADIUS 20

using namespace cv;
using namespace std;

#define MAX 100.0

int height;

int dft_M, dft_N;

float gamal = 0;
float max_gamal = 100;
int gamal_slider = 0;

int d0_slider = 0;
float max_d0 = 256;
float d0 = 0;

float gamah = 0;
float max_gamah = 100;
int gamah_slider = 0;


Mat image, imageDft, padded;

char TrackbarName[50];

// troca os quadrantes da imagem da DFT como havia sido feito na 1 unidade
void deslocaDFT(Mat& image ){
    Mat aux, A, B, C, D;

    // se impar recorta para nao ter imagens desiguais
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols/2;
    int cy = image.rows/2;

    // reorganiza os quadrantes da transformada
    A = image(Rect(0, 0, cx, cy));
    B = image(Rect(cx, 0, cx, cy));
    C = image(Rect(0, cy, cx, cy));
    D = image(Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(aux);  D.copyTo(A);  aux.copyTo(D);

    // C <-> B
    C.copyTo(aux);  B.copyTo(C);  aux.copyTo(B);
}

void homomorfico() {
    Mat filter = Mat(padded.size(), CV_32FC2, Scalar(0));
    Mat aux = Mat(dft_M, dft_N, CV_32F);

    for (int i = 0; i < dft_M; i++) {
        for (int j = 0; j < dft_N; j++) {
            float d2 = pow(i - dft_M/2.0, 2) + pow(j - dft_N/2.0, 2);
            float valor = (gamah - gamal)*(1 - expf(- (d2/pow(d0, 2))) ) + gamal; //
            aux.at<float> (i,j) = valor;
        }
    }

    Mat comps[] = {aux, aux};
    merge(comps, 2, filter);

    Mat dftClone = imageDft.clone();

    mulSpectrums(dftClone,filter,dftClone,0);

    deslocaDFT(dftClone);

    idft(dftClone, dftClone);

    vector<Mat> planos;

    split (dftClone, planos);

    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

    char bufferd0[20], bufferyh[20], bufferyl[20];
    sprintf(bufferd0, "D0: %f", d0);
    sprintf(bufferyh, "Alta Frequencia: %f", gamah);
    sprintf(bufferyl, "Baixa Frequencia: %f", gamal);
    putText(planos[0], bufferd0,cv::Point(0,10), CV_FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(255),1,8,false);
    putText(planos[0], bufferyh,cv::Point(0,20), CV_FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(255),1,8,false);
    putText(planos[0], bufferyl,cv::Point(0,30), CV_FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(255),1,8,false);
    imshow("Filtro Homomorfico", planos[0]);
    imshow("Original", image);
}

void on_trackbar_gamal(int, void*){
    gamal = (float) gamal_slider;
    gamal = max_gamal*gamal/MAX;
    homomorfico();
}

void on_trackbar_d0(int, void *){
    d0 = d0_slider*max_d0/MAX;
    homomorfico();
}

void on_trackbar_gamah(int, void*) {
    gamah = gamah_slider*max_gamah/MAX;
    homomorfico();
}

int main(int argvc, char** argv){
    image = imread("mi.jpg");
    cvtColor(image, image, CV_BGR2GRAY);

    height = image.size().height;

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    // realiza o padding da imagem
    Mat_<float> zeros;
    copyMakeBorder(image, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    imageDft = Mat(padded.size(), CV_32FC2, Scalar(0));

    copyMakeBorder(image, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    Mat_<float> realInput = Mat_<float>(padded);
    vector<Mat> planos;
    planos.push_back(realInput);
    planos.push_back(zeros);
    merge(planos, imageDft);
    dft(imageDft, imageDft);
    deslocaDFT(imageDft);
    namedWindow("Filtro Homomorfico", 1);

    sprintf( TrackbarName, "Alta Frequencia");
    createTrackbar( TrackbarName, "Filtro Homomorfico",
                    &gamah_slider,
                    MAX,
                    on_trackbar_gamah);

    sprintf( TrackbarName, "Baixa Frequencia");
    createTrackbar( TrackbarName, "Filtro Homomorfico",
                    &gamal_slider,
                    MAX,
                    on_trackbar_gamal);

    sprintf( TrackbarName, "D0");
    createTrackbar( TrackbarName, "Filtro Homomorfico",
                    &d0_slider,
                    MAX,
                    on_trackbar_d0 );
    homomorfico();
    waitKey(0);

    return 0;
}


