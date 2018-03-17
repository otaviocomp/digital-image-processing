#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, gray_image, equalize_image, histGr, histEq;
  int width, height;
  VideoCapture cap;
  int nbins = 64;
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
  Mat histImgGr(histh, histw, CV_8UC1, Scalar(0,0,0));
  Mat histImgEq(histh, histw, CV_8UC1, Scalar(0,0,0));

  while(1){
    cap >> image;
	cvtColor(image,gray_image,CV_BGR2GRAY);
	equalizeHist(gray_image,equalize_image);

    calcHist(&gray_image, 1, 0, Mat(), histGr, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&equalize_image, 1, 0, Mat(), histEq, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histGr, histGr, 0, histImgGr.rows, NORM_MINMAX, -1, Mat());
    normalize(histEq, histEq, 0, histImgEq.rows, NORM_MINMAX, -1, Mat());

    histImgGr.setTo(Scalar(0));
    histImgEq.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgGr,
           Point(i, histh),
           Point(i, histh-cvRound(histGr.at<float>(i))),
           Scalar(255), 1, 8, 0);
      line(histImgEq,
           Point(i, histh),
           Point(i, histh-cvRound(histEq.at<float>(i))),
           Scalar(255), 1, 8, 0);
    }
    histImgGr.copyTo(gray_image(Rect(0, 0       ,nbins, histh)));
    histImgEq.copyTo(equalize_image(Rect(0, histh   ,nbins, histh)));

    imshow("image", image);
	imwrite("../images/resultado4.png", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
