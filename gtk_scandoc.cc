#include "gtk_scandoc.h"
#include "ocv_scandoc.h"

std::vector<Gtk::Image *> image_widgets;
extern std::vector<std::string> image_paths;

MainWindow::MainWindow()
    : file_button("Choose An Image"), file_button1("Open Webcam"),
      main_box(Gtk::Orientation::VERTICAL),
      button_box(Gtk::Orientation::HORIZONTAL),
      entry_box(Gtk::Orientation::VERTICAL),
      image_box(Gtk::Orientation::HORIZONTAL),
      utility_box(Gtk::Orientation::HORIZONTAL), clear_button("Clear"),
      save_button("Save") {
  set_title("Select An Image To Scan");
  set_child(main_box);
  set_default_size(640, 480);

  height_entry.set_text("Enter Height For Output Image");
  height_entry.set_margin(30);
  height_entry.select_region(0, height_entry.get_text_length());
  width_entry.set_text("Enter Width For Output Image");
  width_entry.set_margin(30);
  width_entry.select_region(0, width_entry.get_text_length());

  image_label.set_text("Current Images in Buffers");
  image_label.set_margin(10);
  main_box.append(entry_box);
  main_box.append(button_box);
  main_box.append(s);
  main_box.append(image_label);
  main_box.append(image_preview);
  main_box.append(se);
  main_box.append(utility_box);
  entry_box.append(height_entry);
  entry_box.append(width_entry);
  button_box.append(file_button);
  button_box.append(file_button1);
  utility_box.append(clear_button);
  utility_box.append(save_button);
  utility_box.set_margin(10);
  clear_button.set_margin(10);
  save_button.set_margin(10);
  clear_button.set_hexpand(10);
  save_button.set_hexpand(10);
  image_box.set_margin(10);
  image_preview.set_child(image_box);
  image_preview.set_expand();
  file_button.set_margin(10);
  file_button.set_hexpand(10);
  file_button1.set_margin(10);
  file_button1.set_hexpand(10);
  file_button.signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_file_selected));
  file_button1.signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::start_webcam));
  save_button.signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::save_pdf));
  clear_button.signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::clear_data));
}

void MainWindow::on_file_selected() {
  auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                           Gtk::FileChooser::Action::OPEN);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &MainWindow::on_file_dialog_response), dialog));

  // Add response buttons to the dialog:
  dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  dialog->add_button("_Open", Gtk::ResponseType::OK);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog->add_filter(filter_any);

  // Show the dialog and wait for a user response:
  dialog->show();
}

void MainWindow::start_webcam() {
  std::string path = scan_image_webcam(str_to_float(height_entry.get_text()),
                                       str_to_float(width_entry.get_text()));
  if (path == "") {
    exit(-1);
  }
  auto aer = new Gtk::Image(path);
  aer->set_size_request(300, 300);
  aer->set_margin(10);
  image_box.append(*aer);
  image_widgets.push_back(aer);
}

MainWindow::~MainWindow() {}

void MainWindow::save_pdf() {
  convert_to_pdf(image_paths);
  m_pDialog.reset(new Gtk::MessageDialog(*this, "PDF has been Saved!"));
  m_pDialog->set_modal(true);
  m_pDialog->set_hide_on_close(true);
  m_pDialog->signal_response().connect(
      sigc::hide(sigc::mem_fun(*m_pDialog, &Gtk::Widget::hide)));
  m_pDialog->show();
}

void MainWindow::clear_data() {
  for (int i = 0; i < image_widgets.size(); i++) {
    image_box.remove(*image_widgets[i]);
  }
  image_paths.clear();
  image_widgets.clear();
}

void MainWindow::on_file_dialog_response(int responseId,
                                         Gtk::FileChooserDialog *dialog) {
  switch (responseId) {
  case Gtk::ResponseType::OK: {
    auto fileName = dialog->get_file()->get_path();
    std::string path =
        scan_image(fileName, str_to_float(height_entry.get_text()),
                   str_to_float(width_entry.get_text()));
    if (path == "") {
      exit(-1);
    }
    auto aer = new Gtk::Image(path);
    aer->set_size_request(300, 300);
    aer->set_margin(10);
    image_box.append(*aer);
    image_widgets.push_back(aer);
    break;
  }
  case Gtk::ResponseType::CANCEL: {
    break;
  }
  default: {
    std::cout << "Unexpected Button Clicked. " << std::endl;
    break;
  }
  }
  delete dialog;
}
