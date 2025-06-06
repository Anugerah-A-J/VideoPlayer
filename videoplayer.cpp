#include "videoplayer.h"
#include <QStyle>
#include <QScreen>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QVideoFrame>
#include <QToolTip>
#include <iostream>

PositionSlider::PositionSlider(Qt::Orientation orientation, QWidget *parent):
    QSlider{orientation, parent},
    mouseIsInsideMe{false},
    timeLabel{nullptr, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint}, // without title bar and always on top
    timeText{""}
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
}

void PositionSlider::mouseMoveEvent(QMouseEvent* e)
{
    bool b;
    ms = e->position().x() / width() * maximum();

    QString newTimeText = VideoPlayer::msToTime(ms, b);

    QPoint position {
        static_cast<int>(e->position().x()),
        -height()
    };

    if (timeText != newTimeText)
    {
        timeText = newTimeText;
        timeLabel.setText(newTimeText);
        emit timeTextChanged(ms, mapToGlobal(position) + QPoint(0, -timeLabel.height()));
    }
    timeLabel.setFixedWidth(timeLabel.sizeHint().width());
    position.setX(position.x() - timeLabel.size().width() / 2);
    timeLabel.move(mapToGlobal(position));
    timeLabel.show();
}

void PositionSlider::mousePressEvent(QMouseEvent*)
{}

void PositionSlider::mouseReleaseEvent(QMouseEvent*)
{
    setValue(ms);
}

void PositionSlider::enterEvent(QEnterEvent*)
{
    mouseIsInsideMe = true;
}

void PositionSlider::leaveEvent(QEvent*)
{
    mouseIsInsideMe = false;
    timeLabel.hide();
}

ControlPanel::ControlPanel(QWidget *parent):
    QWidget{parent},
    mouseLeftPressed{false},
    itsALeftClick{false},
    alreadyLeftClickedOnce{false},
    positionSlider{Qt::Horizontal},
    playIcon{"icon/play_arrow.svg"},
    pauseIcon{"icon/pause.svg"},
    fullscreenIcon{"icon/fullscreen.svg"},
    exitFullscreenIcon{"icon/fullscreen_exit.svg"}
{
    playButton.setEnabled(false);
    playButton.setIcon(playIcon);
    playButton.setCursor(Qt::PointingHandCursor);
    // playButton.setAttribute(Qt::WA_TranslucentBackground); this doesn't work

    fullscreenButton.setIcon(fullscreenIcon);
    fullscreenButton.setCursor(Qt::PointingHandCursor);
    // fullscreenButton.setAttribute(Qt::WA_TranslucentBackground); this doesn't work

    layout.addWidget(&positionSlider, 1, 0, 1, 3);
    layout.addWidget(&playButton, 2, 0, 1, 1);
    layout.addWidget(&fullscreenButton, 2, 2, 1, 1);
    layout.setRowStretch(0, 1);

    layout.addWidget(&timeLabel, 2, 1, 1, 1);
    layout.setColumnStretch(1, 1);

    setLayout(&layout);
    setMouseTracking(true);
    // setStyleSheet("border: 1px solid red");
}

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget{parent},
    openFileButton{"Open"},
    // thumbnailLabel{nullptr, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint} // without title bar and always on top
    thumbnailLabel{nullptr, Qt::Window | Qt::FramelessWindowHint} // without title bar
    // keySpacePressed{false},
    // startKeySpacePress{std::chrono::steady_clock::now()}
    // ,graphicsView{&scene}
{
    // scene.addItem(&videoItem);
    openFileButton.setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    connect(&controlPanel.playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);
    connect(&controlPanel.positionSlider, &QAbstractSlider::valueChanged, this, &VideoPlayer::setPosition);
    connect(&controlPanel.fullscreenButton, &QAbstractButton::clicked, this, &VideoPlayer::toggleFullscreen);
    connect(&openFileButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    // layout.addWidget(&graphicsView);
    layout.addWidget(&openFileButton);
    layout.addWidget(&controlPanel);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    // mediaPlayer.setVideoOutput(&videoItem);
    mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.setVideoSink(&videoSink);
    connect(&mediaPlayer, &QMediaPlayer::errorOccurred, this, &VideoPlayer::printError);
    connect(&mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &VideoPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(&videoSink, &QVideoSink::videoFrameChanged, this, QOverload<>::of(&VideoPlayer::update));

    // graphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // graphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // graphicsView.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&VideoPlayer::timeEvent));
    timer.start(100);
    controlPanel.hide();
    resize(640, 480);
    setMouseTracking(true);
    connect(&controlPanel.positionSlider, &PositionSlider::timeTextChanged, this, &VideoPlayer::updateThumbnail);
    // graphicsView.setStyleSheet("border: 5px solid red");
}

void VideoPlayer::printError(QMediaPlayer::Error error, const QString &errorString)
{
    qDebug() << error ;
    qDebug() << errorString;
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
    {
        load(fileDialog.selectedUrls().constFirst());
        play();
        openFileButton.hide();
        controlPanel.show();
    }
}

void VideoPlayer::run()
{
    play();
    openFileButton.hide();
    controlPanel.show();
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
        controlPanel.show();
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    controlPanel.positionSlider.blockSignals(true);
    controlPanel.positionSlider.setValue(position);
    controlPanel.positionSlider.blockSignals(false);
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

        if (min < 10)
            time += '0';
        time += QString::number(min) + ':';
    }
    else
    {
        time += QString::number(min) + ':';
    }

    if (sec < 10)
        time += '0';
    time += QString::number(sec);

    return time;
}

void VideoPlayer::updateThumbnail(const qint64& ms, const QPoint& cursorGlobalPosition)
{
    int oldPosition = mediaPlayer.position();
    setPosition(ms); // not efficient
    QImage img = videoSink.videoFrame().toImage();
    setPosition(oldPosition); // not efficient

    QPixmap px = QPixmap::fromImage(img);
    thumbnailLabel.setPixmap(px.scaledToHeight(thumbnailHeight));
    // thumbnailLabel.move(cursorGlobalPosition + QPoint(-px.width() / 2, 0));
    thumbnailLabel.move(cursorGlobalPosition * 0);
    thumbnailLabel.show();
}

void VideoPlayer::durationChanged(qint64 duration)
{
    controlPanel.positionSlider.setRange(0, duration);
    bool overAnHour;
    QString text;
    durationTime = msToTime(duration, overAnHour);
    if (overAnHour)
        text = "0:00:00 / ";
    else
        text = "0:00 / ";

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

void VideoPlayer::timeEvent()
{
    if (openFileButton.isVisible())
        return;

    if (!controlPanel.positionSlider.mouseIsInsideMe)
        thumbnailLabel.hide();

    const auto finish = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration;

    duration = finish - controlPanel.startShowChildren;
    if (
        duration.count() > showControlPanelDuration &&
        mediaPlayer.playbackState() == QMediaPlayer::PlayingState &&
        !controlPanel.positionSlider.mouseIsInsideMe
    )
        controlPanel.hide();

    duration = finish - controlPanel.startMouseLeftPress;
    if (duration.count() > holdTreshold && controlPanel.mouseLeftPressed)
    {
        controlPanel.itsALeftClick = false;
        mediaPlayer.setPlaybackRate(2);
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
    case Qt::Key_Space:
        if (!e->isAutoRepeat())
            keySpacePressIsAutoRepeat = false;
        else
        {
            keySpacePressIsAutoRepeat = true;
            mediaPlayer.setPlaybackRate(2);
        }
        break;
    }
}

void VideoPlayer::keyReleaseEvent(QKeyEvent* e)
{
    switch (e->key())
    {
    case Qt::Key_Left:
        controlPanel.positionSlider.setValue(controlPanel.positionSlider.value() - skipDuration);
        controlPanel.show();
        controlPanel.startShowChildren = std::chrono::steady_clock::now();
        break;
    case Qt::Key_Right:
        controlPanel.positionSlider.setValue(controlPanel.positionSlider.value() + skipDuration);
        controlPanel.show();
        controlPanel.startShowChildren = std::chrono::steady_clock::now();
        break;
    case Qt::Key_Space:
        if (!e->isAutoRepeat() && !keySpacePressIsAutoRepeat)
        {
            play();
            controlPanel.show();
            controlPanel.startShowChildren = std::chrono::steady_clock::now();
        }
        else if (!e->isAutoRepeat() && keySpacePressIsAutoRepeat)
            mediaPlayer.setPlaybackRate(1);
        break;
    }
}

void VideoPlayer::paintEvent(QPaintEvent*)
{
    QPainter painter{this};
    QVideoFrame::PaintOptions paintOptions;
    videoSink.videoFrame().paint(&painter, rect(), paintOptions);
}

void VideoPlayer::leaveEvent(QEvent*)
{
    controlPanel.hide();
}

// void VideoPlayer::showEvent(QShowEvent*)
// {
// }

void VideoPlayer::mouseMoveEvent(QMouseEvent*)
{
    controlPanel.show();
    controlPanel.startShowChildren = std::chrono::steady_clock::now();
}

void ControlPanel::mouseMoveEvent(QMouseEvent* e)
{
    static_cast<VideoPlayer*>(parentWidget())->mouseMoveEvent(e);
}

void VideoPlayer::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        controlPanel.startMouseLeftPress = std::chrono::steady_clock::now();
        controlPanel.mouseLeftPressed = true;
    }
}

void ControlPanel::mousePressEvent(QMouseEvent* e)
{
    static_cast<VideoPlayer*>(parentWidget())->mousePressEvent(e);
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        // if you use the commented one, the time is inaccurate
        // const auto duration = (std::chrono::steady_clock::now() - controlPanel.startMouseLeftPress).count();
        const auto finish = std::chrono::steady_clock::now();
        std::chrono::duration<float> clickDuration{finish - controlPanel.startMouseLeftPress};
        std::chrono::duration<float> clickInterval{controlPanel.startMouseLeftPress - controlPanel.startMouseLeftRelease};

        // std::cout << duration << " s\n";
        std::cout << clickDuration.count() << " s\n";

        controlPanel.mouseLeftPressed = false;

        // if (duration <= controlPanel.holdTreshold)
        if (clickDuration.count() <= holdTreshold)
        {
            controlPanel.show();
            controlPanel.startShowChildren = std::chrono::steady_clock::now();
            play();

            if (controlPanel.itsALeftClick && clickInterval.count() < doubleClickDelay && controlPanel.alreadyLeftClickedOnce)
                toggleFullscreen();

            controlPanel.itsALeftClick = true;
        }
        // else
        // {
            mediaPlayer.setPlaybackRate(1);
        // }

        if (!controlPanel.alreadyLeftClickedOnce)
            controlPanel.alreadyLeftClickedOnce = true;
        controlPanel.startMouseLeftRelease = finish;
    }
}

void ControlPanel::mouseReleaseEvent(QMouseEvent* e)
{
    static_cast<VideoPlayer*>(parentWidget())->mouseReleaseEvent(e);
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