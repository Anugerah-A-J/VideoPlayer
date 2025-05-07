// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "videoplayer.h"

#include <QBoxLayout>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QStandardPaths>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>
#include <QSizePolicy>
#include <qnamespace.h>
#include <qsizepolicy.h>

VideoPlayer::VideoPlayer(QWidget *parent) : QWidget(parent),
    showUI{false},
    count{0}
{
    m_mediaPlayer = new QMediaPlayer(this);
    const QSize screenGeometry = screen()->availableSize();
    m_videoItem = new QGraphicsVideoItem;
    m_videoItem->setSize(QSizeF(screenGeometry.width() / 3, screenGeometry.height() / 2));

    QGraphicsScene *scene = new QGraphicsScene(this);
    // QGraphicsView *graphicsView = new QGraphicsView(scene);
    QGraphicsView *graphicsView = new QGraphicsView(scene, this);

    scene->addItem(m_videoItem);

    // QSlider *rotateSlider = new QSlider(Qt::Horizontal);
    // rotateSlider->setToolTip(tr("Rotate Video"));
    // rotateSlider->setRange(-180, 180);
    // rotateSlider->setValue(0);

    // connect(rotateSlider, &QAbstractSlider::valueChanged, this, &VideoPlayer::rotateVideo);

    // QAbstractButton *openButton = new QPushButton(tr("Open..."));
    // connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    m_playButton = new QPushButton;
    m_playButton->setEnabled(false);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setRange(0, 0);

    connect(m_positionSlider, &QAbstractSlider::sliderMoved, this, &VideoPlayer::setPosition);

    // QBoxLayout *controlLayout = new QHBoxLayout;
    QBoxLayout *controlLayout = new QVBoxLayout(this);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    // controlLayout->addWidget(openButton);
    controlLayout->addWidget(m_positionSlider);
    controlLayout->addWidget(m_playButton);

    // QBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(graphicsView);
    // layout->addWidget(rotateSlider);
    // layout->addLayout(controlLayout);

    m_mediaPlayer->setVideoOutput(m_videoItem);
    m_mediaPlayer->setAudioOutput(&audioOutput);
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this,
            &VideoPlayer::mediaStateChanged);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);

    // connect(&timer, &QTimer::timeout, this, QOverload<>::of(&VideoPlayer::update));
    // timer.start(1000/fps);
    setMouseTracking(true);
    graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);
    graphicsView->fitInView(m_videoItem, Qt::KeepAspectRatio);
    load(QUrl("test.mp4"));
    play();
}

VideoPlayer::~VideoPlayer() { }

QSize VideoPlayer::sizeHint() const
{
    return (m_videoItem->size() * qreal(3) / qreal(2)).toSize();
}

bool VideoPlayer::isPlayerAvailable() const
{
    return m_mediaPlayer->isAvailable();
}

void VideoPlayer::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation)
                                    .value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        load(fileDialog.selectedUrls().constFirst());
}

void VideoPlayer::load(const QUrl &url)
{
    m_mediaPlayer->setSource(url);
    m_playButton->setEnabled(true);
}

void VideoPlayer::play()
{
    switch (m_mediaPlayer->playbackState()) {
    case QMediaPlayer::PlayingState:
        m_mediaPlayer->pause();
        break;
    default:
        m_mediaPlayer->play();
        break;
    }
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    m_positionSlider->setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    m_positionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    m_mediaPlayer->setPosition(position);
}

void VideoPlayer::rotateVideo(int angle)
{
    // rotate around the center of video element
    qreal x = m_videoItem->boundingRect().width() / 2.0;
    qreal y = m_videoItem->boundingRect().height() / 2.0;
    m_videoItem->setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));
}

void VideoPlayer::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouse is moved!";
}

void VideoPlayer::paintEvent(QPaintEvent*)
{
    // if (!showUI)
        return;

    // count++;

    // if (count == fps * showUIDuration)
    //     showUI = false;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawLine(QLine(0, 0, width(), height()));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#888888"));
    // painter.setTransform(QTransform(1, 0, 0, -1, 1, 1));
    // painter.translate(0, height());
    // painter.drawConvexPolygon(playButton, 3);
}

#include "moc_videoplayer.cpp"