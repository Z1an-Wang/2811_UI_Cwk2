#ifndef MAIN_GRID_H
#define MAIN_GRID_H

#include <iostream>
#include <string>
#include <QApplication>
#include <QtGui>
#include <QDebug.h>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlayer.h>
#include <QtMultimedia/QMediaPlaylist.h>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>

#include <QtCore/QDir.h>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QImageReader>

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
	vector<TheButtonInfo> videos_;

	void Set_VideoPlayer();
	QVideoWidget * videoWidget;
	ThePlayer * videoPlayer;	// Used in Set_VideoButton to set video content

	void Set_VolumeSlider();
	QSlider * volumeSlider;
	int volume_;

	void Set_BrightSlider();
	QSlider * brightSlider;
	int bright_;

	void Set_ControlButton();
	QHBoxLayout * controlBarLayout_;
	QPushButton * nextVideo_;
	QPushButton * frontVideo_;
	QPushButton * pauseVideo_;
	QPushButton * playVideo_;


	void Set_VideoButton();
	QScrollArea * buttonScroll;


	void Make_Connections();

};

#endif