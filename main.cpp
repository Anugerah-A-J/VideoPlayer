#include <QApplication>
#include <QMediaPlayer>
#include "VideoWidget.h"
#include <QAudioOutput>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMediaPlayer player;
    player.setSource(QUrl("./test.mp4"));

    VideoWidget videoWidget;
    player.setVideoOutput(&videoWidget);

    QAudioOutput audioOutput;
    player.setAudioOutput(&audioOutput);

    videoWidget.show();
    player.play();

    return app.exec();
}