#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histR_ant;
  int nbins = 64;
  double d;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

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
  Mat histImgR_ant(histh, histw, CV_8UC3, Scalar(0,0,0));
  bool primeiro=true;

  while(1){
    cap >> image;
    split (image, planes);

    if(!primeiro){
    	histR.copyTo(histR_ant);
    	histImgR.copyTo(histImgR_ant);
    }

    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);

    }
    histImgR.copyTo(image(Rect(0, 0,nbins, histh))); //o de cima é o da imagem atual
    if (!primeiro)
    	histImgR_ant.copyTo(image(Rect(0, histh,nbins, histh)));

    if (!primeiro){
    	d=compareHist(histR,histR_ant,CV_COMP_CORREL);
    	cout << d << endl;
    	if (d<0.995){
			printf("movimento detectado!!!\n");
			imwrite("Detectado Movimento.png",image);
    	    imshow("Detectado Movimento",image);
    	}

    }
    primeiro=false;
    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
