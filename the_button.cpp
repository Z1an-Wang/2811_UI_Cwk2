//
// Created by twak on 11/11/2019.
//

#include "the_button.h"

// Initial the botton Icon by the BottonInfo
void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info = i;
}

// The Clicked signal trigger another SIGNAL
void TheButton::clicked() {
    emit jumpTo(info);
}