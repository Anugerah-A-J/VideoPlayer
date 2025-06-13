#include "video_player.h"
// #include <QStyle>
// #include <QScreen>
#include <QFileDialog>
#include <QStandardPaths>
// #include <QMouseEvent>
#include <QVideoFrame>
#include <QPainter>
#include <iostream>
#include <qpixmap.h>

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget{parent},
    openFileButton{"Open"},
    updateFrameTicksCount{0}
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
    // mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.setVideoSink(&videoSink);
    connect(&mediaPlayer, &QMediaPlayer::errorOccurred, this, &VideoPlayer::printError);
    connect(&mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &VideoPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(&videoSink, &QVideoSink::videoFrameChanged, this, QOverload<>::of(&VideoPlayer::update));
    thumbnailMediaPlayer.setVideoSink(&thumbnailVideoSink);
    connect(&thumbnailVideoSink, &QVideoSink::videoFrameChanged, this, QOverload<>::of(&VideoPlayer::updateThumbnail));

    // graphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // graphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // graphicsView.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&VideoPlayer::timeEvent));
    timer.start(timeEventInterval);
    controlPanel.hide();
    resize(640, 480);
    setMouseTracking(true);
    // connect(&controlPanel.positionSlider, &PositionSlider::timeTextChanged, this, &VideoPlayer::updateThumbnail);
    // thumbnailMediaPlayer.setVideoSink(&thumbnailVideoSink);
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
        // thumbnailMediaPlayer.play();
        // thumbnailMediaPlayer.pause();
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
    // frameIndexer.load(url);
    thumbnailMediaPlayer.setSource(url);
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

// void VideoPlayer::updateThumbnail(const qint64& ms, const QPoint& cursorGlobalPositionOnTopOfSlider)
// {
    // const FFMS_Frame* frame = frameIndexer.getFrameByTime(ms * 0.001);
    // int i = 0;
    // QImage img = QImage(frame->Data[i], frame->ScaledWidth, frame->ScaledHeight, frame->Linesize[i], QImage::Format_RGBA8888);
    // QPixmap px = QPixmap::fromImage(img).scaledToHeight(thumbnailHeight);
    // controlPanel.positionSlider.thumbnail.pictureLabel.setPixmap(px);
    // controlPanel.positionSlider.thumbnail.move(cursorGlobalPositionOnTopOfSlider + QPoint(
    //     -controlPanel.positionSlider.thumbnail.width() / 2,
    //     -controlPanel.positionSlider.thumbnail.height()
    // ));
    // controlPanel.positionSlider.thumbnail.show();
// }

void VideoPlayer::updateThumbnail()
{
    // auto start{std::chrono::steady_clock::now()};

    // const FFMS_Frame* frame = frameIndexer.getFrameByTime(
    //     controlPanel.positionSlider.ms * 0.001
    // );

    // auto finish{std::chrono::steady_clock::now()};
    // std::chrono::duration<double> elapsed_seconds{finish - start};
    // std::cout << "ffms get frame: " << elapsed_seconds.count() << '\n';

    // int i = 0;
    // start = std::chrono::steady_clock::now();

    // QImage img = QImage(frame->Data[i], frame->ScaledWidth, frame->ScaledHeight, frame->Linesize[i], QImage::Format_RGBA8888);

    // finish = std::chrono::steady_clock::now();
    // elapsed_seconds = finish - start;
    // std::cout << "making qimage: " << elapsed_seconds.count() << '\n';

    // QPixmap px = QPixmap::fromImage(img).scaledToHeight(thumbnailHeight);
    // controlPanel.positionSlider.thumbnail.pictureLabel.setPixmap(px);
    if (controlPanel.positionSlider.thumbnail.pictureLabel.height() < thumbnailHeight)
    {
        controlPanel.positionSlider.thumbnail.pictureLabel.setPixmap(
        //     frameIndexer.getFrameByTime(
        //         controlPanel.positionSlider.ms
        //     )
            // QPixmap::fromImage(
            //     thumbnailVideoSink.videoFrame().toImage()
            // ).scaledToHeight(thumbnailHeight)
            QPixmap::fromImage(
                videoSink.videoFrame().toImage()
            ).scaledToHeight(thumbnailHeight)
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
        //     frameIndexer.getFrameByTime(
        //         controlPanel.positionSlider.ms
        //     )
            // QPixmap::fromImage(
            //     thumbnailVideoSink.videoFrame().toImage()
            // ).scaledToHeight(thumbnailHeight)
            QPixmap::fromImage(
               videoSink.videoFrame().toImage()
            ).scaledToHeight(thumbnailHeight)
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

    if (controlPanel.positionSlider.mouseIsInsideMe && controlPanel.positionSlider.timeTextChanged && updateFrameTicksCount == updateFrameInterval / timeEventInterval)
    {
        updateThumbnail();
        // thumbnailMediaPlayer.setPosition(controlPanel.positionSlider.ms);

        controlPanel.positionSlider.timeTextChanged = false;
        updateFrameTicksCount = 0;
    }

    if (updateFrameTicksCount != updateFrameInterval / timeEventInterval)
        updateFrameTicksCount++;
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

void VideoPlayer::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        controlPanel.startMouseLeftPress = std::chrono::steady_clock::now();
        controlPanel.mouseLeftPressed = true;
    }
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

        if (clickDuration.count() <= holdTreshold)
        {
            controlPanel.show();
            controlPanel.startShowChildren = std::chrono::steady_clock::now();
            play();

            if (controlPanel.itsALeftClick && clickInterval.count() < doubleClickDelay && controlPanel.alreadyLeftClickedOnce)
                toggleFullscreen();

            controlPanel.itsALeftClick = true;
        }

        mediaPlayer.setPlaybackRate(1);

        if (!controlPanel.alreadyLeftClickedOnce)
            controlPanel.alreadyLeftClickedOnce = true;
        controlPanel.startMouseLeftRelease = finish;
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