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
#include <string>
#include <QApplication>

#include "the_player.h"
#include "the_button.h"
#include "Main_Grid.h"



int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
	qDebug() << "Qt version: " << QT_VERSION_STR;

    // create the Qt Application
    QApplication app(argc, argv);

	// create the main window and layout
	QWidget window;
	window.setMinimumSize(300, 200);
	window.setGeometry(40, 40, 800, 680);

	Main_Grid *main_content = new Main_Grid(&window);

	main_content->Init_All();

	window.setLayout(main_content);
    window.setWindowTitle("tomeo");

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
