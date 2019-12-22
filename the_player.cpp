//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

// The Player has two Vertor data field for botton Object and botton Info
// This function will call at the initial stage 
// To pass the two reference to the data field of the player class
// Then jumpTo the first video to play
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i){
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}


void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
}

// The [Public SLOT] to switch the video with the ButtonInfo provided.
void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}
