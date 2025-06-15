#include "video_player.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QVideoFrame>
#include <QPainter>
#include <iostream>

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget{parent},
    mouseLeftPressed{false},
    mouseRightPressed{false},
    itsALeftClick{false},
    alreadyLeftClickedOnce{false},
    openFileButton{"Open"},
    updateFrameTicksCount{0},
    zoomFactor{1}
{
    openFileButton.setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    connect(&controlPanel.playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);
    connect(&controlPanel.positionSlider, &QAbstractSlider::valueChanged, this, &VideoPlayer::setPosition);
    connect(&controlPanel.fullscreenButton, &QAbstractButton::clicked, this, &VideoPlayer::toggleFullscreen);
    connect(&openFileButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    layout.addWidget(&openFileButton);
    layout.addWidget(&controlPanel);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    // mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.setVideoSink(&videoSink);
    connect(&mediaPlayer, &QMediaPlayer::errorOccurred, this, &VideoPlayer::printError);
    connect(&mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &VideoPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(&videoSink, &QVideoSink::videoFrameChanged, this, QOverload<>::of(&VideoPlayer::update));

    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&VideoPlayer::timeEvent));
    timer.start(timeEventInterval);
    controlPanel.hide();
    setCursor(Qt::BlankCursor);
    resize(initialWidth, initialHeight);
    setMouseTracking(true);
    // connect(&controlPanel.positionSlider, &PositionSlider::timeTextChanged, this, &VideoPlayer::updateThumbnail);
    // graphicsView.setStyleSheet("border: 5px solid red");
}

void VideoPlayer::printError(QMediaPlayer::Error error, const QString &errorString)
{
    qDebug() << error;
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
        setCursor(Qt::ArrowCursor);
    }
}

void VideoPlayer::run()
{
    play();
    openFileButton.hide();
    controlPanel.show();
    setCursor(Qt::ArrowCursor);
}

void VideoPlayer::load(const QUrl &url)
{
    mediaPlayer.setSource(url);
    frameIndexer.load(url, frame_width, frame_height);
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
        setCursor(Qt::ArrowCursor);
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
    controlPanel.timeOverDurationLabel.setText(text + " / " + durationTime);
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

void VideoPlayer::updateThumbnail()
{
    // auto start{std::chrono::steady_clock::now()};
    // ....
    // auto finish{std::chrono::steady_clock::now()};
    // std::chrono::duration<double> elapsed_seconds{finish - start};
    // std::cout << "ffms get frame: " << elapsed_seconds.count() << '\n';

    if (controlPanel.positionSlider.thumbnail.pictureLabel.height() < thumbnailHeight)
    {
        controlPanel.positionSlider.thumbnail.pictureLabel.setPixmap(
            frameIndexer.getFrameByTime(
                controlPanel.positionSlider.ms
            )
        );
        controlPanel.positionSlider.thumbnail.move(controlPanel.positionSlider.cursorGlobalPositionOnTopOfSlider + QPoint(
            -controlPanel.positionSlider.thumbnail.width() / 2,
            -controlPanel.positionSlider.thumbnail.height()
        ));
        controlPanel.positionSlider.thumbnail.show();
    }
    else
    {
        controlPanel.positionSlider.thumbnail.move(controlPanel.positionSlider.cursorGlobalPositionOnTopOfSlider + QPoint(
            -controlPanel.positionSlider.thumbnail.width() / 2,
            -controlPanel.positionSlider.thumbnail.height()
        ));
        controlPanel.positionSlider.thumbnail.show();
        controlPanel.positionSlider.thumbnail.pictureLabel.setPixmap(
            frameIndexer.getFrameByTime(
                controlPanel.positionSlider.ms
            )
        );
    }
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

    controlPanel.timeOverDurationLabel.setText(text + durationTime);
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

    const auto finish = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration;

    duration = finish - startShowChildren;
    if (
        duration.count() > showControlPanelDuration &&
        mediaPlayer.playbackState() == QMediaPlayer::PlayingState &&
        !controlPanel.positionSlider.mouseIsInsideMe
    )
    {
        controlPanel.hide();
        setCursor(Qt::BlankCursor);
    }

    duration = finish - startMouseLeftPress;
    if (duration.count() > holdTreshold && mouseLeftPressed)
    {
        itsALeftClick = false;
        mediaPlayer.setPlaybackRate(2);
    }

    if (controlPanel.positionSlider.mouseIsInsideMe && controlPanel.positionSlider.timeTextChanged && updateFrameTicksCount == updateFrameInterval / timeEventInterval)
    {
        updateThumbnail();

        controlPanel.positionSlider.timeTextChanged = false;
        updateFrameTicksCount = 0;
    }

    if (updateFrameTicksCount != updateFrameInterval / timeEventInterval)
        updateFrameTicksCount++;
}

void VideoPlayer::resizeEvent(QResizeEvent*)
{
    fitAndCenterFrameRect();
}

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
        setCursor(Qt::ArrowCursor);
        startShowChildren = std::chrono::steady_clock::now();
        break;
    case Qt::Key_Right:
        controlPanel.positionSlider.setValue(controlPanel.positionSlider.value() + skipDuration);
        controlPanel.show();
        setCursor(Qt::ArrowCursor);
        startShowChildren = std::chrono::steady_clock::now();
        break;
    case Qt::Key_Space:
        if (!e->isAutoRepeat() && !keySpacePressIsAutoRepeat)
        {
            play();
            controlPanel.show();
            setCursor(Qt::ArrowCursor);
            startShowChildren = std::chrono::steady_clock::now();
        }
        else if (!e->isAutoRepeat() && keySpacePressIsAutoRepeat)
            mediaPlayer.setPlaybackRate(1);
        break;
    }
}

void VideoPlayer::fitAndCenterFrameRect()
{
    int window_width = geometry().width();
    int window_height = geometry().height();

    if (window_width * frame_height > frame_width * window_height) // window too wide
    {
        frameRect.setY(0);
        frameRect.setHeight(window_height);
        frameRect.setWidth(frameRect.height() * frame_width / frame_height);
        frameRect.setX((window_width - frameRect.width()) / 2);
    }
    else if (window_width * frame_height < frame_width * window_height)
    {
        frameRect.setX(0);
        frameRect.setWidth(window_width);
        frameRect.setHeight(frameRect.width() * frame_height / frame_width);
        frameRect.setY((window_height - frameRect.height()) / 2);
    }
    else
    {
        frameRect.setX(0);
        frameRect.setY(0);
        frameRect.setWidth(window_width);
        frameRect.setHeight(window_height);
    }
}

void VideoPlayer::paintEvent(QPaintEvent*)
{
    QPainter painter{this};

    QVideoFrame::PaintOptions paintOptions;
    videoSink.videoFrame().paint(&painter, frameRect, paintOptions);

    // if (zoomFactor > 1)
    {
        painter.setPen(QColor(127, 127, 127));

        float factor = static_cast<float>(thumbnailHeight) / frameRect.height();
        QPointF anchor = mapFromGlobal(geometry().topLeft());
        painter.setPen(QColor(85, 85, 85));
        painter.drawRect(
            10 + anchor.x(),
            10 + anchor.y(),
            frameRect.width() * factor,
            thumbnailHeight
        );

        factor = static_cast<float>(thumbnailHeight) / geometry().height();
        anchor += frameRect.topLeft();
        painter.setPen(QColor(170, 170, 170));
        painter.drawRect(
            anchor.x(),
            anchor.y(),
            geometry().width() * factor,
            geometry().height() * factor
        );
    }
}

void VideoPlayer::leaveEvent(QEvent*)
{
    controlPanel.hide();
}

void VideoPlayer::showEvent(QShowEvent*)
{
    // qDebug() << "geometry before: " << geometry();
    qDebug() << "frame before   : " << frameRect;
    fitAndCenterFrameRect();
    // qDebug() << "geometry after : " << geometry();
    qDebug() << "frame after    : " << frameRect;
}

void VideoPlayer::mouseMoveEvent(QMouseEvent* e)
{
    if (!mouseRightPressed)
    {
        controlPanel.show();
        setCursor(Qt::ArrowCursor);
        startShowChildren = std::chrono::steady_clock::now();
    }
    else if (zoomFactor > 1)
    {
        int dx = e->position().x() - oldMousePosition.x();
        int dy = e->position().y() - oldMousePosition.y();
        frameRect.translate(dx, dy);

        if (dx > 0 && frameRect.left() > 0) // move right
            frameRect.translate(-frameRect.left(), 0);

        else if (dx < 0 && frameRect.right() < geometry().right()) // move left
            frameRect.translate(geometry().right() - frameRect.right(), 0);

        if (dy > 0 && frameRect.top() > 0) // move bottom
            frameRect.translate(0, -frameRect.top());

        else if (dy < 0 && frameRect.bottom() < geometry().bottom()) // move top
            frameRect.translate(0, geometry().bottom() - frameRect.bottom());

        oldMousePosition = e->position();
    }
}

void VideoPlayer::wheelEvent(QWheelEvent* e)
{
    QPoint numDegrees = e->angleDelta() / 8;

    if (!numDegrees.isNull() && mouseRightPressed)
    {
        QPoint numSteps = numDegrees / 15; // (0, 1) or (0, -1)
        numSteps *= 1;

        if (numSteps.y() < 0)
            zoomFactor -= 0.1;
        else if (numSteps.y() > 0)
            zoomFactor += 0.1;

        zoomFrameRect(e->position(), zoomFactor);

        if (zoomFactor < 1)
        {
            zoomFactor = 1;
            fitAndCenterFrameRect();
        }
    }
}

void VideoPlayer::zoomFrameRect(const QPointF& anchor, float zoomFactor) // bounded so zoom scale >= 100%
{
    int dx = anchor.x() - frameRect.x();
    int dy = anchor.y() - frameRect.y();

    frameRect.setX(anchor.x() - dx * zoomFactor);
    frameRect.setY(anchor.y() - dy * zoomFactor);

    frameRect.setWidth(frameRect.width() * zoomFactor);
    frameRect.setHeight(frameRect.height() * zoomFactor);
}

void VideoPlayer::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        startMouseLeftPress = std::chrono::steady_clock::now();
        mouseLeftPressed = true;
    }
    else if (e->button() == Qt::RightButton)
    {
        mouseRightPressed = true;
        setCursor(Qt::ClosedHandCursor);
        oldMousePosition = e->position();
    }
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        const auto finish = std::chrono::steady_clock::now();
        std::chrono::duration<float> clickDuration{finish - startMouseLeftPress};
        std::chrono::duration<float> clickInterval{startMouseLeftPress - startMouseLeftRelease};

        // std::cout << clickDuration.count() << " s\n";

        mouseLeftPressed = false;

        if (clickDuration.count() <= holdTreshold)
        {
            controlPanel.show();
            setCursor(Qt::ArrowCursor);
            startShowChildren = std::chrono::steady_clock::now();
            play();

            if (itsALeftClick && clickInterval.count() < doubleClickDelay && alreadyLeftClickedOnce)
                toggleFullscreen();

            itsALeftClick = true;
        }

        mediaPlayer.setPlaybackRate(1);

        if (!alreadyLeftClickedOnce)
            alreadyLeftClickedOnce = true;
        startMouseLeftRelease = finish;
    }
    if (e->button() == Qt::RightButton)
    {
        mouseRightPressed = false;
        setCursor(Qt::ArrowCursor);
    }
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