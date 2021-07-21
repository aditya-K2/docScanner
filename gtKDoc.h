#include<gtkmm-4.0/gtkmm.h>
#include<gtkmm-4.0/gtkmm/application.h>
#include"oCV.h"

using namespace std;

class MainWindow: public Gtk::Window {
	public:
		MainWindow();
		virtual ~MainWindow();
	protected:
		void onFileSelected();
		void onFileDialogResponse(int responseId, Gtk::FileChooserDialog* dialog);
		void startWebcam();
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
	fileButton1.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::startWebcam));
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
	scanImageToWebcam(convertToFloat(heightEntry.get_text()), convertToFloat(widthEntry.get_text()));
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

