#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR,histR2;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  double diff;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  cap >> image;
  split (image, planes);
  calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);

  while(1){
    cap >> image;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR2, 1,
             &nbins, &histrange,
             uniform, acummulate);
    diff = compareHist(histR,histR2,CV_COMP_CORREL);
    if(diff<0.995){
		putText(image, "MOVIMENTO DETECTADO!", Point(width/2 - 200, height/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
    	calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             	&nbins, &histrange,
             	uniform, acummulate);
    }
	else
		putText(image, " ", Point(width/2 - 200, height/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
    	calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             	&nbins, &histrange,
             	uniform, acummulate);
    imshow("image", image);
	waitKey(30);
  }
  return 0;
}
