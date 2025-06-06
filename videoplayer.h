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

class PositionSlider : public QSlider
{
friend class VideoPlayer;
public:
    PositionSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
private:
    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    bool mouseIsInsideMe;
    QLabel timeLabel;
    qint64 ms;
};

class ControlPanel : public QWidget
{
friend class VideoPlayer;
private:
    ControlPanel(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    bool mouseLeftPressed;
    std::chrono::time_point<std::chrono::steady_clock> startMouseLeftPress;
    std::chrono::time_point<std::chrono::steady_clock> startMouseLeftRelease;
    bool itsALeftClick;
    bool alreadyLeftClickedOnce;
    std::chrono::time_point<std::chrono::steady_clock> startShowChildren;
    PositionSlider positionSlider;
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
friend class ControlPanel;
public:
    VideoPlayer(QWidget *parent = nullptr);
    void load(const QUrl &url);
    bool isPlayerAvailable() const;
    // QSize sizeHint() const override;
    void openFile();
    void play();
    void run();
    static QString msToTime(qint64 ms, bool& overAnHour);
private:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    // void rotateVideo(int angle);
    void timeEvent();
    void toggleFullscreen();
    void printError(QMediaPlayer::Error error, const QString &errorString);

    // void resizeEvent(QResizeEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void leaveEvent(QEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
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
    bool keySpacePressIsAutoRepeat;
    // std::chrono::time_point<std::chrono::steady_clock> startKeySpacePress;
public:
    static constexpr int skipDuration = 7000; // in millisecond
    static constexpr float showControlPanelDuration = 3; // in second
    static constexpr float holdTreshold = 0.5; // in second
    static constexpr float doubleClickDelay = 0.5; // in second
};

#endif