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
	videos_ = GetInfoIn("./Resources/video");

	if (videos_.size() == 0) {
		qDebug() << "no videos found";
		exit(-1);
	}

	this->Set_VideoPlayer();
	this->Set_VideoButton();

	this->Set_ControlButton();
	this->Set_VolumeSlider();
	this->Set_BrightSlider();

	setSpacing(5);

	for (int i = 0; i < 12; i++) {
		setColumnStretch(i, 1);		// Split one row into 12 equally scaled column
	}
	setRowStretch(0, 0);	// Title (void for the moment)
	setRowStretch(1, 1);	// The main Video Window and the Sliders
	setRowStretch(2, 0);	// The Control button and Video progress bar
	setRowStretch(3, 0);	// The image buttons of the video 
	setRowStretch(4, 0);	// Void for the moment

	this->Make_Connections();
	
	return;
}

void Main_Grid::Set_VideoPlayer() {

	// the widget that will show the video
	this->videoWidget = new QVideoWidget();
	bright_ = kInitBright;
	videoWidget->setBrightness(kInitBright);

	videoWidget->setBaseSize(600, 480);

	// the QMediaPlayer which controls the playback
	this-> videoPlayer = new ThePlayer(this);
	volume_ = kInitVolume;
	videoPlayer->setVideoOutput(videoWidget);


	this->addWidget(videoWidget,1,1, 1, 10);
}

void Main_Grid::Set_VolumeSlider() {
	QVBoxLayout * volumeVLayout = new QVBoxLayout();

	this->volumeSlider = new QSlider(Qt::Vertical);
	volumeSlider->setRange(0, 100);
	volumeSlider->setFixedWidth(25);
	volumeSlider->setTickPosition(QSlider::TicksLeft);
	volumeSlider->setTickInterval(10);
	volumeSlider->setValue(kInitVolume);

	QLabel *volumeIcon = new QLabel();
	QPixmap temp("./Resources/volume.jpg");
	QPixmap image = temp.scaled(QSize(25, 25), Qt::KeepAspectRatio);
	volumeIcon->setPixmap(image);
	volumeIcon->setFixedSize(25, 25);

	volumeVLayout->addWidget(volumeSlider);
	volumeVLayout->addWidget(volumeIcon);

	this->addLayout(volumeVLayout, 1, 11, 1, 1);
}

void Main_Grid::Set_BrightSlider() {
	QVBoxLayout * brightVLayout = new QVBoxLayout();

	this->brightSlider = new QSlider(Qt::Vertical);
	brightSlider->setRange(-100, 100);
	brightSlider->setFixedWidth(25);
	brightSlider->setTickPosition(QSlider::TicksRight);
	brightSlider->setTickInterval(20);
	brightSlider->setValue(kInitBright);

	QLabel *brightIcon = new QLabel();
	QPixmap temp("./Resources/bright.jpg");
	QPixmap image = temp.scaled(QSize(25, 25), Qt::KeepAspectRatio);
	brightIcon->setPixmap(image);
	brightIcon->setFixedSize(25, 25);

	brightVLayout->addWidget(brightSlider);
	brightVLayout->addWidget(brightIcon);

	this->addLayout(brightVLayout, 1, 0, 1, 1);
}

void Main_Grid::Set_ControlButton() {
	this->controlBarLayout_ = new QHBoxLayout();

	this->nextVideo_ = new QPushButton();
	nextVideo_->setFixedSize(45, 30);
	nextVideo_->setText(">");

	this->frontVideo_ = new QPushButton();
	frontVideo_->setFixedSize(45, 30);
	frontVideo_->setText("<");

	this->pauseVideo_ = new QPushButton();
	pauseVideo_->setFixedSize(75, 30);
	pauseVideo_->setText("Pause");

	this->playVideo_ = new QPushButton();
	playVideo_->setFixedSize(75, 30);
	playVideo_->setText("Play");

	controlBarLayout_->addWidget(frontVideo_);
	controlBarLayout_->addWidget(playVideo_);
	controlBarLayout_->addWidget(pauseVideo_);
	controlBarLayout_->addWidget(nextVideo_);

	this->addLayout(controlBarLayout_, 2, 1, 1, 10);
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

	// a list of the buttons
	vector<TheButton*> buttons;
	// create the four buttons
	for (size_t i = 0; i < videos_.size(); i++) {
		TheButton *button = new TheButton(buttonScroll);
		button->connect(button, SIGNAL(jumpTo(TheButtonInfo*)), videoPlayer, SLOT(jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
		buttons.push_back(button);
		layout->addWidget(button);
		button->init(&videos_.at(i));
	}

	// tell the player what buttons and videos are available
	videoPlayer->setContent(&buttons, &videos_);


	buttonScroll->setWidget(buttonWidget);
	buttonScroll->setFixedHeight(170);
	this->addWidget(buttonScroll, 3, 1, 1, 10);
}

void Main_Grid::Make_Connections() {
	QSlider::connect(volumeSlider, SIGNAL(valueChanged(int)), videoPlayer, SLOT(setVolume(int)));
	QMediaPlayer::connect(videoPlayer, SIGNAL(volumeChanged(int)), volumeSlider, SLOT(setValue(int)));

	QSlider::connect(brightSlider, SIGNAL(valueChanged(int)), videoWidget, SLOT(setBrightness(int)));
	QVideoWidget::connect(videoWidget, SIGNAL(brightnessChanged(int)), brightSlider, SLOT(setValue(int)));

	QPushButton::connect(nextVideo_, SIGNAL(clicked()), videoPlayer, SLOT(jumpNext()));
	QPushButton::connect(frontVideo_, SIGNAL(clicked()), videoPlayer, SLOT(jumpFront()));
	QPushButton::connect(pauseVideo_, SIGNAL(clicked()), videoPlayer, SLOT(pause()));
	QPushButton::connect(playVideo_, SIGNAL(clicked()), videoPlayer, SLOT(play()));
}