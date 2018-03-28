#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histG, histG_prev;
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
  Mat hist_img(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat hist_img_prev(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;
    split (image, planes);

    calcHist(&planes[0], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    
    normalize(histG, histG, 0, hist_img.rows, NORM_MINMAX, -1, Mat());

    hist_img.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
    	line(hist_img,
        	Point(i, histh),
            Point(i, histh-cvRound(histR.at<float>(i))),
            Scalar(0, 0, 255), 1, 8, 0);
    }
    hist_img.copyTo(image(Rect(0, 0       ,nbins, histh)));
    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
