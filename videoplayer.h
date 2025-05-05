// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <QWidget>
#include <QAudioOutput>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QGraphicsVideoItem;
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void load(const QUrl &url);
    bool isPlayerAvailable() const;

    QSize sizeHint() const override;

public slots:
    void openFile();
    void play();

private slots:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void rotateVideo(int angle);

private:
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    QMediaPlayer *m_mediaPlayer = nullptr;
    QGraphicsVideoItem *m_videoItem = nullptr;
    QAbstractButton *m_playButton = nullptr;
    QSlider *m_positionSlider = nullptr;

    QAudioOutput audioOutput;
    bool showUI;
    unsigned int count;
    QTimer timer;
};

constexpr unsigned int showUIDuration = 1; // in sec.
constexpr unsigned int fps = 50 / 50;
constexpr QPoint playButton[3] = {QPoint(0, 0), QPoint(0, 12), QPoint(8, 6)};

#endif
