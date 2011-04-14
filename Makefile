# simple Makefile for project: Pupil Tracker Demo

CPPFLAGS = -O3 `wx-config --cxxflags --libs --gl-libs` `pkg-config opencv --cflags --libs`

gazetracker: main.o gui.o MainFrame.o GazeTracker.o VideoFrame.o OwnCVImage.o ImagePanel.o V4L2Helper.o Webcamera.o
	g++ $(CPPFLAGS) -o gazetracker main.o gui.o MainFrame.o GazeTracker.o VideoFrame.o OwnCVImage.o ImagePanel.o V4L2Helper.o Webcamera.o

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

gui.o: gui.cpp gui.h
	g++ $(CPPFLAGS) -c gui.cpp

MainFrame.o: MainFrame.cpp MainFrame.h gui.cpp gui.h
	g++ $(CPPFLAGS) -c MainFrame.cpp

GazeTracker.o: GazeTracker.cpp GazeTracker.h Webcamera.cpp Webcamera.h
	g++ $(CPPFLAGS) -c GazeTracker.cpp

VideoFrame.o: VideoFrame.cpp VideoFrame.h gui.cpp gui.h
	g++ $(CPPFLAGS) -c VideoFrame.cpp

OwnCVImage.o: OwnCVImage.cpp OwnCVImage.h
	g++ $(CPPFLAGS) -c OwnCVImage.cpp

ImagePanel.o: ImagePanel.cpp ImagePanel.h
	g++ $(CPPFLAGS) -c ImagePanel.cpp

V4L2Helper.o: V4L2Helper.cpp V4L2Helper.h
	g++ $(CPPFLAGS) -c V4L2Helper.cpp

Webcamera.o: Webcamera.cpp Webcamera.h V4L2Helper.cpp V4L2Helper.h
	g++ $(CPPFLAGS) -c Webcamera.cpp

clean:
	rm *.o gazetracker 
