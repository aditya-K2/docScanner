#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/img_hash.hpp>
#include <iostream>

using namespace std;
using namespace cv;

struct Pixels{
	int x;
	int y;
	Pixels(int x, int y){
		this->x = x;
		this->y = y;
	}
};

vector <Pixels>  pix;

float convertToFloat(Glib::ustring a){
	int b = 0;
	for(int i=0;i<a.length();i++){
		if(int(char(a[i])) <=57 && int(char(a[i])) >= 48){
			b*=10;
			b+= (int(char(a[i])) - 48); 
		}
	}
	return float(b);
}

void drawCircle(int event, int x, int y, int flags, void* param)
{
    if(event==EVENT_LBUTTONDOWN)
    {
		if(pix.size() <= 3){
			Mat &img = *((Mat*)(param)); // 1st cast it back, then deref
			circle(img,Point(x,y),50,Scalar(255,255,255),10);
			imshow("My window", img);
			pix.push_back(Pixels(x,y));
		}
    }
}

void convertPoints(int n, vector<Pixels> pix, Point2f pointArray[]){
	for(int i=0;i<n;i++){ pointArray[i] = {float(pix[i].x), float(pix[i].y)}; }
}

void scanImage(string path, float h, float w){
	Point2f pointArray[4];
	Point2f destinationPoints[4] = {{0.0f, 0.0f}, {w, 0.0f} , {0.0f, h}, {w,h}};
	Mat imgorig = imread(path);
	Mat img = imgorig.clone();
	Mat warpedImage;
 	while(1){
		namedWindow("My window", 1); 
		setMouseCallback("My window", drawCircle, &img); // pass address of img here
		imshow("My window", img);
		waitKey(0);
		if(pix.size()<=3) continue;
		break;
	}
	convertPoints(4, pix, pointArray);
	Mat transformationMatrix = getPerspectiveTransform(pointArray, destinationPoints);
	warpPerspective(imgorig, warpedImage, transformationMatrix, Point(w,h));
	imshow("My Window", warpedImage);
	imwrite("./image.png", warpedImage);
	pix.clear();
}

void scanImageToWebcam(float h, float w){
	cv::VideoCapture cap(-1);
	cv::Mat img1;
	Point2f pointArray[4];
	Point2f destinationPoints[4] = {{0.0f, 0.0f}, {w, 0.0f} , {0.0f, h}, {w,h}};
	Mat warpedImage, imgorig;
	while(1){
		cap.read(img1);
		cv::imshow("vidl", img1);
		char key = (char) cv::waitKey(1);
		imgorig = img1;
		if(key == 32){ 
			Mat img = imgorig.clone();
			namedWindow("My window", 1); 
			setMouseCallback("My window", drawCircle, &img); // pass address of img here
			imshow("My window", img);
			waitKey(0);
			if(pix.size()<=3) continue;
			break;
		} 
	}
	convertPoints(4, pix, pointArray);
	Mat transformationMatrix = getPerspectiveTransform(pointArray, destinationPoints);
	warpPerspective(imgorig, warpedImage, transformationMatrix, Point(w,h));
	imshow("My Window", warpedImage);
	imwrite("./image.png", warpedImage);
	pix.clear();
}

