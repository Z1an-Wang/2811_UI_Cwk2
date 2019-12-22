//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H


#include <QtWidgets/QPushButton>


class TheButtonInfo {

public:
    QUrl* url;                  // video file to play
    QIcon* icon;                // icon to display

    // The initializer list
    TheButtonInfo ( QUrl* url, QIcon* icon) : url (url), icon (icon) {}
};

class TheButton : public QPushButton {

    // The macro to define SLOT and SINAL
    Q_OBJECT

public:
    TheButtonInfo* info;        // Data fields to record the current video

    TheButton(QWidget *parent) : QPushButton(parent){

        // Can change the button size here
        setIconSize(QSize(200, 110));

        // if QPushButton clicked...then run clicked() below
        connect(this, SIGNAL(released()), this, SLOT(clicked()));
    }

    // Set the initial video to display 
    // And set its icon as the button cover
    void init(TheButtonInfo* i);

private slots:
    // Override the clicked() [ from Public SIGNAL in QAbstractButton ]
    // This SLOT will emitted the jumpTo SIGANL of this Widgets class
    void clicked();

signals:
    // Transmit the ButtonInfo to the MadiePlayer named jumpTo(SLOT)
    void jumpTo(TheButtonInfo*);

};

#endif //CW2_THE_BUTTON_H
