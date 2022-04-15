//
// Created by kurisu on 2022/4/14.
//

#ifndef PONYPLAYER_DEMUXER_H
#define PONYPLAYER_DEMUXER_H

extern "C" {
#include <libavutil/frame.h>
};

/**
 * 初始化demuxer模块
 * @return 返回0表示正常，小于0表示异常
 */
int initDemuxer();

/**
 * 阻塞地读一个视频帧到frame
 * 使用方法：
 *      frame = av_frame_alloc();
 *      ......
 *      for (;;) {
 *          ret = videoFrameQueueFront(frame);
 *          ......
 *          av_frame_unref(frame);
 *      }
 *      ......
 *      av_frame_free(&frame);
 *
 * @param frame 一个有效的AVFrame对象指针
 * @return 1表示读取到frame，0表示帧已全部读取，-1表示异常
 */
int videoFrameQueueFront(AVFrame* frame);

/**
 * 成功读取一个视频帧后需要调用该函数
 */
void videoFrameQueuePop();

/**
 * 阻塞地读一个音频帧到frame
 * 使用方法：
 *      frame = av_frame_alloc();
 *      ......
 *      for (;;) {
 *          ret = audioFrameQueueFront(frame);
 *          ......
 *          av_frame_unref(frame);
 *      }
 *      ......
 *      av_frame_free(&frame);
 *
 * @param frame 一个有效的AVFrame对象指针
 * @return 1表示读取到frame，0表示帧已全部读取，-1表示异常
 */
int audioFrameQueueFront(AVFrame* frame);

/**
 * 成功读取一个音频帧后需要调用该函数
 */
void audioFrameQueuePop();


#endif //PONYPLAYER_DEMUXER_H
