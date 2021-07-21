#include"gtKDoc.h"

int main(int argc, char *argv[]){
	auto app = Gtk::Application::create("Doc Scanner");
	return app->make_window_and_run<MainWindow>(argc, argv);
}
