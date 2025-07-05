#include "video_player.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QVideoFrame>
#include <QPainter>
#if defined(Q_OS_WIN)
    #include <windows.h>
    #ifndef GET_X_LPARAM
        #define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
    #endif
    #ifndef GET_Y_LPARAM
        #define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
    #endif
#endif

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget{parent},
    mouseLeftPressed{false},
    mouseRightPressed{false},
    itsALeftClick{false},
    alreadyLeftClickedOnce{false},
    openFileButton{"Open"},
    updateFrameTicksCount{0},
    zoomFactor{1},
    alwaysOnTop{false}
{
    openFileButton.setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    connect(&controlPanel.playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);
    connect(&controlPanel.loopButton, &QAbstractButton::clicked, this, &VideoPlayer::toggleLoop);
    connect(&controlPanel.alwaysOnTopButton, &QAbstractButton::clicked, this, &VideoPlayer::toggleAlwaysOnTop);
    connect(&controlPanel.positionSlider, &QAbstractSlider::valueChanged, this, &VideoPlayer::setPosition);
    connect(&controlPanel.fullscreenButton, &QAbstractButton::clicked, this, &VideoPlayer::toggleFullscreen);
    connect(&openFileButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    layout.addWidget(&openFileButton);
    layout.addWidget(&controlPanel);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    mediaPlayer.setAudioOutput(&audioOutput);
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
    // controlPanel.setStyleSheet("border: 1px solid red");
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

    if (controlPanel.positionSlider.thumbnail.height() < thumbnailHeight)
    {
        controlPanel.positionSlider.thumbnail = frameIndexer.getFrameByTime(
            controlPanel.positionSlider.ms
        );
        controlPanel.positionSlider.updateThumbnailPosition(geometry().topLeft(), width());
        controlPanel.positionSlider.showThumbnail = true;
    }
    else
    {
        controlPanel.positionSlider.updateThumbnailPosition(geometry().topLeft(), width());
        controlPanel.positionSlider.showThumbnail = true;
        controlPanel.positionSlider.thumbnail = frameIndexer.getFrameByTime(
            controlPanel.positionSlider.ms
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
    const int window_width = width();
    const int window_height = height();
    fitWindowRect.setWidth(window_width);
    fitWindowRect.setHeight(window_height);
    int fitWindowRect_new_x;
    int fitWindowRect_new_y;
    int fitWindowRect_new_width;
    int fitWindowRect_new_height;

    // if (window_width / window_height > frame_width / frame_height)
    if (window_width * frame_height > frame_width * window_height)
    {
        /*
        +-----+-----+-----+
        |     |video|     |
        +-----+-----+-----+
        */
        fitWindowRect_new_height = window_height;
        fitWindowRect_new_y = 0;
        fitWindowRect_new_width = fitWindowRect_new_height * frame_width / frame_height;
        fitWindowRect_new_x = (window_width - fitWindowRect_new_width) / 2;
    }
    // if (window_width / window_height < frame_width / frame_height)
    else if (window_width * frame_height < frame_width * window_height)
    {
        /*
        +-----+
        |     |
        +-----+
        |video|
        +-----+
        |     |
        +-----+
        */
        fitWindowRect_new_width = window_width;
        fitWindowRect_new_x = 0;
        fitWindowRect_new_height = fitWindowRect_new_width * frame_height / frame_width;
        fitWindowRect_new_y = (window_height - fitWindowRect_new_height) / 2;
    }
    else
    {
        fitWindowRect_new_x = 0;
        fitWindowRect_new_y = 0;
        fitWindowRect_new_width = window_width;
        fitWindowRect_new_height = window_height;
    }
    fitFrameRect.setTopLeft({fitWindowRect_new_x, fitWindowRect_new_y});
    fitFrameRect.setSize({fitWindowRect_new_width, fitWindowRect_new_height});

    frameRect = fitFrameRect;
    zoomFactor = 1;
}

void VideoPlayer::paintEvent(QPaintEvent*)
{
    QPainter painter{this};

    QVideoFrame::PaintOptions paintOptions;
    videoSink.videoFrame().paint(&painter, frameRect, paintOptions);
    // painter.drawRect(0, 0, width(), height());
    // painter.drawRect(fitFrameRect);
    // painter.drawRect(frameRect);

    if (zoomFactor > 1)
    {
        const float factor = static_cast<float>(thumbnailHeight) / fitWindowRect.height(); // use scale that makes fitWindowrect has same height with thumbnail
        constexpr int margin = 10;

        int fitFrameRectYStop = fitFrameRect.y() * factor + margin
            + fitFrameRect.height() * factor;
        int frameRectYStop = -frameRect.y() * factor / zoomFactor + fitFrameRect.y() * factor + margin
            + height() * factor / zoomFactor;
        int yStop = max(fitFrameRectYStop, frameRectYStop) + margin;

        QLinearGradient gradient {0, 0, 0, static_cast<qreal>(yStop)};
        gradient.setColorAt(0, {0, 0, 0, 255});
        gradient.setColorAt(1, {0, 0, 0, 0});

        painter.setPen({0, 0, 0, 0});
        painter.setBrush(gradient);
        painter.drawRect(0, 0, width(), height() / 2);

        // static
        // painter.setPen(QColor(85, 85, 85));
        painter.setPen({255, 255, 255, 255 / 3});
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(
            fitFrameRect.x() * factor + margin,
            fitFrameRect.y() * factor + margin,
            fitFrameRect.width() * factor,
            fitFrameRect.height() * factor
        );

        // dynamic, changed when zoom scale is changed
        // painter.setPen(QColor(170, 170, 170));
        painter.setPen({255, 255, 255, 255 / 2});
        painter.drawRect(
            -frameRect.x() * factor / zoomFactor + fitFrameRect.x() * factor + margin,
            -frameRect.y() * factor / zoomFactor + fitFrameRect.y() * factor + margin,
            width() * factor / zoomFactor,
            height() * factor / zoomFactor
        );

    }
    if (controlPanel.isVisible())
    {
        QLinearGradient gradient
        {
            0,
            static_cast<qreal>(height()),
            0,
            static_cast<qreal>(height() - controlPanel.positionSlider.height() * 2 - thumbnailHeight)
        };
        gradient.setColorAt(0, {0, 0, 0, 255});
        gradient.setColorAt(1, {0, 0, 0, 0});

        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawRect(0, height() / 2, width(), height() / 2);
    }
    if (controlPanel.positionSlider.showThumbnail)
    {
        controlPanel.positionSlider.drawThumbnail(painter);
    }
}

void VideoPlayer::leaveEvent(QEvent*)
{
    controlPanel.hide();
}

void VideoPlayer::showEvent(QShowEvent*)
{
    // qDebug() << "frame before   : " << frameRect;
    fitAndCenterFrameRect();
    // qDebug() << "frame after    : " << frameRect;
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

        else if (dx < 0 && frameRect.right() < width()) // move left
            frameRect.translate(width() - frameRect.right(), 0);

        if (dy > 0 && frameRect.top() > 0) // move bottom
            frameRect.translate(0, -frameRect.top());

        else if (dy < 0 && frameRect.bottom() < height()) // move top
            frameRect.translate(0, height() - frameRect.bottom());

        oldMousePosition = e->position();
    }
    if (mediaPlayer.playbackState() == QMediaPlayer::PausedState)
        update();
}

void VideoPlayer::wheelEvent(QWheelEvent* e)
{
    QPoint numDegrees = e->angleDelta() / 8;

    if (!numDegrees.isNull() && mouseRightPressed)
    {
        QPoint numSteps = numDegrees / 15; // (0, 1) or (0, -1)
        numSteps *= 1;
        const float oldZoomFactor = zoomFactor;

        if (numSteps.y() < 0)
            zoomFactor -= 0.1;
        else if (numSteps.y() > 0)
            zoomFactor += 0.1;

        zoomFrameRect(e->position(), oldZoomFactor);

        if (zoomFactor < 1) // bounded so zoom scale >= 100%
        {
            // zoomFactor = 1;
            fitAndCenterFrameRect();
        }
    }
    qDebug() << zoomFactor;
}

void VideoPlayer::zoomFrameRect(const QPointF& anchor, float oldZoomFactor)
{
    int dx = anchor.x() - frameRect.x();
    int dy = anchor.y() - frameRect.y();

    int frameRect_new_width = fitFrameRect.width() * zoomFactor;
    int frameRect_new_height = fitFrameRect.height() * zoomFactor;
    int frameRect_new_x = anchor.x() - dx * zoomFactor / oldZoomFactor;
    int frameRect_new_y = anchor.y() - dy * zoomFactor / oldZoomFactor;

    frameRect.setTopLeft({frameRect_new_x, frameRect_new_y});
    frameRect.setSize({frameRect_new_width, frameRect_new_height});

    if (mediaPlayer.playbackState() == QMediaPlayer::PausedState)
        update();
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

void VideoPlayer::mouseLeftReleased()
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

void VideoPlayer::mouseRightReleased()
{
    mouseRightPressed = false;
    setCursor(Qt::ArrowCursor);
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        mouseLeftReleased();
    }
    if (e->button() == Qt::RightButton)
    {
        mouseRightReleased();
    }
}

// bool VideoPlayer::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
// {
// #if defined(Q_OS_WIN)
//     MSG* msg = static_cast<MSG*>(message);
//     switch (msg->message)
//     {
//     // case WM_LBUTTONDOWN:
//     //     qDebug() << "Native WM_LBUTTONDOWN";
//     //     return true;
//     case WM_LBUTTONUP:
//         mouseLeftReleased();
//         return true;
//     // case WM_RBUTTONDOWN:
//     //     mouseRightPressed = true;
//     //     setCursor(Qt::ClosedHandCursor);
//     //     oldMousePosition = {
//     //         static_cast<qreal>(GET_X_LPARAM(msg->lParam)),
//     //         static_cast<qreal>(GET_Y_LPARAM(msg->lParam))
//     //     };
//     //     qDebug() << oldMousePosition;
//     //     return false;
//     case WM_RBUTTONUP:
//         mouseRightReleased();
//         return true;
//     default:
//         break;
//     }
//     // return false;
// #endif
//     return QWidget::nativeEvent(eventType, message, result);
// }

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

void VideoPlayer::toggleLoop()
{
    switch (mediaPlayer.loops())
    {
    case QMediaPlayer::Once:
        mediaPlayer.setLoops(QMediaPlayer::Infinite);
        controlPanel.loopButton.setIcon(controlPanel.loopAcvtiveIcon);
        break;
    case QMediaPlayer::Infinite:
        mediaPlayer.setLoops(QMediaPlayer::Once);
        controlPanel.loopButton.setIcon(controlPanel.loopInactiveIcon);
        break;
    }
}

void VideoPlayer::toggleAlwaysOnTop()
{
    if (alwaysOnTop)
    {
        alwaysOnTop = false;
        controlPanel.alwaysOnTopButton.setIcon(controlPanel.alwaysOnTopInactiveIcon);
        setWindowFlags(windowFlags() &= ~Qt::WindowStaysOnTopHint);
        show();
    }
    else
    {
        alwaysOnTop = true;
        controlPanel.alwaysOnTopButton.setIcon(controlPanel.alwaysOnTopActiveIcon);
        setWindowFlags(windowFlags() |= Qt::WindowStaysOnTopHint);
        show();
    }
}