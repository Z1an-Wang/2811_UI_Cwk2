#ifndef MAIN_GRID_H
#define MAIN_GRID_H

#include <iostream>
#include <string>
#include <QApplication>
#include <QtGui>

// About the media player
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlayer.h>
#include <QtMultimedia/QMediaPlaylist.h>

// All need widgets
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>

// To support the import module
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

	// Used in the player class to control the video status
	QPushButton * pause_play_;

// Change the none-agrs signal to bool-args signal;
// For the fullscreen button [clicked() -> setFullScreen(bool)]
signals:
	void isFullScreen(bool clicked);

public slots:
	void FullScreenClicked(){emit isFullScreen(true);}

private:
	QWidget *window_;
	vector<TheButtonInfo> GetInfoIn(string);	//Get Icon and Info for each video button
	vector<TheButtonInfo> videos_;				// Data field to store the Info for each button

	// Different Modules for the whole Grid Layout
	// Set widgets in private field in order to make connection.
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
	QPushButton * full_screen_;
	The_Process_Bar * process_slider_;

	void SetVideoButton();
	QScrollArea * button_scroll_;

	// Deal with all connections
	void MakeConnections();

};

#endif