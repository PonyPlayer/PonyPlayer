//
// Created by ColorsWind on 2022/5/22.
//

#pragma once

#include <QtCore>
#include <QCoreApplication>

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
#endif
    }
}