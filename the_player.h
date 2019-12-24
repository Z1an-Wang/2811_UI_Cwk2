//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include <QtWidgets/QSlider>
#include "the_button.h"
#include <vector>

using namespace std;

const int kInitVolume = 10;
const int kInitBright = 0;

class The_Player : public QMediaPlayer {

Q_OBJECT

private:
    // Contains the datafile with botton object and info( Icon and Video Url)
    // Video Url used to create MediaContent [ QMediaContent(const QUrl &url) ]
    std::vector<TheButtonInfo>* infos_;
    std::vector<TheButton*>* buttons_;
	int total_video_;
    int current_video_;

public:
    The_Player(QObject *parent) : QMediaPlayer(parent) {
        setVolume(kInitVolume);     // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
    }

    // all buttons have been setup, store pointers here
    void setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i);


private slots:

    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void JumpTo (TheButtonInfo* button);
	void JumpNext();
	void JumpFront();
	void SetPosition(int);
};

class The_Process_Bar : public QSlider{

    Q_OBJECT

public:
    // Qt::Orientation orientation, QWidget *parent = nullptr
    The_Process_Bar(QWidget *parent = nullptr) : QSlider(Qt::Horizontal, parent) {}

public slots : 
    void SetProcessRange(qint64 position);
    void SetCurrentPosition(qint64 position);

private:


};

//CW2_THE_PLAYER_H
#endif 
