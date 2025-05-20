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
#include <QTimer>

class VideoPlayer : public QWidget
{
public:
    VideoPlayer(QWidget *parent = nullptr);
    void load(const QUrl &url);
    bool isPlayerAvailable() const;
    QSize sizeHint() const override;
    void openFile();
    void play();
private:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void rotateVideo(int angle);
    void hideControlPanel();

    void resizeEvent(QResizeEvent*) override;
    void showEvent(QShowEvent*) override;

    struct ControlPanel : public QWidget
    {
        ControlPanel(QWidget *parent = nullptr);
        void mouseMoveEvent(QMouseEvent*) override;
        QWidget core;
        QSlider positionSlider;
        QPushButton playButton;
        QPushButton fullscreenButton;
        QGridLayout coreLayout;
        QGridLayout layout;
        std::chrono::time_point<std::chrono::steady_clock> start;
        // std::chrono::time_point<std::chrono::steady_clock> finish;
        QIcon playIcon;
        QIcon pauseIcon;
        QIcon fullscreenIcon;
        QIcon exitFullscreenIcon;
    }
    controlPanel;
    QMediaPlayer mediaPlayer;
    QGraphicsScene scene;
    QGraphicsVideoItem videoItem;
    QAudioOutput audioOutput;
    QGraphicsView graphicsView;
    QStackedLayout layout;
    QTimer timer;
};

#endif