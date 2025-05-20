// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "videoplayer.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QUrl>
#include <QStyle>

struct StandardIconDemo : public QWidget
{
    StandardIconDemo();
};

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
    // StandardIconDemo standardIconDemo;
    // standardIconDemo.show();

    return app.exec();
}

StandardIconDemo::StandardIconDemo()
{
    std::vector<QPushButton*> buttons;

    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AddressBookNew), ""));
    buttons.back()->setToolTip("AddressBookNew");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit), ""));
    buttons.back()->setToolTip("ApplicationExit");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AppointmentNew), ""));
    buttons.back()->setToolTip("AppointmentNew");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::CallStart), ""));
    buttons.back()->setToolTip("CallStart");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::CallStop), ""));
    buttons.back()->setToolTip("CallStop");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ContactNew), ""));
    buttons.back()->setToolTip("ContactNew");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew), ""));
    buttons.back()->setToolTip("DocumentNew");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), ""));
    buttons.back()->setToolTip("DocumentOpen");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpenRecent), ""));
    buttons.back()->setToolTip("DocumentOpenRecent");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPageSetup), ""));
    buttons.back()->setToolTip("DocumentPageSetup");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrint), ""));
    buttons.back()->setToolTip("DocumentPrint");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrintPreview), ""));
    buttons.back()->setToolTip("DocumentPrintPreview");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties), ""));
    buttons.back()->setToolTip("DocumentProperties");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentRevert), ""));
    buttons.back()->setToolTip("DocumentRevert");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave), ""));
    buttons.back()->setToolTip("DocumentSave");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs), ""));
    buttons.back()->setToolTip("DocumentSaveAs");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSend), ""));
    buttons.back()->setToolTip("DocumentSend");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditClear), ""));
    buttons.back()->setToolTip("EditClear");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy), ""));
    buttons.back()->setToolTip("EditCopy");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditCut), ""));
    buttons.back()->setToolTip("EditCut");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete), ""));
    buttons.back()->setToolTip("EditDelete");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditFind), ""));
    buttons.back()->setToolTip("EditFind");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste), ""));
    buttons.back()->setToolTip("EditPaste");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo), ""));
    buttons.back()->setToolTip("EditRedo");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditSelectAll), ""));
    buttons.back()->setToolTip("EditSelectAll");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo), ""));
    buttons.back()->setToolTip("EditUndo");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FolderNew), ""));
    buttons.back()->setToolTip("FolderNew");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatIndentLess), ""));
    buttons.back()->setToolTip("FormatIndentLess");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatIndentMore), ""));
    buttons.back()->setToolTip("FormatIndentMore");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyCenter), ""));
    buttons.back()->setToolTip("FormatJustifyCenter");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyFill), ""));
    buttons.back()->setToolTip("FormatJustifyFill");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyLeft), ""));
    buttons.back()->setToolTip("FormatJustifyLeft");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyRight), ""));
    buttons.back()->setToolTip("FormatJustifyRight");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextDirectionLtr), ""));
    buttons.back()->setToolTip("FormatTextDirectionLtr");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextDirectionRtl), ""));
    buttons.back()->setToolTip("FormatTextDirectionRtl");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextBold), ""));
    buttons.back()->setToolTip("FormatTextBold");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextItalic), ""));
    buttons.back()->setToolTip("FormatTextItalic");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextUnderline), ""));
    buttons.back()->setToolTip("FormatTextUnderline");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextStrikethrough), ""));
    buttons.back()->setToolTip("FormatTextStrikethrough");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::GoDown), ""));
    buttons.back()->setToolTip("GoDown");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::GoHome), ""));
    buttons.back()->setToolTip("GoHome");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::GoNext), ""));
    buttons.back()->setToolTip("GoNext");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious), ""));
    buttons.back()->setToolTip("GoPrevious");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::GoUp), ""));
    buttons.back()->setToolTip("GoUp");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout), ""));
    buttons.back()->setToolTip("HelpAbout");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::HelpFaq), ""));
    buttons.back()->setToolTip("HelpFaq");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::InsertImage), ""));
    buttons.back()->setToolTip("InsertImage");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::InsertLink), ""));
    buttons.back()->setToolTip("InsertLink");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::InsertText), ""));
    buttons.back()->setToolTip("InsertText");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd), ""));
    buttons.back()->setToolTip("ListAdd");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove), ""));
    buttons.back()->setToolTip("ListRemove");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailForward), ""));
    buttons.back()->setToolTip("MailForward");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailMarkImportant), ""));
    buttons.back()->setToolTip("MailMarkImportant");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailMarkRead), ""));
    buttons.back()->setToolTip("MailMarkRead");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailMarkUnread), ""));
    buttons.back()->setToolTip("MailMarkUnread");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew), ""));
    buttons.back()->setToolTip("MailMessageNew");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailReplyAll), ""));
    buttons.back()->setToolTip("MailReplyAll");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailReplySender), ""));
    buttons.back()->setToolTip("MailReplySender");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailSend), ""));
    buttons.back()->setToolTip("MailSend");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaEject), ""));
    buttons.back()->setToolTip("MediaEject");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause), ""));
    buttons.back()->setToolTip("MediaPlaybackPause");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart), ""));
    buttons.back()->setToolTip("MediaPlaybackStart");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStop), ""));
    buttons.back()->setToolTip("MediaPlaybackStop");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaRecord), ""));
    buttons.back()->setToolTip("MediaRecord");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaSeekBackward), ""));
    buttons.back()->setToolTip("MediaSeekBackward");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaSeekForward), ""));
    buttons.back()->setToolTip("MediaSeekForward");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaSkipBackward), ""));
    buttons.back()->setToolTip("MediaSkipBackward");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaSkipForward), ""));
    buttons.back()->setToolTip("MediaSkipForward");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ObjectRotateLeft), ""));
    buttons.back()->setToolTip("ObjectRotateLeft");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ObjectRotateRight), ""));
    buttons.back()->setToolTip("ObjectRotateRight");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ProcessStop), ""));
    buttons.back()->setToolTip("ProcessStop");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SystemLockScreen), ""));
    buttons.back()->setToolTip("SystemLockScreen");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SystemLogOut), ""));
    buttons.back()->setToolTip("SystemLogOut");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SystemSearch), ""));
    buttons.back()->setToolTip("SystemSearch");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SystemReboot), ""));
    buttons.back()->setToolTip("SystemReboot");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SystemShutdown), ""));
    buttons.back()->setToolTip("SystemShutdown");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ToolsCheckSpelling), ""));
    buttons.back()->setToolTip("ToolsCheckSpelling");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ViewFullscreen), ""));
    buttons.back()->setToolTip("ViewFullscreen");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ViewRefresh), ""));
    buttons.back()->setToolTip("ViewRefresh");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ViewRestore), ""));
    buttons.back()->setToolTip("ViewRestore");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WindowClose), ""));
    buttons.back()->setToolTip("WindowClose");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew), ""));
    buttons.back()->setToolTip("WindowNew");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ZoomFitBest), ""));
    buttons.back()->setToolTip("ZoomFitBest");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ZoomIn), ""));
    buttons.back()->setToolTip("ZoomIn");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOut), ""));
    buttons.back()->setToolTip("ZoomOut");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AudioCard), ""));
    buttons.back()->setToolTip("AudioCard");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AudioInputMicrophone), ""));
    buttons.back()->setToolTip("AudioInputMicrophone");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::Battery), ""));
    buttons.back()->setToolTip("Battery");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::CameraPhoto), ""));
    buttons.back()->setToolTip("CameraPhoto");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::CameraVideo), ""));
    buttons.back()->setToolTip("CameraVideo");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::CameraWeb), ""));
    buttons.back()->setToolTip("CameraWeb");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::Computer), ""));
    buttons.back()->setToolTip("Computer");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DriveHarddisk), ""));
    buttons.back()->setToolTip("DriveHarddisk");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DriveOptical), ""));
    buttons.back()->setToolTip("DriveOptical");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::InputGaming), ""));
    buttons.back()->setToolTip("InputGaming");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::InputKeyboard), ""));
    buttons.back()->setToolTip("InputKeyboard");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::InputMouse), ""));
    buttons.back()->setToolTip("InputMouse");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::InputTablet), ""));
    buttons.back()->setToolTip("InputTablet");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaFlash), ""));
    buttons.back()->setToolTip("MediaFlash");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaOptical), ""));
    buttons.back()->setToolTip("MediaOptical");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaTape), ""));
    buttons.back()->setToolTip("MediaTape");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MultimediaPlayer), ""));
    buttons.back()->setToolTip("MultimediaPlayer");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::NetworkWired), ""));
    buttons.back()->setToolTip("NetworkWired");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::NetworkWireless), ""));
    buttons.back()->setToolTip("NetworkWireless");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::Phone), ""));
    buttons.back()->setToolTip("Phone");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::Printer), ""));
    buttons.back()->setToolTip("Printer");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::Scanner), ""));
    buttons.back()->setToolTip("Scanner");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::VideoDisplay), ""));
    buttons.back()->setToolTip("VideoDisplay");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AppointmentMissed), ""));
    buttons.back()->setToolTip("AppointmentMissed");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AppointmentSoon), ""));
    buttons.back()->setToolTip("AppointmentSoon");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeHigh), ""));
    buttons.back()->setToolTip("AudioVolumeHigh");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeLow), ""));
    buttons.back()->setToolTip("AudioVolumeLow");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeMedium), ""));
    buttons.back()->setToolTip("AudioVolumeMedium");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeMuted), ""));
    buttons.back()->setToolTip("AudioVolumeMuted");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::BatteryCaution), ""));
    buttons.back()->setToolTip("BatteryCaution");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::BatteryLow), ""));
    buttons.back()->setToolTip("BatteryLow");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DialogError), ""));
    buttons.back()->setToolTip("DialogError");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DialogInformation), ""));
    buttons.back()->setToolTip("DialogInformation");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DialogPassword), ""));
    buttons.back()->setToolTip("DialogPassword");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DialogQuestion), ""));
    buttons.back()->setToolTip("DialogQuestion");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::DialogWarning), ""));
    buttons.back()->setToolTip("DialogWarning");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FolderDragAccept), ""));
    buttons.back()->setToolTip("FolderDragAccept");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen), ""));
    buttons.back()->setToolTip("FolderOpen");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::FolderVisiting), ""));
    buttons.back()->setToolTip("FolderVisiting");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ImageLoading), ""));
    buttons.back()->setToolTip("ImageLoading");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ImageMissing), ""));
    buttons.back()->setToolTip("ImageMissing");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailAttachment), ""));
    buttons.back()->setToolTip("MailAttachment");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailUnread), ""));
    buttons.back()->setToolTip("MailUnread");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailRead), ""));
    buttons.back()->setToolTip("MailRead");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MailReplied), ""));
    buttons.back()->setToolTip("MailReplied");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistRepeat), ""));
    buttons.back()->setToolTip("MediaPlaylistRepeat");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistShuffle), ""));
    buttons.back()->setToolTip("MediaPlaylistShuffle");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::NetworkOffline), ""));
    buttons.back()->setToolTip("NetworkOffline");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::PrinterPrinting), ""));
    buttons.back()->setToolTip("PrinterPrinting");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SecurityHigh), ""));
    buttons.back()->setToolTip("SecurityHigh");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SecurityLow), ""));
    buttons.back()->setToolTip("SecurityLow");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SoftwareUpdateAvailable), ""));
    buttons.back()->setToolTip("SoftwareUpdateAvailable");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SoftwareUpdateUrgent), ""));
    buttons.back()->setToolTip("SoftwareUpdateUrgent");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SyncError), ""));
    buttons.back()->setToolTip("SyncError");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::SyncSynchronizing), ""));
    buttons.back()->setToolTip("SyncSynchronizing");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::UserAvailable), ""));
    buttons.back()->setToolTip("UserAvailable");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::UserOffline), ""));
    buttons.back()->setToolTip("UserOffline");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherClear), ""));
    buttons.back()->setToolTip("WeatherClear");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherClearNight), ""));
    buttons.back()->setToolTip("WeatherClearNight");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherFewClouds), ""));
    buttons.back()->setToolTip("WeatherFewClouds");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherFewCloudsNight), ""));
    buttons.back()->setToolTip("WeatherFewCloudsNight");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherFog), ""));
    buttons.back()->setToolTip("WeatherFog");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherShowers), ""));
    buttons.back()->setToolTip("WeatherShowers");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherSnow), ""));
    buttons.back()->setToolTip("WeatherSnow");
    buttons.push_back(new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::WeatherStorm), ""));
    buttons.back()->setToolTip("WeatherStorm");

    QGridLayout* layout = new QGridLayout;
    int i = 0, j = 0;
    for (auto x : buttons)
    {
        layout->addWidget(x, i, j);
        qDebug() << i << ", " << j;
        j++;

        if (j == 13)
            j = 0, i++;
    }
    setLayout(layout);
}