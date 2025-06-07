#include "control_panel.h"
#include "video_player.h"

ControlPanel::ControlPanel(QWidget *parent):
    QWidget{parent},
    mouseLeftPressed{false},
    itsALeftClick{false},
    alreadyLeftClickedOnce{false},
    positionSlider{Qt::Horizontal},
    playIcon{"icon/play_arrow.svg"},
    pauseIcon{"icon/pause.svg"},
    fullscreenIcon{"icon/fullscreen.svg"},
    exitFullscreenIcon{"icon/fullscreen_exit.svg"}
{
    playButton.setEnabled(false);
    playButton.setIcon(playIcon);
    playButton.setCursor(Qt::PointingHandCursor);
    // playButton.setAttribute(Qt::WA_TranslucentBackground); this doesn't work

    fullscreenButton.setIcon(fullscreenIcon);
    fullscreenButton.setCursor(Qt::PointingHandCursor);
    // fullscreenButton.setAttribute(Qt::WA_TranslucentBackground); this doesn't work

    layout.addWidget(&positionSlider, 1, 0, 1, 3);
    layout.addWidget(&playButton, 2, 0, 1, 1);
    layout.addWidget(&fullscreenButton, 2, 2, 1, 1);
    layout.setRowStretch(0, 1);

    layout.addWidget(&timeOverDurationLabel, 2, 1, 1, 1);
    layout.setColumnStretch(1, 1);

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