#include<gtkmm-4.0/gtkmm.h>
#include<gtkmm-4.0/gtkmm/application.h>
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

class MainWindow: public Gtk::Window {
	public:
		MainWindow();
		virtual ~MainWindow();
	protected:
		void onFileSelected();
		void onFileDialogResponse(int responseId, Gtk::FileChooserDialog* dialog);
		Gtk::Box buttonBox, entryBox;
		Gtk::Button fileButton, fileButton1;
		Gtk::Entry heightEntry, widthEntry;
		Gtk::Separator s;
};

MainWindow::MainWindow() : fileButton("Choose An Image"), fileButton1("Open Webcam"), buttonBox(), entryBox(Gtk::Orientation::VERTICAL)  {
	set_title("Select An Image To Scan");
	set_child(entryBox);
	set_default_size(640 , 480);

	heightEntry.set_text("Enter Height For Output Image");
	widthEntry.set_text("Enter Width For Output Image");
	heightEntry.set_margin(30);
	widthEntry.set_margin(30);
	fileButton.set_margin(10);
	fileButton1.set_margin(10);
	fileButton.set_hexpand(10);
	fileButton1.set_hexpand(10);
	heightEntry.select_region(0, heightEntry.get_text_length());
	widthEntry.select_region(0, widthEntry.get_text_length());
	entryBox.append(heightEntry);
	entryBox.append(widthEntry);
	entryBox.append(s);
	entryBox.append(buttonBox);
	buttonBox.append(fileButton);
	buttonBox.append(fileButton1);
	fileButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onFileSelected));
	fileButton1.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onFileSelected));
}

void MainWindow::onFileSelected()
{
  auto dialog = new Gtk::FileChooserDialog("Please choose a file",
          Gtk::FileChooser::Action::OPEN);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
    sigc::mem_fun(*this, &MainWindow::onFileDialogResponse), dialog));

  //Add response buttons to the dialog:
  dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  dialog->add_button("_Open", Gtk::ResponseType::OK);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog->add_filter(filter_any);

  //Show the dialog and wait for a user response:
  dialog->show();
}

MainWindow::~MainWindow(){ }

void MainWindow::onFileDialogResponse(int responseId, Gtk::FileChooserDialog* dialog){
	switch(responseId){
		case Gtk::ResponseType::OK:{ 
									   auto fileName = dialog->get_file()->get_path();
									   scanImage(fileName, convertToFloat(heightEntry.get_text()), convertToFloat(widthEntry.get_text()));
									   break;
	}
		case Gtk::ResponseType::CANCEL:{
									   std::cout<<"CANCEL Clicked"<<std::endl;
									   break;
	}
									   default:{
									   std::cout<<"Unexpected Button Clicked. "<<std::endl;
									   break;
	}
}
	delete dialog;
}

int main(int argc, char *argv[]){
	auto app = Gtk::Application::create("Doc Scanner");
	return app->make_window_and_run<MainWindow>(argc, argv);
}
