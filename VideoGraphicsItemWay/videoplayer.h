// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QSlider>
#include <QGridLayout>
#include <QAudioOutput>
#include <QStackedLayout>

class VideoPlayer : public QWidget
{
    // Q_OBJECT
public:
    VideoPlayer(QWidget *parent = nullptr);
    // ~VideoPlayer();
    void load(const QUrl &url);
    bool isPlayerAvailable() const;
    QSize sizeHint() const override;
// public slots:
    void openFile();
    void play();
// private slots:
private:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void rotateVideo(int angle);
// private:
    struct ControlPanel : public QWidget
    {
        ControlPanel(QWidget *parent = nullptr);
        QSlider positionSlider;
        QPushButton playButton;

        QGridLayout layout;
    };
    QMediaPlayer mediaPlayer;
    QGraphicsVideoItem* videoItem;
    QGraphicsScene scene;
    QAudioOutput audioOutput;

    QGraphicsView graphicsView;
    ControlPanel controlPanel;

    QStackedLayout layout;
};

#endif