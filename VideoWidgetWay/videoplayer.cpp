// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "videoplayer.h"

#include <QFileDialog>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QString>
#include <QStyle>

VideoPlayer::VideoPlayer(QWidget *parent):
    QWidget(parent),
    mediaPlayer(),
    videoWidget(this),
    audioOutput(),
    layout(),
    controlLayout(),
    positionSlider(Qt::Horizontal, this),
    playButton(this),
    errorLabel(this)
{
    // QAbstractButton *openButton = new QPushButton(tr("Open..."));
    // connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    playButton.setEnabled(false);
    playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(&playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);

    positionSlider.setRange(0, 0);

    connect(&positionSlider, &QAbstractSlider::sliderMoved, this, &VideoPlayer::setPosition);

    errorLabel.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    // controlLayout->setContentsMargins(0, 0, 0, 0);
    // // controlLayout->addWidget(openButton);

    // QBoxLayout *layout = new QVBoxLayout;
    // layout->addLayout(controlLayout);
    // layout->addWidget(m_errorLabel);

    // controlLayout.addWidget(&videoWidget);
    controlLayout.addWidget(&positionSlider);
    controlLayout.addWidget(&playButton);
    layout.addWidget(&videoWidget);
    layout.addItem(&controlLayout);
    layout.setStackingMode(QStackedLayout::StackAll);
    setLayout(&layout);

    // videoWidget.lower();

    mediaPlayer.setVideoOutput(&videoWidget);
    connect(&mediaPlayer, &QMediaPlayer::playbackStateChanged, this,
            &VideoPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(&mediaPlayer, &QMediaPlayer::errorChanged, this, &VideoPlayer::handleError);

    setUrl(QUrl("../test.mp4"));
    mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.play();
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
        setUrl(fileDialog.selectedUrls().constFirst());
}

void VideoPlayer::setUrl(const QUrl &url)
{
    errorLabel.setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mediaPlayer.setSource(url);
    playButton.setEnabled(true);
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
            playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        default:
            playButton.setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    positionSlider.setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    positionSlider.setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void VideoPlayer::handleError()
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

#include "moc_videoplayer.cpp"
