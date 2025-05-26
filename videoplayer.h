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
#include <QLabel>

class ControlPanel : public QWidget
{
Q_OBJECT
friend class VideoPlayer;
private:
    ControlPanel(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void showChildren();
    void hideChildren();
    // void keyPressEvent(QKeyEvent*) override;
    QSlider positionSlider;
    QPushButton playButton;
    QLabel timeLabel;
    QPushButton fullscreenButton;
    QGridLayout layout;
    std::chrono::time_point<std::chrono::steady_clock> startMouseMove;
    std::chrono::time_point<std::chrono::steady_clock> startMouseLeftButtonPress;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon fullscreenIcon;
    QIcon exitFullscreenIcon;
    bool mouseLeftButtonPressed;
signals:
    void doubleClicked();
    // void skipForward(int position);
    // void skipBackward(int position);
};

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
    void timeEvent();
    void toggleFullscreen();
    QString msToTime(qint64 ms, bool& overAnHour);

    void resizeEvent(QResizeEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

    ControlPanel controlPanel;
    QMediaPlayer mediaPlayer;
    QGraphicsScene scene;
    QGraphicsVideoItem videoItem;
    QAudioOutput audioOutput;
    QGraphicsView graphicsView;
    QStackedLayout layout;
    QTimer timer;
    Qt::WindowStates beforeFullscreenState;
    QString durationTime;
};

#endif