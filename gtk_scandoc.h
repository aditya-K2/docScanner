#pragma once

#include <gtkmm-4.0/gtkmm.h>
#include <gtkmm-4.0/gtkmm/application.h>

class MainWindow : public Gtk::Window {
public:
  MainWindow();
  virtual ~MainWindow();

protected:
  void onFileSelected();
  void onFileDialogResponse(int responseId, Gtk::FileChooserDialog *dialog);
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
