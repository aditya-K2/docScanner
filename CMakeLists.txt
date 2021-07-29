cmake_minimum_required(VERSION 3.20)
project(example)

find_package(PkgConfig)
find_package( OpenCV REQUIRED )

pkg_check_modules(GTKMM gtkmm-4.0)

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

include_directories(
    "/usr/include/podofo/"  )

include_directories( 
	${GTKMM_LIBRARIES} )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(example 
    # what.cpp )
	gui.cc)

target_link_libraries(example 
	${GTKMM_LIBRARIES} ${OpenCV_LIBS} /usr/lib/libpodofo.so)
