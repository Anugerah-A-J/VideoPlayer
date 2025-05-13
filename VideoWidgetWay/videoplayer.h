// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void setUrl(const QUrl &url);

public slots:
    void openFile();
    void play();

private slots:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void handleError();

private:
    QMediaPlayer mediaPlayer;
    QVideoWidget videoWidget;
    QAudioOutput audioOutput;
    QStackedLayout layout;
    QVBoxLayout controlLayout;
    QSlider positionSlider;
    QPushButton playButton;
    QLabel errorLabel;
};

#endif
