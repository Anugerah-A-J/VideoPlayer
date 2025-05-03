#include <QApplication>
#include <QMediaPlayer>
#include "VideoWidget.h"
#include <QAudioOutput>
#include <QGraphicsVideoItem>
#include <QGraphicsView>

void usingQVideoWidget(QMediaPlayer& player, VideoWidget& videoWidget)
{
    player.setVideoOutput(&videoWidget);
    videoWidget.show();
}

void usingQGraphicsVideoItem(QMediaPlayer& player, QGraphicsView& graphicsView, QGraphicsVideoItem* graphicsItem)
{
    player.setVideoOutput(graphicsItem);
    graphicsView.scene()->addItem(graphicsItem);
    graphicsView.show();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMediaPlayer player;
    player.setSource(QUrl("./test.mp4"));
    QAudioOutput audioOutput;
    player.setAudioOutput(&audioOutput);

    VideoWidget videoWidget;
    QGraphicsVideoItem graphicsItem;
    QGraphicsView graphicsView;

    usingQVideoWidget(player, videoWidget);
    // usingQGraphicsVideoItem(player, graphicsView, &graphicsItem); // segfault

    player.play();
    return app.exec();
}