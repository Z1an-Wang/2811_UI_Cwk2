//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include "Main_Grid.h"

// The Player has two Vertor data field for botton Object and botton Info
// This function will call at the initial stage 
// To pass the two reference to the data field of the player class
// Then jumpTo the first video to play
void The_Player::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i){
    this->buttons_ = b;
    this->infos_ = i;
	this->total_video_ = buttons_->size();
	this->current_video_ = 0;
    JumpTo(buttons_ -> at(current_video_) -> info);
}


// SLOT Define in <the_player> class

void The_Player::playStateChanged (QMediaPlayer::State ms) {

// Use this block need to move the connect() from constructor to the Make_Connections
/*	try {
		QPushButton *play = dynamic_cast<Main_Grid *>(this->parent())->play_video_;
		QPushButton *pause = dynamic_cast<Main_Grid *>(this->parent())->pause_video_;
		QGridLayout *layout = dynamic_cast<Main_Grid *>(this->parent())->control_bar_layout_;

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
void The_Player::JumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}

void The_Player::JumpNext() {

	current_video_ = (current_video_ + 1) % total_video_;
	setMedia( * infos_ ->at(current_video_).url);
	play();
}

void The_Player::JumpFront() {

	current_video_ = --current_video_ < 0 ? total_video_ - 1 : current_video_;
	setMedia(*infos_->at(current_video_).url);
	play();
}

void The_Process_Bar::SetProcessRange(qint64 position){
	QSlider::setRange(0, int(position));
}
void The_Process_Bar::SetCurrentPosition(qint64 position){
	QSlider::setValue(int(position));
}