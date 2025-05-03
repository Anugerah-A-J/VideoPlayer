#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QVideoWidget>
#include <QTimer>
#include <QPoint>

constexpr unsigned int showUIDuration = 1; // in sec.
constexpr unsigned int fps = 50;
constexpr QPoint playButton[3] = {QPoint(0, 0), QPoint(0, 12), QPoint(8, 6)};

class VideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    VideoWidget();
protected:
private:
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool showUI;
    unsigned int count;
    QTimer timer;
    // const QPoint playButton[3];
};

#endif