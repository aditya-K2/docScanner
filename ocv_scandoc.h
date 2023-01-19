#include <ctime>
#include <gtkmm-4.0/gtkmm.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/img_hash.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <podofo/podofo.h>

struct Pixels {
  int x;
  int y;
  Pixels(int x, int y);
};

const std::string currentDateTime();
void convertToPDF(std::vector<std::string> inputFiles);
std::string convertIntToString(int n);
float convertToFloat(Glib::ustring a);
void drawCircle(int event, int x, int y, int flags, void *param);
void convertPoints(int n, std::vector<Pixels> pix, cv::Point2f pointArray[]);
std::string scanImage(std::string path, float h, float w);
std::string scanImageToWebcam(float h, float w);
