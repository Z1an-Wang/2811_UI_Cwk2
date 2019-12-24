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

void Main_Grid::InitAll() {

	// collect all the videos in the folder
	videos_ = GetInfoIn("./Resources/video");

	if (videos_.size() == 0) {
		qDebug() << "no videos found";
		exit(-1);
	}

	// Set and initial all elements for the main window
	this->SetVideoPlayer();
	this->SetVideoButton();

	this->SetControlButton();
	this->SetVolumeSlider();
	this->SetBrightSlider();

	setSpacing(5);

	// Set the dynamic response for the layout
	for (int i = 0; i < 12; i++) {
		setColumnStretch(i, 1);		// Split one row into 12 equally scaled column
	}
	setRowStretch(0, 0);	// Title (void for the moment)
	setRowStretch(1, 1);	// The main Video Window and the Sliders
	setRowStretch(2, 0);	// The Control button and Video progress bar
	setRowStretch(3, 0);	// The image buttons of the video 
	setRowStretch(4, 0);	// Void for the moment

	// Finally, make all connections
	this->MakeConnections();
	
	return;
}

void Main_Grid::SetVideoPlayer() {

	// the widget that will show the video
	this->video_widget_ = new QVideoWidget();
	bright_ = kInitBright;
	video_widget_->setBrightness(kInitBright);

	video_widget_->setBaseSize(600, 480);

	// the QMediaPlayer which controls the playback
	this-> video_player_ = new The_Player(this);
	volume_ = kInitVolume;
	video_player_->setVideoOutput(video_widget_);

	this->addWidget(video_widget_,1,1, 1, 10);	// Add the video widget to grid layout
}

void Main_Grid::SetVolumeSlider() {
	// A Vertiacl Layout to hold the volume slider and icon
	QVBoxLayout * volumeVLayout = new QVBoxLayout();

	// Set the volume slider
	this->volume_slider_ = new QSlider(Qt::Vertical);		// Orientation
	volume_slider_->setRange(0, 100);						// Range
	volume_slider_->setFixedWidth(25);						// Width
	volume_slider_->setTickPosition(QSlider::TicksLeft);
	volume_slider_->setTickInterval(10);					// Interval
	volume_slider_->setValue(kInitVolume);					// Init Value

	// Set the Volume Icon
	QLabel *volumeIcon = new QLabel();
	QPixmap temp("./Resources/volume.jpg");
	QPixmap image = temp.scaled(QSize(25, 25), Qt::KeepAspectRatio);
	volumeIcon->setPixmap(image);
	volumeIcon->setFixedSize(25, 25);

	// add widgets to layout and add layout to main grid
	volumeVLayout->addWidget(volume_slider_);
	volumeVLayout->addWidget(volumeIcon);

	this->addLayout(volumeVLayout, 1, 11, 1, 1);
}

void Main_Grid::SetBrightSlider() {
	// A Vertiacl Layout to hold the bright slider and icon
	QVBoxLayout * brightVLayout = new QVBoxLayout();

	// Set the bright slider
	this->bright_slider_ = new QSlider(Qt::Vertical);		// Orientation
	bright_slider_->setRange(-100, 100);					// Range
	bright_slider_->setFixedWidth(25);						// Width
	bright_slider_->setTickPosition(QSlider::TicksRight);
	bright_slider_->setTickInterval(20);					// Interval
	bright_slider_->setValue(kInitBright);					// Init Value

	// Set the bright Icon
	QLabel *brightIcon = new QLabel();
	QPixmap temp("./Resources/bright.jpg");
	QPixmap image = temp.scaled(QSize(25, 25), Qt::KeepAspectRatio);
	brightIcon->setPixmap(image);
	brightIcon->setFixedSize(25, 25);

	// add widgets to layout and add layout to main grid
	brightVLayout->addWidget(bright_slider_);
	brightVLayout->addWidget(brightIcon);

	this->addLayout(brightVLayout, 1, 0, 1, 1);
}

void Main_Grid::SetControlButton() {
	this->control_bar_layout_ = new QHBoxLayout();

	this->next_video_ = new QPushButton();
	next_video_->setFixedSize(65, 35);
	next_video_->setText(">");
	next_video_->setShortcut(QKeySequence(QLatin1String("Ctrl+f")));

	this->front_video_ = new QPushButton();
	front_video_->setFixedSize(65, 35);
	front_video_->setText("<");
	front_video_->setShortcut(QKeySequence(QLatin1String("Ctrl+d")));

	this->pause_play_ = new QPushButton();
	pause_play_->setFixedSize(85, 35);
	pause_play_->setText("Pause");
	pause_play_->setShortcut(QKeySequence(QLatin1String("Space")));

	this->process_slider_ = new The_Process_Bar();

	this->full_screen_ = new QPushButton();
	full_screen_->setFixedSize(100, 35);
	full_screen_->setText("Full Screen");
	full_screen_->setShortcut(QKeySequence(QLatin1String("Ctrl+x")));

	control_bar_layout_->addWidget(front_video_);
	control_bar_layout_->addWidget(pause_play_);
	control_bar_layout_->addWidget(next_video_);
	control_bar_layout_->addWidget(process_slider_);
	control_bar_layout_->addWidget(full_screen_);
	this->addLayout(control_bar_layout_, 2, 0, 1, 12);
}

void Main_Grid::SetVideoButton() {

	// Set the srcoll area the hold the buttons
	this -> button_scroll_ = new QScrollArea(this->widget());
	// the buttons are arranged horizontally
	QHBoxLayout *layout = new QHBoxLayout(button_scroll_);
	// a row of buttons
	QWidget *buttonWidget = new QWidget(button_scroll_);

	buttonWidget->setLayout(layout);

	buttonWidget->setFixedWidth(1540);
	buttonWidget->setFixedHeight(150);
	button_scroll_->setAlignment(Qt::AlignCenter);

	// a list of the buttons
	vector<TheButton*> buttons;
	// Create all video buttons 
	// connect the buttons to switching specific videos
	// set it into the widget and class data field
	for (size_t i = 0; i < videos_.size(); i++) {
		TheButton *button = new TheButton(button_scroll_);
		button->connect(button, SIGNAL(jumpTo(TheButtonInfo*)), video_player_, SLOT(JumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
		buttons.push_back(button);
		layout->addWidget(button);
		button->init(&videos_.at(i));
	}
	// tell the player what buttons and videos are available
	video_player_->setContent(&buttons, &videos_);

	button_scroll_->setWidget(buttonWidget);
	button_scroll_->setFixedHeight(180);
	this->addWidget(button_scroll_, 3, 0, 1, 12);
}

void Main_Grid::MakeConnections() {

	// Connection for birght and volume setting
	QSlider::connect(volume_slider_, SIGNAL(valueChanged(int)), video_player_, SLOT(setVolume(int)));
	QMediaPlayer::connect(video_player_, SIGNAL(volumeChanged(int)), volume_slider_, SLOT(setValue(int)));
	QSlider::connect(bright_slider_, SIGNAL(valueChanged(int)), video_widget_, SLOT(setBrightness(int)));
	QVideoWidget::connect(video_widget_, SIGNAL(brightnessChanged(int)), bright_slider_, SLOT(setValue(int)));

	// Connection for switch status for Play/Pause button
	QMediaPlayer::connect(video_player_, SIGNAL(stateChanged(QMediaPlayer::State)), video_player_, SLOT(playStateChanged(QMediaPlayer::State)));
	QPushButton::connect(pause_play_, SIGNAL(clicked()), video_player_, SLOT(pause()));

	// Set for setting fullscreen [Transmit button clicked to setFullScreen]
	QPushButton::connect(full_screen_, SIGNAL(clicked()), this, SLOT(FullScreenClicked()));
	Main_Grid::connect(this, SIGNAL(isFullScreen(bool)), video_widget_, SLOT(setFullScreen(bool)));

	// Set for switching next and front video
	QPushButton::connect(next_video_, SIGNAL(clicked()), video_player_, SLOT(JumpNext()));
	QPushButton::connect(front_video_, SIGNAL(clicked()), video_player_, SLOT(JumpFront()));

	// Set for process bar
	QMediaPlayer::connect(video_player_, SIGNAL(durationChanged(qint64)), process_slider_, SLOT(SetProcessRange(qint64)));
	QMediaPlayer::connect(video_player_, SIGNAL(positionChanged(qint64)), process_slider_, SLOT(SetCurrentPosition(qint64)));
	QSlider::connect(process_slider_, SIGNAL(valueChanged(int)), video_player_, SLOT(SetPosition(int)));
}