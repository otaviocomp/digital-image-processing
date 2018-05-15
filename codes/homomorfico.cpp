#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

int yL, yLmax=100;
int yH, yHmax=100;
int D0, D0max=100;
int C0, C0max=100;

Mat complexImage;
Mat padded, filter,result,filtrada;
Mat image, imagegray, temp;
Mat_<float> realInput, zeros;
vector<Mat> planos;

int dft_M, dft_N;
char TrackbarName[50];
char key;

void on_trackbar_filtro(int, void*){
    int dft_M= temp.size().height;
    int dft_N= temp.size().width;

    double gH= yH/10.0;
    double gL= yL/10.0;
    double d0= D0/10.0;
    // variacao entre 0 e 0.1
    double c0= C0/1000.0;

  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
    	double D= pow(i-dft_M/2,2)+pow(j-dft_N/2, 2);
      temp.at<float> (i,j) = (gH-gL)*(1-exp(-c0*D/pow(d0,2)))+gL;
      }
   }

    // junta o plano real e o imaginario
    Mat comps[]= {temp, temp};
    merge(comps, 2, filter);
}

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat temp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(temp);  D.copyTo(A);  temp.copyTo(D);

  // C <-> B
  C.copyTo(temp);  B.copyTo(C);  temp.copyTo(B);
}

int main(int argc, char** argv){
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  namedWindow("com filtro", CV_WINDOW_AUTOSIZE);

    sprintf( TrackbarName, "yH x %d", yHmax );
    createTrackbar( TrackbarName, "com filtro",&yH,
    						 yHmax,
    						 on_trackbar_filtro);

    sprintf( TrackbarName, "yL x %d", yLmax );
    createTrackbar( TrackbarName, "com filtro",&yL,
    						 yLmax,
    						 on_trackbar_filtro);

    sprintf( TrackbarName, "D0 x %d", D0max );
    createTrackbar( TrackbarName, "com filtro",&D0,
    						 D0max,
    						 on_trackbar_filtro);

    sprintf( TrackbarName, "C0 x %d", D0max );
    createTrackbar( TrackbarName, "com filtro",&C0,
    						 C0max,
    						 on_trackbar_filtro);


  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  temp = Mat(dft_M, dft_N, CV_32F);

  while(1){
    imagegray= image.clone();

    imshow("original", imagegray);

    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));
    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);
    realInput += Scalar::all(1);
    // calcula o log da imagem
    log(realInput, realInput);
    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);
    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    idft(complexImage, complexImage,DFT_SCALE);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);
    // calcular expodencial
    exp(planos[0], planos[0]);

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("com filtro", planos[0]);
    planos[0].convertTo(filtrada, CV_8UC1, 255.0);

    vector<Mat> pFiltro;
    split(filter,pFiltro);
    normalize(pFiltro[0], pFiltro[0], 0, 1, CV_MINMAX);
    // converte para uchar para salvar
    pFiltro[0].convertTo(result, CV_8UC1, 255.0);
    imshow("filtro",result);

    key = (char) waitKey(10);
    if( key == 27 ) break; 
    }
  return 0;
}
