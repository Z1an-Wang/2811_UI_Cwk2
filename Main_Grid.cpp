#include "Main_Grid.h"

// read in videos and thumbnails to this directory
vector<TheButtonInfo> Main_Grid::GetInfoIn(string loc) {

	vector<TheButtonInfo> out = vector<TheButtonInfo>();
	QDirIterator it(QString::fromStdString(loc), QDir::Files);

	while (it.hasNext()) { // for all files

		QString f = it.next();

		if (!f.contains(".png")) { // if it isn't an image
			QString thumb = f.left(f.length() - 4) + ".png";
			if (QFile(thumb).exists()) { // but a png thumbnail exists
				QImageReader *imageReader = new QImageReader(thumb);
				QImage sprite = imageReader->read(); // read the thumbnail
				if (!sprite.isNull()) {
					QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
					QUrl* url = new QUrl(QUrl::fromLocalFile(f)); // convert the file location to a generic url
					out.push_back(TheButtonInfo(url, ico)); // add to the output list
				}
				else
					qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb;
			}
			else
				qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb;
		}
	}

	return out;
}

void Main_Grid::Init_All() {

	// collect all the videos in the folder
	videos = GetInfoIn("./Resources/video");

	if (videos.size() == 0) {
		qDebug() << "no videos found";
		exit(-1);
	}

	this->Set_VideoPlayer();
	this->Set_VideoButton();
	this->Set_VolumeSlider();
	this->Set_BrightSlider();

	setSpacing(5);

	for (int i = 0; i < 12; i++) {
		setColumnStretch(i, 1);
	}
	setRowStretch(0, 0);
	setRowStretch(1, 1);
	setRowStretch(2, 0);
	setRowStretch(3, 0);
	setRowStretch(4, 0);

	this->Make_Connections();

}

void Main_Grid::Set_VideoPlayer() {

	// the widget that will show the video
	this->videoWidget = new QVideoWidget(this->widget());
	bright_ = kInitBright;
	videoWidget->setBrightness(kInitBright);

	videoWidget->setBaseSize(600, 480);

	// the QMediaPlayer which controls the playback
	this-> videoPlayer = new ThePlayer(this->widget());
	this->bright_ = kInitVolume;
	videoPlayer->setVideoOutput(videoWidget);


	this->addWidget(videoWidget,1,1, 1, 10);
}

void Main_Grid::Set_VideoButton() {

	// Set the srcoll area the hold the buttons
	this -> buttonScroll = new QScrollArea(this->widget());
	// the buttons are arranged horizontally
	QHBoxLayout *layout = new QHBoxLayout(buttonScroll);
	// a row of buttons
	QWidget *buttonWidget = new QWidget(buttonScroll);

	buttonWidget->setLayout(layout);

	buttonWidget->setFixedWidth(1540);
	buttonWidget->setFixedHeight(150);
	buttonScroll->setAlignment(Qt::AlignCenter);
	buttonScroll->setWidget(buttonWidget);


	// a list of the buttons
	vector<TheButton*> buttons;
	// create the four buttons
	for (size_t i = 0; i < videos.size(); i++) {
		TheButton *button = new TheButton(buttonScroll);
		button->connect(button, SIGNAL(jumpTo(TheButtonInfo*)), videoPlayer, SLOT(jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
		buttons.push_back(button);
		layout->addWidget(button);
		button->init(&videos.at(i));
	}

	// tell the player what buttons and videos are available
	videoPlayer->setContent(&buttons, &videos);


	buttonScroll->setFixedHeight(170);
	this->addWidget(buttonScroll, 2, 1, 1, 10);
}

void Main_Grid::Set_ControlButton() {

}

void Main_Grid::Set_VolumeSlider() {
	this->volumeSlider = new QSlider(Qt::Horizontal);
	volumeSlider->setRange(0, 100);
	volumeSlider->setTickPosition(QSlider::TicksBelow);
	volumeSlider->setTickInterval(10);
	volumeSlider->setValue(kInitVolume);

	volumeSlider->setFixedHeight(25);
	this->addWidget(volumeSlider, 3, 1, 1, 10);
}

void Main_Grid::Set_BrightSlider() {
	this->brightSlider = new QSlider(Qt::Horizontal);
	brightSlider->setRange(-100, 100);
	brightSlider->setTickPosition(QSlider::TicksBelow);
	brightSlider->setTickInterval(20);
	brightSlider->setValue(kInitBright);

	brightSlider->setFixedHeight(25);
	this->addWidget(brightSlider, 4, 1, 1, 10);
}

void Main_Grid::Make_Connections() {
	QSlider::connect(volumeSlider, SIGNAL(valueChanged(int)), videoPlayer, SLOT(setVolume(int)));
	QMediaPlayer::connect(videoPlayer, SIGNAL(volumeChanged(int)), volumeSlider, SLOT(setValue(int)));

	QSlider::connect(brightSlider, SIGNAL(valueChanged(int)), videoWidget, SLOT(setBrightness(int)));
	QVideoWidget::connect(videoWidget, SIGNAL(brightnessChanged(int)), brightSlider, SLOT(setValue(int)));
}