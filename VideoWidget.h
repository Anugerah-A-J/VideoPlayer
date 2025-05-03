#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QVideoWidget>
#include <QTimer>
#include <QPoint>

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
    const unsigned int showUIDuration; // in sec.
    const unsigned int fps;
    unsigned int count;
    QTimer timer;
    const QPoint playButton[3];
};

#endif