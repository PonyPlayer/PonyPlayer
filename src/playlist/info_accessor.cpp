//
// Created by 彭郑威 on 2022/4/30.
//

#include "info_accessor.h"
#include <QDebug>

void infoAccessor::getInfo(QString filename, PlayListItem& res) {
    AVFormatContext* input_AVFormat_context_ = avformat_alloc_context();
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