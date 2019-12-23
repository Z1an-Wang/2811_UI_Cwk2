#ifndef MAIN_GRID_H
#define MAIN_GRID_H

#include <iostream>
#include <string>
#include <QApplication>
#include <QtGui>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/qmediaplayer.h>
#include <QtMultimedia/qmediaplaylist.h>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QScrollArea>

#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QImageReader>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>

#include "the_player.h"
#include "the_button.h"

using namespace std;

class Main_Grid : public QGridLayout
{
	Q_OBJECT

public:
	Main_Grid(QWidget *parent) :QGridLayout(parent) { this->window = parent;}
	~Main_Grid() {}

	void Init_All();





private:
	QWidget *window;
	vector<TheButtonInfo> GetInfoIn(string);
	vector<TheButtonInfo> videos;

	void Set_VideoPlayer();
	QVideoWidget * videoWidget;
	ThePlayer * videoPlayer;	// Used in Set_VideoButton to set video content

	void Set_VideoButton();
	QScrollArea * buttonScroll;

	void Set_ControlButton();

	void Set_VolumeSlider();
	QSlider * volumeSlider;
	int volume_;
	void Set_BrightSlider();
	QSlider * brightSlider;
	int bright_;


	void Make_Connections();

};

#endif