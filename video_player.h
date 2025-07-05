#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QAudioOutput>
#include <QPushButton>
#include <QStackedLayout>
#include <QTimer>

#include "frame_indexer.h"
#include "control_panel.h"

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
    void updateThumbnail(const qint64& ms, const QPoint& cursorGlobalPositionOnTopOfSlider);
    void updateThumbnail();
private:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    // void rotateVideo(int angle);
    void timeEvent();
    void toggleFullscreen();
    void printError(QMediaPlayer::Error error, const QString &errorString);
    void fitAndCenterFrameRect();
    void zoomFrameRect(const QPointF&, float oldZoomFactor);
    void toggleLoop();
    void toggleAlwaysOnTop();

    void resizeEvent(QResizeEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void leaveEvent(QEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
	void wheelEvent(QWheelEvent*) override;
    bool mouseLeftPressed;
    bool mouseRightPressed;
    std::chrono::time_point<std::chrono::steady_clock> startMouseLeftPress;
    std::chrono::time_point<std::chrono::steady_clock> startMouseLeftRelease;
    bool itsALeftClick;
    bool alreadyLeftClickedOnce;
    std::chrono::time_point<std::chrono::steady_clock> startShowChildren;
    void showEvent(QShowEvent*) override;
    // bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    void mouseLeftReleased();
    void mouseRightReleased();

    ControlPanel controlPanel;
    QMediaPlayer mediaPlayer;
    QVideoSink videoSink;
    QPushButton openFileButton;
    QAudioOutput audioOutput;
    QStackedLayout layout;
    QTimer timer;
    Qt::WindowStates beforeFullscreenState;
    QString durationTime;
    bool keySpacePressIsAutoRepeat;
    FrameIndexer frameIndexer;
    QRect frameRect;
    QRect fitWindowRect;
    QRect fitFrameRect;
    QPointF oldMousePosition;
    int updateFrameTicksCount;
    float zoomFactor; // 1 to 2
    int frame_width;
    int frame_height;
    bool alwaysOnTop;
public:
    static constexpr int initialWidth = 640;
    static constexpr int initialHeight = 480;
    static constexpr int timeEventInterval          = 100; // in millisecond
    static constexpr int skipDuration               = 7000; // in millisecond
    static constexpr float showControlPanelDuration = 3; // in second
    static constexpr float holdTreshold             = 0.5; // in second
    static constexpr float doubleClickDelay         = 0.5; // in second
    static constexpr int thumbnailHeight            = 100;
    static constexpr int updateFrameInterval        = 100; // in millisecond
};

#endif