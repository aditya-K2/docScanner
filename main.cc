#include<gtkmm-4.0/gtkmm.h>
#include<gtkmm-4.0/gtkmm/application.h>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/img_hash.hpp>
#include <iostream>

using namespace std;

class MainWindow: public Gtk::Window {
	public:
		MainWindow();
		virtual ~MainWindow();
	protected:
		void onFileSelected();
		void onFileDialogResponse(int responseId, Gtk::FileChooserDialog* dialog);
		Gtk::Box buttonBox;
		Gtk::Button fileButton;
};

MainWindow::MainWindow() : buttonBox(Gtk::Orientation::VERTICAL), fileButton("Choose An Image"){
	set_title("Select An Image To Scan");
	set_child(buttonBox);
	buttonBox.append(fileButton);
	fileButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onFileSelected));
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
									   std::cout<< "Open Clicked." << std::endl;
									   auto fileName = dialog->get_file()->get_path();
									   std::cout<< "File Selected: "<< fileName << std::endl;
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
