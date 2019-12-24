#ifndef MAIN_GRID_H
#define MAIN_GRID_H

#include <iostream>
#include <string>
#include <QApplication>
#include <QtGui>
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

class The_Process_Bar;
class The_Player;

class Main_Grid : public QGridLayout
{
	Q_OBJECT

public:
	Main_Grid(QWidget *parent) :QGridLayout(parent) { this->window_ = parent;}
	~Main_Grid() {}

	void InitAll();


private:
	QWidget *window_;
	vector<TheButtonInfo> GetInfoIn(string);
	vector<TheButtonInfo> videos_;

	void SetVideoPlayer();
	QVideoWidget * video_widget_;
	The_Player * video_player_;	// Used in Set_VideoButton to set video content

	void SetVolumeSlider();
	QSlider * volume_slider_;
	int volume_;

	void SetBrightSlider();
	QSlider * bright_slider_;
	int bright_;

	void SetControlButton();
	QHBoxLayout * control_bar_layout_;
	QPushButton * next_video_;
	QPushButton * front_video_;
	QPushButton * pause_video_;
	QPushButton * play_video_;
	QPushButton * full_screen_;
	The_Process_Bar * process_slider_;

	void SetVideoButton();
	QScrollArea * button_scroll_;


	void MakeConnections();

};

#endif