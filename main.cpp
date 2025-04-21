#include <QApplication>
#include <QStyleHints>

#include "VideoPlayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.styleHints()->setColorScheme(Qt::ColorScheme::Dark);

    VideoPlayer videoPlayer;
    videoPlayer.show();
    
    return QCoreApplication::exec();
}