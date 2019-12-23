/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for no reason
 *
 * 2811 cw2 November 2019 by twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
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

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDirIterator it(QString::fromStdString(loc), QDir::Files);

    while (it.hasNext()) { // for all files

		QString f = it.next();

        if (!f.contains(".png")) { // if it isn't an image
            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // but a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
					if (!sprite.isNull()) {
						QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
						QUrl* url = new QUrl(QUrl::fromLocalFile(f)); // convert the file location to a generic url
						out.push_back(TheButtonInfo(url, ico)); // add to the output list
					}
					else
						qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb ;
            }
            else
				qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
	qDebug() << "Qt version: " << QT_VERSION_STR;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos = getInfoIn("./Resources/video");

    if (videos.size() == 0) {
		qDebug() << "no videos found";
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;
	videoWidget -> setBaseSize(600, 480);

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget ->setLayout(layout);
    buttonWidget -> setFixedWidth(1540);
    buttonWidget -> setFixedHeight(150);
    QScrollArea* buttonScroll = new QScrollArea();
    buttonScroll -> setWidget(buttonWidget);
    buttonScroll -> setFixedHeight(170);

    // create the four buttons
    for ( size_t i = 0; i < videos.size(); i++ ) {
        TheButton *button = new TheButton(buttonScroll);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    QSlider *volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setTickPosition(QSlider::TicksBelow);
    volumeSlider->setTickInterval(10);
    volumeSlider->setValue(_initVolume);

    QSlider::connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    QMediaPlayer::connect(player, SIGNAL(volumeChanged(int)), volumeSlider, SLOT(setValue(int)));

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");

    // add the video and the buttons to the top level widget
	top->addWidget(videoWidget);
    top->addWidget(buttonScroll);
    top->addWidget(volumeSlider);

	window.setMinimumSize(300, 200);
	window.setGeometry(40, 40, 800, 680);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
