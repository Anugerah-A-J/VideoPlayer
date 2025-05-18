// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "videoplayer.h"
#include <QStyle>
#include <QScreen>
#include <QFileDialog>
#include <QStandardPaths>

VideoPlayer::ControlPanel::ControlPanel(QWidget *parent):
    QWidget{parent},
    positionSlider{Qt::Horizontal},
    start{std::chrono::steady_clock::now()}
{
    positionSlider.setRange(0, 0);

    playButton.setEnabled(false);
    playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    coreLayout.addWidget(
        &positionSlider,
        0, 0, 1, 1,
        Qt::AlignBottom
    );
    coreLayout.addWidget(
        &playButton,
        1, 0, 1, 1,
        Qt::AlignBottom
    );
    core.setLayout(&coreLayout);

    layout.addWidget(
        &core,
        1, 0, 1, 1,
        Qt::AlignBottom
    );
    setLayout(&layout);
    setMouseTracking(true);
    positionSlider.hide();
    playButton.hide();
}

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget{parent},
    // videoItem(new QGraphicsVideoItem),
    graphicsView{&scene}
{
    scene.addItem(&videoItem);

    connect(&controlPanel.playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);
    connect(&controlPanel.positionSlider, &QAbstractSlider::sliderMoved, this, &VideoPlayer::setPosition);

    layout.addWidget(&graphicsView);
    layout.addWidget(&controlPanel);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    mediaPlayer.setVideoOutput(&videoItem);
    mediaPlayer.setAudioOutput(&audioOutput);
    connect(&mediaPlayer,
        &QMediaPlayer::playbackStateChanged,
        this,
        &VideoPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);

    load(QUrl("test.mp4"));
    graphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    // graphicsView.setStyleSheet("border: 5px solid red");
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&VideoPlayer::hideControlPanel));
    timer.start(100);
    play();
}

// VideoPlayer::~VideoPlayer() { }

QSize VideoPlayer::sizeHint() const
{
    return (videoItem.size() * qreal(3) / qreal(2)).toSize();
}

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
    switch (mediaPlayer.playbackState()) {
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
    switch (state) {
    case QMediaPlayer::PlayingState:
        controlPanel.playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        controlPanel.playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    controlPanel.positionSlider.setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    controlPanel.positionSlider.setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void VideoPlayer::rotateVideo(int angle)
{
    // rotate around the center of video element
    qreal x = videoItem.boundingRect().width() / 2.0;
    qreal y = videoItem.boundingRect().height() / 2.0;
    videoItem.setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));
}

void VideoPlayer::hideControlPanel()
{
    const auto finish = std::chrono::steady_clock::now();
    const std::chrono::duration<float> elapsed_seconds = finish - controlPanel.start;

    if (elapsed_seconds.count() >= 3)
    {
        controlPanel.playButton.hide();
        controlPanel.positionSlider.hide();
    }
}

void VideoPlayer::resizeEvent(QResizeEvent*)
{
    graphicsView.fitInView(&videoItem, Qt::KeepAspectRatio);
}

void VideoPlayer::showEvent(QShowEvent*)
{
    videoItem.setSize(size());
    controlPanel.playButton.setMaximumWidth(controlPanel.playButton.height());
}

void VideoPlayer::ControlPanel::mouseMoveEvent(QMouseEvent*)
{
    qDebug() << "mouse is moved";
    playButton.show();
    positionSlider.show();
    start = std::chrono::steady_clock::now();
}