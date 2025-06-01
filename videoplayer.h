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
#include <QVideoWidget>
#include <QVideoSink>

class ControlPanel : public QWidget
{
friend class VideoPlayer;
private:
    ControlPanel(QWidget *parent = nullptr);
    // void mouseMoveEvent(QMouseEvent*) override;
    bool mouseMoved;
    // void mousePressEvent(QMouseEvent*) override;
    // void mouseReleaseEvent(QMouseEvent*) override;
    bool mouseLeftPressed;
    bool mouseLeftReleased;
    std::chrono::time_point<std::chrono::steady_clock> startMouseLeftPress;
    std::chrono::time_point<std::chrono::steady_clock> startMouseLeftRelease;
    std::chrono::time_point<std::chrono::steady_clock> startShowChildren;
    QSlider positionSlider;
    QPushButton playButton;
    QLabel timeLabel;
    QPushButton fullscreenButton;
    QGridLayout layout;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon fullscreenIcon;
    QIcon exitFullscreenIcon;
};

class VideoPlayer : public QWidget
{
public:
    VideoPlayer(QWidget *parent = nullptr);
    void load(const QUrl &url);
    bool isPlayerAvailable() const;
    // QSize sizeHint() const override;
    void openFile();
    void play();
    void run();
private:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    // void rotateVideo(int angle);
    void timeEvent();
    void toggleFullscreen();
    QString msToTime(qint64 ms, bool& overAnHour);
    void printError(QMediaPlayer::Error error, const QString &errorString);

    // void resizeEvent(QResizeEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;
    // void leaveEvent(QEvent*) override;
    // void showEvent(QShowEvent*) override;

    ControlPanel controlPanel;
    QMediaPlayer mediaPlayer;
    QVideoSink videoSink;
    QPushButton openFileButton;
    // QGraphicsScene scene;
    // QGraphicsVideoItem videoItem;
    // QGraphicsView graphicsView;
    QAudioOutput audioOutput;
    QStackedLayout layout;
    QTimer timer;
    Qt::WindowStates beforeFullscreenState;
    QString durationTime;
    bool processKeySpaceRelease;
    bool processKeySpacePress;
    std::chrono::time_point<std::chrono::steady_clock> startKeySpacePress;
    static constexpr float keyRepeatDelay = 0.25; // in second
    static constexpr int skipDuration = 7000; // in millisecond
    static constexpr float holdTreshold = 0.5; // in second
    static constexpr float showControlPanelDuration = 3; // in second
    static constexpr float doubleClickDelay = 0.5; // in second
};

#endif