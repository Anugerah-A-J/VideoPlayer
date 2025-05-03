#include "VideoWidget.h"
#include <QMouseEvent>
#include <QPainter>

VideoWidget::VideoWidget():
    showUI{false},
    showUIDuration{1},
    fps{50},
    count{0},
    playButton{
        QPoint{0, 0},
        QPoint{0, 12},
        QPoint{8, 6}
    }
{
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&VideoWidget::update));
    timer.start(1000/fps);
    setMouseTracking(true);
}

void VideoWidget::mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "mouse moved!";

    if (event->position().x() < 0 || event->position().y() < 0)
        showUI = false;
    else
    {
        showUI = true;
        count = 0;
    }
}

void VideoWidget::paintEvent(QPaintEvent*)
{
    if (!showUI)
        return;

    count++;

    if (count == fps * showUIDuration)
        showUI = false;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#888888"));
    painter.setTransform(QTransform(1, 0, 0, -1, 1, 1));
    // painter.drawLine(QLine(0, 0, width(), height()));
    painter.translate(0, height());
    painter.drawConvexPolygon(playButton, 3);
}