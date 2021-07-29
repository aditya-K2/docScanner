#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/img_hash.hpp>
#include <iostream>
#include <ctime>
#include"podofo/podofo.h"

using namespace std;
using namespace cv;

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y.%m.%d.%I.%M.%S", &tstruct);
    return buf;
}

void convertToPDF(std::vector<std::string>inputFiles) {
		PoDoFo::PdfMemDocument document;
		PoDoFo::PdfPainter painter;
		PoDoFo::PdfPage *page;
		for (auto& imageName: inputFiles) {
			PoDoFo::PdfImage image(&document);
			/* Loading image */
			image.LoadFromFile(imageName.c_str());
			/* Setting default size of image in pdf */
			PoDoFo::PdfRect size = PoDoFo::PdfRect(0.0,0.0,image.GetWidth(),
                                                                                image.GetHeight());
			/* Creating a new page in our document */
			page = document.CreatePage(size);
			painter.SetPage(page);


			double dScaleX = size.GetWidth()/image.GetWidth();
			double dScaleY = size.GetHeight()/image.GetHeight();
			double dScale = PoDoFo::PDF_MIN(dScaleX,dScaleY);

			if(dScale < 1.0)
				painter.DrawImage(0.0,0.0,&image,dScale,dScale);
			else {
				double dX = (size.GetWidth() - image.GetWidth())/2.0;
				double dY = (size.GetHeight() - image.GetHeight())/2.0;
				painter.DrawImage(dX,dY,&image);
			}
		}

		std::string outFileName = currentDateTime() + ".pdf";
		painter.FinishPage();
		fprintf(stdout,"Writing to file %s \n",outFileName.c_str()); 
		document.Write(outFileName.c_str()); //Writing the file 
		cout<<"Done";
}

vector <string>  imagePaths;

struct Pixels{
	int x;
	int y;
	Pixels(int x, int y){
		this->x = x;
		this->y = y;
	}
};

vector <Pixels>  pix;

string convertIntToString(int n){
	string a[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	string result = "";
	while(n){
		result += a[n%10];
		n = n/10;
	}
	string result1 = "";
	for(int i=result.length();i>=0;i--){ result1+=result[i]; } 
	return result1;
}



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
			circle(img,Point(x,y),10,Scalar(0,0,0),10, FILLED);
			imshow("My window", img);
			pix.push_back(Pixels(x,y));
		}
    }
}

void convertPoints(int n, vector<Pixels> pix, Point2f pointArray[]){
	for(int i=0;i<n;i++){ pointArray[i] = {float(pix[i].x), float(pix[i].y)}; }
}

string scanImage(string path, float h, float w){
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
	string pathName = currentDateTime() + ".png";
	string TpathName = "thumbnails/T" + pathName;
	Mat resizeImage;
	resize(warpedImage,resizeImage, Size(600, 600 ));
	imwrite(pathName , warpedImage);
	imwrite(TpathName , resizeImage);
	imagePaths.push_back(pathName);
	pix.clear();
	return TpathName;
}

string scanImageToWebcam(float h, float w){
	cv::VideoCapture cap(1);
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
	string pathName = currentDateTime() + ".png";
	string TpathName = "thumbnails/T" + pathName;
	Mat resizeImage;
	resize(warpedImage,resizeImage, Size(600, 600));
	imwrite(pathName, warpedImage);
	imwrite(TpathName, resizeImage);
	imagePaths.push_back(pathName);
	pix.clear();
	return TpathName;
}

