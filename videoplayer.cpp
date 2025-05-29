#include "videoplayer.h"
#include <QStyle>
#include <QScreen>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QVideoFrame>

ControlPanel::ControlPanel(QWidget *parent):
    QWidget{parent},
    mouseMoved{false},
    mouseLeftPressed{false},
    mouseLeftReleased{false},
    startMouseLeftPress{std::chrono::steady_clock::now()},
    startMouseLeftRelease{std::chrono::steady_clock::now()},
    startShowChildren{std::chrono::steady_clock::now()},
    positionSlider{Qt::Horizontal},
    playIcon{"icon/play_arrow.svg"},
    pauseIcon{"icon/pause.svg"},
    fullscreenIcon{"icon/fullscreen.svg"},
    exitFullscreenIcon{"icon/fullscreen_exit.svg"}
{
    positionSlider.setRange(0, 0);

    playButton.setEnabled(false);
    playButton.setIcon(playIcon);
    timeLabel.setStyleSheet("border: 5px solid red");
    fullscreenButton.setIcon(fullscreenIcon);

    layout.addWidget(&positionSlider, 1, 0, 1, 3);
    layout.addWidget(&playButton, 2, 0, 1, 1);
    layout.addWidget(&fullscreenButton, 2, 2, 1, 1);
    layout.setRowStretch(0, 1);

    layout.addWidget(&timeLabel, 2, 1, 1, 1);
    layout.setColumnStretch(1, 1);

    setLayout(&layout);
    setMouseTracking(true);
    hideChildren();
}

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget{parent},
    keyLeftPressed{false},
    keyRightPressed{false},
    keySpacePressed{false},
    keySpaceReleased{false},
    startKeyPress{std::chrono::steady_clock::now()}
    // ,graphicsView{&scene}
{
    // scene.addItem(&videoItem);

    connect(&controlPanel.playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);
    connect(&controlPanel.positionSlider, &QAbstractSlider::sliderMoved, this, &VideoPlayer::setPosition);
    connect(&controlPanel.fullscreenButton, &QAbstractButton::clicked, this, &VideoPlayer::toggleFullscreen);

    // layout.addWidget(&graphicsView);
    layout.addWidget(&controlPanel);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    // mediaPlayer.setVideoOutput(&videoItem);
    mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.setVideoSink(&videoSink);
    connect(&mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &VideoPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(&videoSink, &QVideoSink::videoFrameChanged, this, QOverload<>::of(&VideoPlayer::update));

    load(QUrl("test3.mp4"));
    // graphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // graphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // graphicsView.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&VideoPlayer::processEvent));
    timer.start(100);
    setFocusPolicy(Qt::StrongFocus); // for receiving left key and right key event;
    // graphicsView.setStyleSheet("border: 5px solid red");
    play();
}

// VideoPlayer::~VideoPlayer() { }

// QSize VideoPlayer::sizeHint() const
// {
//     return (videoItem.size() * qreal(3) / qreal(2)).toSize();
// }

bool VideoPlayer::isPlayerAvailable() const
{
    return mediaPlayer.isAvailable();
}

void VideoPlayer::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    fileDialog.setDirectory(
        QStandardPaths::standardLocations(
            QStandardPaths::MoviesLocation
        ).value(
            0, QDir::homePath()
        )
    );
    if (fileDialog.exec() == QDialog::Accepted)
        load(fileDialog.selectedUrls().constFirst());
}

void VideoPlayer::load(const QUrl &url)
{
    mediaPlayer.setSource(url);
    controlPanel.playButton.setEnabled(true);
}

void VideoPlayer::play()
{
    switch (mediaPlayer.playbackState())
    {
    case QMediaPlayer::PlayingState:
        mediaPlayer.pause();
        break;
    default:
        mediaPlayer.play();
        break;
    }
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state)
    {
    case QMediaPlayer::PlayingState:
        controlPanel.playButton.setIcon(controlPanel.pauseIcon);
        break;
    default:
        controlPanel.playButton.setIcon(controlPanel.playIcon);
        controlPanel.showChildren();
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    controlPanel.positionSlider.setValue(position);
    bool overAnHour;
    QString text = msToTime(position, overAnHour);
    controlPanel.timeLabel.setText(text + " / " + durationTime);
}

QString VideoPlayer::msToTime(qint64 ms, bool& overAnHour)
{
    /*
    ms = hour * 60 * 60 * 1000 + min * 60 * 1000 + sec * 1000
    int hour = ms / 60 / 60 / 1000;

    (ms - hour * 60 * 60 * 1000) = min * 60 * 1000 + sec * 1000
    int min = (ms - hour * 60 * 60 * 1000) / 60 / 1000;

    (ms - hour * 60 * 60 * 1000 - min * 60 * 1000) = sec * 1000
    int sec = (ms - hour * 60 * 60 * 1000 - min * 60 * 1000) / 1000;
    */
    int hour = ms / 60 / 60 / 1000;
    int min = (ms - hour * 60 * 60 * 1000) / 60 / 1000;
    int sec = (ms - hour * 60 * 60 * 1000 - min * 60 * 1000) / 1000;
    QString time;
    overAnHour = false;
    if (hour >= 1)
    {
        time += QString::number(hour) + ':';
        overAnHour = true;
    }

    time += QString::number(min) + ':';
    time += QString::number(sec);
    return time;
}

void VideoPlayer::durationChanged(qint64 duration)
{
    controlPanel.positionSlider.setRange(0, duration);
    bool overAnHour;
    QString text;
    durationTime = msToTime(duration, overAnHour);
    if (overAnHour)
        text = "0:0:0 / ";
    else
        text = "0:0 / ";

    controlPanel.timeLabel.setText(text + durationTime);
}

void VideoPlayer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

// void VideoPlayer::rotateVideo(int angle)
// {
//     rotate around the center of video element
//     qreal x = videoItem.boundingRect().width() / 2.0;
//     qreal y = videoItem.boundingRect().height() / 2.0;
//     videoItem.setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));
// }

void VideoPlayer::processEvent()
{
    const auto finish = std::chrono::steady_clock::now();

    // mouse move
    const std::chrono::duration<float> showChildrenDuration = finish - controlPanel.startShowChildren;
    if (controlPanel.mouseMoved)
    {
        controlPanel.mouseMoved = false;
        controlPanel.showChildren();
        controlPanel.startShowChildren = std::chrono::steady_clock::now();
    }
    if (showChildrenDuration.count() > showControlPanelDuration && mediaPlayer.playbackState() == QMediaPlayer::PlayingState)
    {
        controlPanel.hideChildren();
    }
    // mouse clicked
    const std::chrono::duration<float> mouseLeftPressDuration = finish - controlPanel.startMouseLeftPress;
    const std::chrono::duration<float> mouseLeftReleaseDuration = finish - controlPanel.startMouseLeftRelease;
    if (controlPanel.mouseLeftReleased && mouseLeftPressDuration.count() < holdTreshold)
    {
        controlPanel.mouseLeftReleased = false;
        play();
        controlPanel.showChildren();
        controlPanel.startShowChildren = std::chrono::steady_clock::now();
    }
    else if (controlPanel.mouseLeftReleased && mouseLeftPressDuration.count() > holdTreshold)
    {
        controlPanel.mouseLeftReleased = false;
        mediaPlayer.setPlaybackRate(1);
    }
    else if (controlPanel.mouseLeftReleased && mouseLeftReleaseDuration.count() - mouseLeftPressDuration.count() < doubleClickDelay)
    {
        controlPanel.mouseLeftReleased = false;
        controlPanel.startMouseLeftRelease = std::chrono::steady_clock::now();
        toggleFullscreen();
    }
    else if (controlPanel.mouseLeftPressed && mouseLeftPressDuration.count() > holdTreshold)
    {
        controlPanel.mouseLeftPressed = false;
        mediaPlayer.setPlaybackRate(2);
    }

    // keyboard
    const std::chrono::duration<float> keyPressDuration = finish - startKeyPress;
    if (keySpaceReleased && keyPressDuration.count() < holdTreshold)
    {
        keySpaceReleased = false;
        play();
        controlPanel.showChildren();
        controlPanel.startShowChildren = std::chrono::steady_clock::now();
    }
    else if (keySpaceReleased && keyPressDuration.count() > holdTreshold)
    {
        keySpaceReleased = false;
        mediaPlayer.setPlaybackRate(1);
    }
    else if (keySpacePressed && keyPressDuration.count() > holdTreshold)
    {
        keySpacePressed = false;
        mediaPlayer.setPlaybackRate(2);
    }
    else if (keyLeftPressed && keyPressDuration.count() > keyRepeatDelay)
    {
        qDebug() << "skip backward\n";
        keyLeftPressed = false;
        controlPanel.positionSlider.setValue(controlPanel.positionSlider.value() - skipDuration);
        setPosition(controlPanel.positionSlider.value());
        controlPanel.showChildren();
        controlPanel.startShowChildren = std::chrono::steady_clock::now();
    }
    else if (keyRightPressed && keyPressDuration.count() > keyRepeatDelay)
    {
        qDebug() << "skip forward\n";
        keyRightPressed = false;
        controlPanel.positionSlider.setValue(controlPanel.positionSlider.value() + skipDuration);
        setPosition(controlPanel.positionSlider.value());
        controlPanel.showChildren();
        controlPanel.startShowChildren = std::chrono::steady_clock::now();
    }
}

// void VideoPlayer::resizeEvent(QResizeEvent*)
// {
//     graphicsView.fitInView(&videoItem, Qt::KeepAspectRatio);
// }

void VideoPlayer::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
    case Qt::Key_Left:
        keyLeftPressed = true;
        startKeyPress = std::chrono::steady_clock::now();
        break;
    case Qt::Key_Right:
        keyRightPressed = true;
        startKeyPress = std::chrono::steady_clock::now();
        break;
    case Qt::Key_Space:
        keySpaceReleased = false;
        keySpacePressed = true;
        startKeyPress = std::chrono::steady_clock::now();
        break;
    }
}

void VideoPlayer::keyReleaseEvent(QKeyEvent* e)
{
    switch (e->key())
    {
    case Qt::Key_Left:
        keyLeftPressed = false;
        break;
    case Qt::Key_Right:
        keyRightPressed = false;
        break;
    case Qt::Key_Space:
        keySpaceReleased = true;
        keySpacePressed = false;
        break;
    }
}

void VideoPlayer::paintEvent(QPaintEvent*)
{
    QPainter painter{this};
    QVideoFrame::PaintOptions paintOptions;
    videoSink.videoFrame().paint(&painter, rect(), paintOptions);
}

void ControlPanel::mouseMoveEvent(QMouseEvent*)
{
    mouseMoved = true;
}

void ControlPanel::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        mouseLeftPressed = true;
        mouseLeftReleased = false;
        startMouseLeftPress = std::chrono::steady_clock::now();
    }
}

void ControlPanel::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        mouseLeftReleased = true;
        mouseLeftPressed = false;
    }
}

void ControlPanel::showChildren()
{
    positionSlider.show();
    playButton.show();
    timeLabel.show();
    fullscreenButton.show();
}

void ControlPanel::hideChildren()
{
    positionSlider.hide();
    playButton.hide();
    timeLabel.hide();
    fullscreenButton.hide();
}

void VideoPlayer::toggleFullscreen()
{
    switch (windowState())
    {
    case Qt::WindowFullScreen:
        setWindowState(beforeFullscreenState);
        controlPanel.fullscreenButton.setIcon(controlPanel.fullscreenIcon);
        break;
    default:
        beforeFullscreenState = windowState();
        setWindowState(Qt::WindowFullScreen);
        controlPanel.fullscreenButton.setIcon(controlPanel.exitFullscreenIcon);
        break;
    }
}