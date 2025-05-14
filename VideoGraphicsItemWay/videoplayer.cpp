// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "videoplayer.h"
#include <QStyle>
#include <QScreen>
#include <QFileDialog>
#include <QStandardPaths>

VideoPlayer::ControlPanel::ControlPanel(QWidget *parent):
    playButton(this),
    positionSlider(Qt::Horizontal, this),

    layout()
{
    positionSlider.setRange(0, 0);

    playButton.setEnabled(false);
    playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    layout.addWidget(&positionSlider, 0, 0, 1, 5);
    layout.addWidget(&playButton, 1, 2, 1, 1);
    setLayout(&layout);
}

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget(parent),
    mediaPlayer(),
    videoItem(new QGraphicsVideoItem),
    scene(),
    audioOutput(),

    graphicsView(&scene, this),
    controlPanel(this),

    layout()
{
    const QSize screenGeometry = screen()->availableSize();

    videoItem->setSize(
        QSizeF(screenGeometry.width() / 3,
        screenGeometry.height() / 2));

    scene.addItem(videoItem);

    // QSlider *rotateSlider = new QSlider(Qt::Horizontal);
    // rotateSlider->setToolTip(tr("Rotate Video"));
    // rotateSlider->setRange(-180, 180);
    // rotateSlider->setValue(0);
    // connect(rotateSlider, &QAbstractSlider::valueChanged, this, &VideoPlayer::rotateVideo);

    // QAbstractButton *openButton = new QPushButton(tr("Open..."));
    // connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    connect(&controlPanel.playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);

    connect(&controlPanel.positionSlider, &QAbstractSlider::sliderMoved, this, &VideoPlayer::setPosition);

    layout.addWidget(&graphicsView);
    layout.addWidget(&controlPanel);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    // QBoxLayout *controlLayout = new QHBoxLayout;
    // controlLayout->setContentsMargins(0, 0, 0, 0);
    // controlLayout->addWidget(openButton);
    // controlLayout->addWidget(m_playButton);
    // controlLayout->addWidget(m_positionSlider);

    // QBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(graphicsView);
    // layout->addWidget(rotateSlider);
    // layout->addLayout(controlLayout);

    mediaPlayer.setVideoOutput(videoItem);
    mediaPlayer.setAudioOutput(&audioOutput);
    connect(&mediaPlayer,
        &QMediaPlayer::playbackStateChanged,
        this,
        &VideoPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);

    load(QUrl("../test.mp4"));
    play();
}

// VideoPlayer::~VideoPlayer() { }

QSize VideoPlayer::sizeHint() const
{
    return (videoItem->size() * qreal(3) / qreal(2)).toSize();
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
    qreal x = videoItem->boundingRect().width() / 2.0;
    qreal y = videoItem->boundingRect().height() / 2.0;
    videoItem->setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));
}

// #include "moc_videoplayer.cpp"
