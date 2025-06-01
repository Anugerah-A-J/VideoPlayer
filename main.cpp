/*
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build
magnet:?xt=urn:btih:C0CEA3B74443699D64DA9E24DFB8B888664D331D&dn=OnlyFans%2023%2009%2013%20Sophie%20Reade%20And%20Johnny%20Sins%20XXX%201080p%20MP4-XX&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.torrent.eu.org%3A451%2Fannounce&tr=udp%3A%2F%2Ftracker.bittor.pw%3A1337%2Fannounce&tr=udp%3A%2F%2Fpublic.popcorn-tracker.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.dler.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fexodus.desync.com%3A6969&tr=udp%3A%2F%2Fopen.demonii.com%3A1337%2Fannounce
https://noodlemagazine.com/watch/-221654591_456239609
*/

#include "videoplayer.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QUrl>
#include <QStyle>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("VideoPlayer");
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt MultiMedia Player QGraphicsView Example");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL to open.");
    parser.process(app);

    VideoPlayer player;

    if (!parser.positionalArguments().isEmpty() && player.isPlayerAvailable())
    {
        const QUrl url = QUrl::fromUserInput(
            parser.positionalArguments().constFirst(),
            QDir::currentPath(),
            QUrl::AssumeLocalFile);
        player.load(url);
        player.run();
    }

    player.show();

    return app.exec();
}