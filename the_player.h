//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>

using namespace std;

const int kInitVolume = 10;
const int kInitBright = 0;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    // Contains the datafile with botton object and info( Icon and Video Url)
    // Video Url used to create MediaContent [ QMediaContent(const QUrl &url) ]
    std::vector<TheButtonInfo>* infos_;
    std::vector<TheButton*>* buttons_;
	int totalVideo_;
    int currentVideo_;

public:
    ThePlayer(QWidget *parent) : QMediaPlayer(parent) {
        setVolume(kInitVolume);     // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
    }

    // all buttons have been setup, store pointers here
    void setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i);

	void Set_VideoCount();

private slots:

    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
};

#endif //CW2_THE_PLAYER_H
