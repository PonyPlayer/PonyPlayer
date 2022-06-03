//
// Created by ColorsWind on 2022/5/22.
//

#pragma once

#include <QtCore>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDesktopServices>

#ifdef __APPLE__
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFBundle.h>
#endif


namespace PonyPlayer {
    inline QString getAssetsDir() {
#ifdef __APPLE__
        CFURLRef resourcesDirUrl = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        CFURLRef resourcesDirAbsoluteUrl = CFURLCopyAbsoluteURL(resourcesDirUrl);
        CFStringRef macPath = CFURLCopyFileSystemPath(resourcesDirAbsoluteUrl, kCFURLPOSIXPathStyle);
        QString path = QString::fromCFString(macPath);
        CFRelease(resourcesDirUrl);
        CFRelease(resourcesDirAbsoluteUrl);
        CFRelease(macPath);
        return path;
#elif WIN32
        return QCoreApplication::applicationDirPath().append(u"/assets"_qs);
#elif defined(__linux__)
        return QStandardPaths::locate(QStandardPaths::AppDataLocation, "assets", QStandardPaths::LocateDirectory);
#endif
    }

    inline void showFileInExplorer(const QString& filePath) {
#ifdef Q_OS_MAC
        QStringList arguments;
        arguments.append("-e");
        arguments.append("tell application \"Finder\"");
        arguments.append("-e");
        arguments.append("activate");
        arguments.append("select POSIX file \""+ filePath +"\"");
        arguments.append("-e");
        arguments.append("end tell");
        QProcess::startDetached("osascript", arguments);
#elif defined(Q_OS_WIN32)
        program = "explorer.exe";
        arguments.append("/select,");
        arguments.append(QDir::toNativeSeparators(filePath));
        QProcess::startDetached("explorer.exe", arguments);
#elif defined(Q_OS_LINUX)
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
#endif
    }
}