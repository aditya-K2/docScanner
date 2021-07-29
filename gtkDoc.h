#include<gtkmm-4.0/gtkmm.h>
#include<gtkmm-4.0/gtkmm/application.h>
#include"oCV.h"

using namespace std;

vector <Gtk::Image *> imageWidgets;
class MainWindow: public Gtk::Window {
	public:
		MainWindow();
		virtual ~MainWindow();
	protected:
		void onFileSelected();
		void onFileDialogResponse(int responseId, Gtk::FileChooserDialog* dialog);
		void startWebcam();
		void savePDF();
		void clearData();
		Gtk::Box mainBox, buttonBox, entryBox, imageBox, utilityBox;
		Gtk::Button fileButton, fileButton1, clearButton, saveButton;
		Gtk::Entry heightEntry, widthEntry;
		Gtk::Separator s, se; 
		Gtk::ScrolledWindow imagePreview;
		Gtk::Label imageLabel;
};

MainWindow::MainWindow() : fileButton("Choose An Image"), fileButton1("Open Webcam"), mainBox(Gtk::Orientation::VERTICAL), 
	buttonBox(Gtk::Orientation::HORIZONTAL), entryBox(Gtk::Orientation::VERTICAL), imageBox(Gtk::Orientation::HORIZONTAL), 
    utilityBox(Gtk::Orientation::HORIZONTAL), clearButton("Clear"), saveButton("Save"){
	set_title("Select An Image To Scan");
	set_child(mainBox);
	set_default_size(640 , 480);

	heightEntry.set_text("Enter Height For Output Image");  heightEntry.set_margin(30);  heightEntry.select_region(0, heightEntry.get_text_length());
	widthEntry.set_text("Enter Width For Output Image");  widthEntry.set_margin(30); widthEntry.select_region(0, widthEntry.get_text_length());

	imageLabel.set_text("Current Images in Buffers");
	imageLabel.set_margin(10);
	mainBox.append(entryBox); mainBox.append(buttonBox); mainBox.append(s); mainBox.append(imageLabel); mainBox.append(imagePreview); mainBox.append(se); mainBox.append(utilityBox);
	entryBox.append(heightEntry); entryBox.append(widthEntry);
	buttonBox.append(fileButton); buttonBox.append(fileButton1);
	utilityBox.append(clearButton); utilityBox.append(saveButton); utilityBox.set_margin(10); clearButton.set_margin(10) ; saveButton.set_margin(10); clearButton.set_hexpand(10); saveButton.set_hexpand(10);
	imageBox.set_margin(10);
	imagePreview.set_child(imageBox); imagePreview.set_expand();
	fileButton.set_margin(10);  fileButton.set_hexpand(10);
	fileButton1.set_margin(10); fileButton1.set_hexpand(10);
	fileButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onFileSelected));
	fileButton1.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::startWebcam));
	saveButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::savePDF));
	clearButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::clearData));
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

void MainWindow::startWebcam(){
   string path = scanImageToWebcam(convertToFloat(heightEntry.get_text()), convertToFloat(widthEntry.get_text()));
   auto aer = new Gtk::Image(path);
   aer->set_size_request(300, 300);
   aer->set_margin(10);
   imageBox.append(*aer);
   imageWidgets.push_back(aer);
}

MainWindow::~MainWindow(){ }

void MainWindow::savePDF(){
	convertToPDF(imagePaths);
}

void MainWindow::clearData(){
	for(int i=0;i<imageWidgets.size();i++){
		imageBox.remove(*imageWidgets[i]);
	}
	imagePaths.clear();
	imageWidgets.clear();
}

void MainWindow::onFileDialogResponse(int responseId, Gtk::FileChooserDialog* dialog){
	switch(responseId){
		case Gtk::ResponseType::OK:{ 
									   auto fileName = dialog->get_file()->get_path();
									   string path = scanImage(fileName, convertToFloat(heightEntry.get_text()), convertToFloat(widthEntry.get_text()));
									   auto aer = new Gtk::Image(path);
									   aer->set_size_request(300, 300);
									   aer->set_margin(10);
									   imageBox.append(*aer);
									   imageWidgets.push_back(aer);
									   break;
	}
		case Gtk::ResponseType::CANCEL:{
									   break;
	}
									   default:{
									   std::cout<<"Unexpected Button Clicked. "<<std::endl;
									   break;
	}
}
	delete dialog;
}

