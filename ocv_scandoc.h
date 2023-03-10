#include <ctime>
#include <gtkmm-4.0/gtkmm.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/img_hash.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <podofo/podofo.h>
#include <sys/stat.h>
#include <unistd.h>

struct Pixels {
  int x;
  int y;
  Pixels(int x, int y);
};

const std::string current_date_time();
void convert_to_pdf(std::vector<std::string> inputFiles);
std::string int_to_str(int n);
float str_to_float(Glib::ustring a);
void draw_circle(int event, int x, int y, int flags, void *param);
void convert_points(int n, std::vector<Pixels> pix, cv::Point2f pointArray[]);
std::string scan_image(std::string path, float h, float w);
std::string scan_image_webcam(float h, float w);
