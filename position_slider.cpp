#include "position_slider.h"
#include "video_player.h"
#include <QPainter>

PositionSlider::PositionSlider(Qt::Orientation orientation, QWidget *parent):
    QSlider{orientation, parent},
    mouseIsInsideMe{false},
    timeText{""},
    timeTextChanged{false}
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
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
    showThumbnail = false;
}

void PositionSlider::updateThumbnailPosition(const QPoint& windowGlobalOrigin, int windowWidth)
{
    thumbnailRenderRect = thumbnail.rect();
    thumbnailRenderRect.moveTopLeft(cursorGlobalPositionOnTopOfSlider - windowGlobalOrigin);
    thumbnailRenderRect.translate(
        -thumbnail.width() / 2,
        -thumbnail.height() //- height()
    );
    const int margin = height() / 2;
    int thumbnailLocalX = thumbnailRenderRect.x();
    qDebug() << "thumbnailLocalX " << thumbnailLocalX;
    if (thumbnailLocalX < margin)
    {
        thumbnailRenderRect.translate(
            margin - thumbnailLocalX,
            0
        );
        qDebug() << "too left " << thumbnailLocalX;
    }
    else if (thumbnailLocalX + thumbnail.width() > windowWidth - margin)
    {
        thumbnailRenderRect.translate(
            windowWidth - margin - thumbnailLocalX - thumbnail.width(),
            0
        );
        qDebug() << "too right " << thumbnailLocalX + thumbnail.width() - windowWidth + margin;
    }
    timeTextRect.setTopLeft(thumbnailRenderRect.bottomLeft());
    timeTextRect.setHeight(height());
    timeTextRect.setWidth(thumbnailRenderRect.width());
}

void PositionSlider::drawThumbnail(QPainter& painter)
{
    painter.drawPixmap(thumbnailRenderRect, thumbnail);
    painter.setPen(Qt::white);
    painter.drawText(timeTextRect, timeText, {Qt::AlignCenter});
}