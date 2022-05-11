//
// Created by 彭郑威 on 2022/4/30.
//

#ifndef PONYPLAYER_INFO_ACCESSOR_H
#define PONYPLAYER_INFO_ACCESSOR_H
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
<<<<<<< HEAD
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
=======
>>>>>>> 80995cf666d29295cf21f8b66b611f5fc3cd99fc
}
#include "playlist.h"

class infoAccessor {
public:
    infoAccessor();
    static void getInfo(QString filename,PlayListItem& res);
};

<<<<<<< HEAD
#endif //PONYPLAYER_INFO_ACCESSOR_H
=======
#endif //PONYPLAYER_INFO_ACCESSOR_H
>>>>>>> 80995cf666d29295cf21f8b66b611f5fc3cd99fc
