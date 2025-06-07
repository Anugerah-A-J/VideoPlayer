#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "position_slider.h"

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
    QLabel timeOverDurationLabel;
    QPushButton fullscreenButton;
    QGridLayout layout;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon fullscreenIcon;
    QIcon exitFullscreenIcon;
};

#endif