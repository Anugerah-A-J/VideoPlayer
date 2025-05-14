#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QStackedLayout>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

class VideoPlayer : public QWidget
{
    // Q_OBJECT
public:
    VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();
    void openFile();
    void setUrl(const QUrl &url);
private:
    class Core : public QWidget
    {
    public:
        Core(QWidget *parent = nullptr);
        void setUrl(const QUrl &url);
        void play();
        void setPosition(int position);
        void mediaStateChanged(QMediaPlayer::PlaybackState state);
        void positionChanged(qint64 position);
        void durationChanged(qint64 duration);
        void handleError();
        QSlider positionSlider;
        QPushButton playButton;
        QGridLayout layout;
        QMediaPlayer mediaPlayer;
        QVideoWidget videoWidget;
        QAudioOutput audioOutput;
        QLabel errorLabel;
    };

    QStackedLayout layout;
    Core core;
};

#endif
