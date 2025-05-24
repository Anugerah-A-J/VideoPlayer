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

    QCoreApplication::setApplicationName("Player Example");
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt MultiMedia Player QGraphicsView Example");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL to open.");
    parser.process(app);

    VideoPlayer player;

    if (!parser.positionalArguments().isEmpty() &&
    player.isPlayerAvailable()) {
        const QUrl url = QUrl::fromUserInput(
            parser.positionalArguments().constFirst(),
            QDir::currentPath(),
            QUrl::AssumeLocalFile);
        player.load(url);
    }

    player.show();

    return app.exec();
}