# scandoc
A GTK Application to scan Documents and Compile PDFs
![Screenshot](./image)

## Download the prerequisites.

Download the following libraries from your distro's package manager

- `gtkmm` *(C++ Interface for GTK)*
- `podofo` *(A C++ library to work with the PDF file format)*
- `opencv` *(Open Source Computer Vision Library)*
- `gstreamer gstreamermm gstreamer-vaapi` *(For Web Cam playback)*
- `glew vtk jsoncpp hdf5` *(Dependencies for other libraries,
Your package manager will install them for you)*

### On Arch Linux:

```sh
sudo pacman -S gtkmm-4.0 podofo opencv gstreamer gstreamermm gstreamer-vaapi glew vtk jsoncpp hdf5
```
### Building

```sh
$ git clone https://github.com/aditya-K2/scandoc
$ cd scandoc
$ cmake .
$ make
```

#### Note:

You can generate `compile_commands.json` for clangd or other clang-tools

```bash
$ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
```
