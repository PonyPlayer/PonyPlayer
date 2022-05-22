//
// Created by ColorsWind on 2022/5/22.
//

#pragma once
#include <QtCore>
#include <QCoreApplication>


namespace PonyPlayer {
    inline QString getAssetsDir() {
#ifdef __APPLE__
        CFURLRef appUrlRef = CFBundleCopyBundleURL( CFBundleGetMainBundle() );
    CFStringRef macPath = CFURLCopyFileSystemPath( appUrlRef, kCFURLPOSIXPathStyle );
    QString path = QString::fromCFString(macPath);
    CFRelease(appUrlRef);
    CFRelease(macPath);
    return path;
#elif WIN32
        return QCoreApplication::applicationDirPath().append(u"/assets"_qs);
#endif
    }
}