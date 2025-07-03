#include "control_panel.h"
#include "video_player.h"

ControlPanel::ControlPanel(QWidget *parent):
    QWidget{parent},
    // mouseLeftPressed{false},
    // itsALeftClick{false},
    // alreadyLeftClickedOnce{false},
    positionSlider{Qt::Horizontal},
    playIcon{":/icon/play.svg"},
    pauseIcon{":/icon/pause.svg"},
    fullscreenIcon{":/icon/enter-fullscreen.svg"},
    exitFullscreenIcon{":/icon/exit-fullscreen.svg"},
    loopAcvtiveIcon{":/icon/loop-active.svg"},
    loopInactiveIcon{":/icon/loop-inactive.svg"},
    alwaysOnTopActiveIcon{":/icon/always-on-top-active.svg"},
    alwaysOnTopInactiveIcon{":/icon/always-on-top-inactive.svg"}
{
    playButton.setEnabled(false);
    playButton.setStyleSheet("background: transparent; border: none;");
    playButton.setIcon(playIcon);
    playButton.setCursor(Qt::PointingHandCursor);
    // playButton.setAttribute(Qt::WA_TranslucentBackground); this doesn't work

    loopButton.setStyleSheet("background: transparent; border: none;");
    loopButton.setIcon(loopInactiveIcon);
    loopButton.setCursor(Qt::PointingHandCursor);

    alwaysOnTopButton.setStyleSheet("background: transparent; border: none;");
    alwaysOnTopButton.setIcon(alwaysOnTopInactiveIcon);
    alwaysOnTopButton.setCursor(Qt::PointingHandCursor);

    fullscreenButton.setStyleSheet("background: transparent; border: none;");
    fullscreenButton.setIcon(fullscreenIcon);
    fullscreenButton.setCursor(Qt::PointingHandCursor);
    // fullscreenButton.setAttribute(Qt::WA_TranslucentBackground); this doesn't work

    layout.addWidget(&positionSlider, 1, 0, 1, 5);
    layout.addWidget(&playButton, 2, 0, 1, 1);
    layout.addWidget(&loopButton, 2, 1, 1, 1);
    layout.addWidget(&timeOverDurationLabel, 2, 2, 1, 1);
    layout.addWidget(&alwaysOnTopButton, 2, 3, 1, 1);
    layout.addWidget(&fullscreenButton, 2, 4, 1, 1);
    layout.setRowStretch(0, 1);

    layout.setColumnStretch(2, 1);

    setLayout(&layout);
    setMouseTracking(true);
    // setStyleSheet("border: 1px solid red");
}

void ControlPanel::mouseMoveEvent(QMouseEvent* e)
{
    static_cast<VideoPlayer*>(parentWidget())->mouseMoveEvent(e);
}

void ControlPanel::mousePressEvent(QMouseEvent* e)
{
    static_cast<VideoPlayer*>(parentWidget())->mousePressEvent(e);
}

void ControlPanel::mouseReleaseEvent(QMouseEvent* e)
{
    static_cast<VideoPlayer*>(parentWidget())->mouseReleaseEvent(e);
}

void ControlPanel::wheelEvent(QWheelEvent* e)
{
    static_cast<VideoPlayer*>(parentWidget())->wheelEvent(e);
}