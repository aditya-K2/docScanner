#pragma once

#include <gtkmm-4.0/gtkmm.h>
#include <gtkmm-4.0/gtkmm/application.h>

class MainWindow : public Gtk::Window {
public:
  MainWindow();
  virtual ~MainWindow();

protected:
  void on_file_selected();
  void on_file_dialog_response(int responseId, Gtk::FileChooserDialog *dialog);
  void start_webcam();
  void save_pdf();
  void clear_data();
  Gtk::Box main_box, button_box, entry_box, image_box, utility_box;
  Gtk::Button file_button, file_button1, clear_button, save_button;
  Gtk::Entry height_entry, width_entry;
  Gtk::Separator s, se;
  Gtk::ScrolledWindow image_preview;
  Gtk::Label image_label;
  std::unique_ptr<Gtk::MessageDialog> m_pDialog;
};
