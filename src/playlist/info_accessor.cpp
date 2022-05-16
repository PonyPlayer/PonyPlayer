//
// Created by 彭郑威 on 2022/4/30.
//

#include "info_accessor.h"
#include <QDebug>
#include <QImage>

/*
 * 保存帧图像
 */
QString saveImage(QString abspath, QImage& image) {
    QFileInfo fi(abspath);
    QString bn = fi.baseName();
    QString des = QDir::currentPath() + "/preview/" + bn + ".png";
    image.save(des);
    QUrl url = QUrl::fromLocalFile(des);
    return url.toString();
}

void infoAccessor::getInfo(QString filename, PlayListItem& res) {
    AVFormatContext* input_AVFormat_context_ = avformat_alloc_context();
    QUrl url(filename);
    filename = url.toLocalFile();
    if(avformat_open_input(&input_AVFormat_context_, filename.toStdString().c_str(), 0, NULL) < 0){
        qDebug()<<"file open error!";
        return;
    }

    if(avformat_find_stream_info(input_AVFormat_context_, NULL) < 0){
        qDebug()<<"error";
        return;
    }

    // 获取流数量
    res.setStreamNumbers(input_AVFormat_context_->nb_streams);
    // 计算视频长度
    int hours, mins, secs;
    secs = input_AVFormat_context_->duration / 1000000;
    mins = secs / 60;
    secs %= 60;
    hours = mins / 60;
    mins %= 60;

    //格式化视频长度
    char duration_foramt_[128];
    sprintf(duration_foramt_, "%d:%d:%d", hours, mins, secs);
    res.setDuration(QString::fromStdString(duration_foramt_));

    //得到输入视频的封装格式

    qDebug()<<"format: "<<input_AVFormat_context_->streams[0]->codecpar->format;
    qDebug()<<"bit rate: "<<input_AVFormat_context_->bit_rate/1000.0;

    const AVInputFormat* infoFormat = input_AVFormat_context_->iformat;
    res.setFormat(infoFormat->name);

    //分别遍历视频的流
    for(unsigned int i = 0; i < res.getStreamNumbers(); i++){
        //取出一路流,并生成AVStream对象
        AVStream* input_stream = input_AVFormat_context_->streams[i];
        //AVDictionaryEntry *lang = av_dict_get(input_stream->metadata, "language", NULL, 0);
        //std::cout<<"ddd: "<<lang->value<<std::endl;
        //判断是否为视频流
        if(input_stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){


            //avg_frame_rate -> AVRational(有理数),
            //avg_frame_rate.num : 分子
            //avg_frame_rate.den : 母
            //得到视频帧率
            res.setFrameRate(input_stream->avg_frame_rate.num / input_stream->avg_frame_rate.den);

            //取出视频流中的编码参数部分, 生成AVCodecParamters对象
            AVCodecParameters* codec_par = input_stream->codecpar;

            //利用编码参数对象AVCdecParamters得到视频宽度，高度，码率，视频大小
            res.setVideoWidth(codec_par->width);
            res.setVideoHeight(codec_par->height);
            res.setBitRate(codec_par->bit_rate/1000);
            res.setVideoSize(res.getBitRate() * secs / (8.0*1024));

            //利用avcodec_paramters_to_context()函数产生AVCodecContext对象
            //input_stream->codec已经被淘汰，不推荐使用这种方式生成AVCodecContext

            /************************* 获取视频初帧 *************************/

            AVPacket* pkt = av_packet_alloc();
            AVFrame* temp_frame = av_frame_alloc();
            SwsContext* sws_ctx = NULL;
            int rets = 0;
            QImage preview;
            bool preview_done = false;

            AVCodecParameters* codecPr = input_stream->codecpar;
            const AVCodec* pCodec = avcodec_find_decoder(codecPr->codec_id);
            AVCodecContext* pCodecCtx = avcodec_alloc_context3(pCodec);
            avcodec_parameters_to_context(pCodecCtx, codecPr);

            avcodec_open2(pCodecCtx, pCodec, nullptr);

            // 读取视频帧
            while(av_read_frame(input_AVFormat_context_, pkt) >= 0) {
                if(pkt->stream_index == i) {
                    av_frame_unref(temp_frame);

                    while((rets = avcodec_receive_frame(pCodecCtx, temp_frame)) == AVERROR(EAGAIN)) {
                        rets = avcodec_send_packet(pCodecCtx, pkt);
                        if(rets < 0) break;
                    }
                    if(rets < 0 && rets != AVERROR_EOF) continue;
                    int dstH = 240;
                    int dstW = qRound(dstH * (float(temp_frame->width)/float(temp_frame->height)));
                    dstH = (dstH >> 4) << 4;
                    dstW = (dstW >> 4) << 4;
                    sws_ctx = sws_getContext(temp_frame->width,
                                             temp_frame->height,
                                             static_cast<AVPixelFormat>(temp_frame->format),
                                             dstW,
                                             dstH,
                                             static_cast<AVPixelFormat>(AV_PIX_FMT_RGBA),
                                             SWS_FAST_BILINEAR,
                                             nullptr,nullptr,nullptr);
                    int linesize[AV_NUM_DATA_POINTERS];
                    linesize[0] = dstW*4;
                    // 生成图片
                    preview = QImage(dstW, dstH, QImage::Format_RGBA8888);
                    uint8_t* data = preview.bits();
                    sws_scale(sws_ctx,
                              temp_frame->data,
                              temp_frame->linesize,
                              0,
                              temp_frame->height,
                              &data,
                              linesize);
                    sws_freeContext(sws_ctx);

                    avcodec_close(pCodecCtx);
                    avcodec_free_context(&pCodecCtx);
                    preview_done = true;
                    break;
                }

            }
            av_frame_free(&temp_frame);
            av_packet_free(&pkt);
            if(preview_done) {
                QString des = saveImage(filename,preview);
                res.setIconPath(des);
                qDebug()<<"QImage done";
            }
            /****************************** 获取视频初帧 ****************************/

            AVCodecContext* avctx_video;
            avctx_video = avcodec_alloc_context3(NULL);
            int ret = avcodec_parameters_to_context(avctx_video, codec_par);
            if (ret < 0) {
                avcodec_free_context(&avctx_video);
                return;
            }
            //使用AVCodecContext得到视频编码格式（不推荐）
            char buf[128];
            avcodec_string(buf, sizeof(buf), avctx_video, 0);
            //使用AVCodecParameters得到视频编码方式
            res.setVideoFormat(avcodec_get_name((codec_par->codec_id)));

            //判断是否为音频流
        }else if(input_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){

            //生成AVcodecParamters对象
            AVCodecParameters* codec_par = input_stream->codecpar;
            AVCodecContext* avctx_audio;
            avctx_audio = avcodec_alloc_context3(NULL);
            int ret = avcodec_parameters_to_context(avctx_audio, codec_par);
            if(ret < 0){
                avcodec_free_context(&avctx_audio);
                return;
            }

            res.setAudioFormat(avcodec_get_name(avctx_audio->codec_id));

            res.setAudioAverageBitRate(codec_par->bit_rate / 1000);

            res.setChannelNumbers(codec_par->channels);

            res.setSampleRate(codec_par->sample_rate);

            res.setAudioSize(res.getAudioAverageBitRate() * secs / (8.0*1024));
        }
    }
}