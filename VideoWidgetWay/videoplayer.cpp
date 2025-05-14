#include "videoplayer.h"

#include <QFileDialog>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QString>
#include <QStyle>

VideoPlayer::Core::Core(QWidget *parent):
    QWidget(parent),
    positionSlider(Qt::Horizontal, this),
    playButton(this),
    layout(),
    mediaPlayer(),
    videoWidget(parent),
    audioOutput(),
    errorLabel(parent)
{
    positionSlider.setRange(0, 0);
    connect(&positionSlider, &QAbstractSlider::sliderMoved, this, &Core::setPosition);

    playButton.setEnabled(false);
    playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(&playButton, &QAbstractButton::clicked, this, &Core::play);

    errorLabel.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    layout.addWidget(&positionSlider, 0, 0, 1, 5);
    layout.addWidget(&playButton, 1, 2, 1, 1);
    setLayout(&layout);

    mediaPlayer.setVideoOutput(&videoWidget);
    connect(&mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &Core::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &Core::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &Core::durationChanged);
    connect(&mediaPlayer, &QMediaPlayer::errorChanged, this, &Core::handleError);
    setUrl(QUrl("../test.mp4"));
    mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.play();
};

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget(parent),
    layout(),
    core(this)
{
    // QAbstractButton *openButton = new QPushButton(tr("Open..."));
    // connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);


    // controlLayout->setContentsMargins(0, 0, 0, 0);
    // // controlLayout->addWidget(openButton);

    // QBoxLayout *layout = new QVBoxLayout;
    // layout->addLayout(controlLayout);
    // layout->addWidget(m_errorLabel);

    // controlLayout.addWidget(&videoWidget);
    layout.addWidget(&core.errorLabel);
    layout.addWidget(&core);
    layout.addWidget(&core.videoWidget);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    // videoWidget.lower();

}

VideoPlayer::~VideoPlayer() { }

void VideoPlayer::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation)
    .value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        core.setUrl(fileDialog.selectedUrls().constFirst());
}

void VideoPlayer::Core::setUrl(const QUrl &url)
{
    errorLabel.setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mediaPlayer.setSource(url);
    playButton.setEnabled(true);
}

void VideoPlayer::setUrl(const QUrl &url)
{
    core.setUrl(url);
}

void VideoPlayer::Core::play()
{
    switch (mediaPlayer.playbackState()) {
        case QMediaPlayer::PlayingState:
            mediaPlayer.pause();
            break;
        default:
            mediaPlayer.play();
            break;
    }
}

void VideoPlayer::Core::mediaStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state) {
        case QMediaPlayer::PlayingState:
            playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        default:
            playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
    }
}

void VideoPlayer::Core::positionChanged(qint64 position)
{
    positionSlider.setValue(position);
}

void VideoPlayer::Core::durationChanged(qint64 duration)
{
    positionSlider.setRange(0, duration);
}

void VideoPlayer::Core::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void VideoPlayer::Core::handleError()
{
    if (mediaPlayer.error() == QMediaPlayer::NoError)
        return;

    playButton.setEnabled(false);
    const QString errorString = mediaPlayer.errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(mediaPlayer.error()));
    else
        message += errorString;
    errorLabel.setText(message);
}

// #include "moc_videoplayer.cpp"