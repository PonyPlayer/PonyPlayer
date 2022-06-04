//
// Created by 彭郑威 on 2022/4/30.
//

#ifndef PONYPLAYER_INFO_ACCESSOR_H
#define PONYPLAYER_INFO_ACCESSOR_H

#include "ponyplayer.h"
INCLUDE_FFMPEG_BEGIN
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
INCLUDE_FFMPEG_END
#include "playlist.h"

class infoAccessor {
public:
    infoAccessor();
    static QString getInfo(QString filename,PlayListItem& res);
};

#endif //PONYPLAYER_INFO_ACCESSOR_H