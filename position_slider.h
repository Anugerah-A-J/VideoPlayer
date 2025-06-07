#ifndef POSITION_SLIDER_H
#define POSITION_SLIDER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>

#include "thumbnail.h"

class PositionSlider : public QSlider
{
    // Q_OBJECT
friend class VideoPlayer;
public:
    PositionSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
// signals:
//     void timeTextChanged(const qint64& ms, const QPoint& cursorGlobalPositionOnTopOfSlider);
private:
    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    bool mouseIsInsideMe;
    Thumbnail thumbnail;
    qint64 ms;
    QString timeText;
    QPoint cursorGlobalPositionOnTopOfSlider;
    bool timeTextChanged;
};

#endif