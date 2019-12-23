//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include "Main_Grid.h"

// The Player has two Vertor data field for botton Object and botton Info
// This function will call at the initial stage 
// To pass the two reference to the data field of the player class
// Then jumpTo the first video to play
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i){
    this->buttons_ = b;
    this->infos_ = i;
	this->totalVideo_ = buttons_->size();
	this->currentVideo_ = 0;
    jumpTo(buttons_ -> at(currentVideo_) -> info);
}


// SLOT Define in <the_player> class

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {

// Use this block need to move the connect() from constructor to the Make_Connections
/*	try {
		QPushButton *play = dynamic_cast<Main_Grid *>(this->parent())->playVideo_;
		QPushButton *pause = dynamic_cast<Main_Grid *>(this->parent())->pauseVideo_;
		QGridLayout *layout = dynamic_cast<Main_Grid *>(this->parent())->controlBarLayout_;
		QRect g = static_cast<Main_Grid *>(this->parent())->g;

		if (ms == QMediaPlayer::PlayingState) {
			layout->addWidget(pause, 0, 1);
		}
		else {
			layout->addWidget(play, 0, 1);
		}
	}
	catch (bad_cast) {
		qDebug() << "Bad Cast in [playStateChanged]";
	}*/
}

// The [Public SLOT] to switch the video with the ButtonInfo provided.
void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}

void ThePlayer::jumpNext() {

	currentVideo_ = (currentVideo_ + 1) % totalVideo_;
	setMedia( * infos_ ->at(currentVideo_).url);
	play();
}

void ThePlayer::jumpFront() {

	currentVideo_ = --currentVideo_ < 0 ? totalVideo_ - 1 : currentVideo_;
	setMedia(*infos_->at(currentVideo_).url);
	play();
}