#include "position_slider.h"
#include "video_player.h"

PositionSlider::PositionSlider(Qt::Orientation orientation, QWidget *parent):
    QSlider{orientation, parent},
    mouseIsInsideMe{false},
    timeText{""},
    timeTextChanged{false}
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    // thumbnail.hide();
}

void PositionSlider::mouseMoveEvent(QMouseEvent* e)
{
    bool b;
    ms = e->position().x() / width() * maximum();

    QString newTimeText = VideoPlayer::msToTime(ms, b);

    if (timeText != newTimeText)
    {
        QPoint position {
            static_cast<int>(e->position().x()),
            -height() // on top of slider
        };
        thumbnail.timeLabel.setText(newTimeText);
        timeText = newTimeText;
        cursorGlobalPositionOnTopOfSlider = mapToGlobal(position);
        timeTextChanged = true;
        // emit timeTextChanged(ms, cursorGlobalPositionOnTopOfSlider);
    }
}

void PositionSlider::mousePressEvent(QMouseEvent*)
{}

void PositionSlider::mouseReleaseEvent(QMouseEvent*)
{
    setValue(ms);
}

void PositionSlider::enterEvent(QEnterEvent*)
{
    mouseIsInsideMe = true;
}

void PositionSlider::leaveEvent(QEvent*)
{
    mouseIsInsideMe = false;
    thumbnail.hide();
}