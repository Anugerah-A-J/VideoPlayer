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

    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarMinButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarMinButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarMenuButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarMenuButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarMaxButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarMaxButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarCloseButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarCloseButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarNormalButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarNormalButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarShadeButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarShadeButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarUnshadeButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TitleBarContextHelpButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TitleBarContextHelpButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MessageBoxInformation), ""));
    buttons.back()->setToolTip("QStyle::SP_MessageBoxInformation");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MessageBoxWarning), ""));
    buttons.back()->setToolTip("QStyle::SP_MessageBoxWarning");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MessageBoxCritical), ""));
    buttons.back()->setToolTip("QStyle::SP_MessageBoxCritical");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MessageBoxQuestion), ""));
    buttons.back()->setToolTip("QStyle::SP_MessageBoxQuestion");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DesktopIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DesktopIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TrashIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_TrashIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ComputerIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_ComputerIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DriveFDIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DriveFDIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DriveHDIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DriveHDIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DriveCDIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DriveCDIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DriveDVDIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DriveDVDIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DriveNetIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DriveNetIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DirHomeIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DirHomeIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DirOpenIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DirOpenIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DirClosedIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DirClosedIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DirIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DirIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DirLinkIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DirLinkIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DirLinkOpenIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_DirLinkOpenIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_FileIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileLinkIcon), ""));
    buttons.back()->setToolTip("QStyle::SP_FileLinkIcon");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogStart), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogStart");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogEnd), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogEnd");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogToParent), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogToParent");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogNewFolder), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogNewFolder");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogDetailedView), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogDetailedView");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogInfoView), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogInfoView");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogContentsView), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogContentsView");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogListView), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogListView");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_FileDialogBack), ""));
    buttons.back()->setToolTip("QStyle::SP_FileDialogBack");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DockWidgetCloseButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DockWidgetCloseButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ToolBarHorizontalExtensionButton), ""));
    buttons.back()->setToolTip("QStyle::SP_ToolBarHorizontalExtensionButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ToolBarVerticalExtensionButton), ""));
    buttons.back()->setToolTip("QStyle::SP_ToolBarVerticalExtensionButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogOkButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogOkButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogCancelButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogCancelButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogHelpButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogHelpButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogOpenButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogOpenButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogSaveButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogSaveButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogCloseButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogCloseButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogApplyButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogApplyButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogResetButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogResetButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogDiscardButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogDiscardButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogYesButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogYesButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogNoButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogNoButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ArrowUp), ""));
    buttons.back()->setToolTip("QStyle::SP_ArrowUp");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ArrowDown), ""));
    buttons.back()->setToolTip("QStyle::SP_ArrowDown");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ArrowLeft), ""));
    buttons.back()->setToolTip("QStyle::SP_ArrowLeft");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ArrowRight), ""));
    buttons.back()->setToolTip("QStyle::SP_ArrowRight");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ArrowBack), ""));
    buttons.back()->setToolTip("QStyle::SP_ArrowBack");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_ArrowForward), ""));
    buttons.back()->setToolTip("QStyle::SP_ArrowForward");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_CommandLink), ""));
    buttons.back()->setToolTip("QStyle::SP_CommandLink");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_VistaShield), ""));
    buttons.back()->setToolTip("QStyle::SP_VistaShield");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_BrowserReload), ""));
    buttons.back()->setToolTip("QStyle::SP_BrowserReload");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_BrowserStop), ""));
    buttons.back()->setToolTip("QStyle::SP_BrowserStop");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaPlay), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaPlay");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaStop), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaStop");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaPause), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaPause");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaSkipForward), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaSkipForward");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaSkipBackward), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaSkipBackward");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaSeekForward), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaSeekForward");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaSeekBackward), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaSeekBackward");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaVolume), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaVolume");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_MediaVolumeMuted), ""));
    buttons.back()->setToolTip("QStyle::SP_MediaVolumeMuted");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_LineEditClearButton), ""));
    buttons.back()->setToolTip("QStyle::SP_LineEditClearButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogYesToAllButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogYesToAllButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogNoToAllButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogNoToAllButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogSaveAllButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogSaveAllButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogAbortButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogAbortButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogRetryButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogRetryButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_DialogIgnoreButton), ""));
    buttons.back()->setToolTip("QStyle::SP_DialogIgnoreButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_RestoreDefaultsButton), ""));
    buttons.back()->setToolTip("QStyle::SP_RestoreDefaultsButton");
    buttons.push_back(new QPushButton(style()->standardIcon(QStyle::SP_TabCloseButton), ""));
    buttons.back()->setToolTip("QStyle::SP_TabCloseButton");

    QGridLayout* layout = new QGridLayout;
    int i = 0, j = 0;
    for (auto x : buttons)
    {
        layout->addWidget(x, i, j);
        qDebug() << i << ", " << j;
        j++;

        if (j == 9)
            j = 0, i++;
    }
    setLayout(layout);
}