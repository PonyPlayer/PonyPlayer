//
// Created by ColorsWind on 2022/5/1.
//

#ifndef PONYPLAYER_AUDIOSINK_H
#define PONYPLAYER_AUDIOSINK_H

enum class PlaybackState {
    PLAYING, ///< 正在播放
    STOPPED, ///< 停止状态
    IDLED,   ///< 空闲状态
};

/**
 * @brief 播放音频裸流, 用于代替QAudioSink.
 * 这个类的RAII的.
 */
class PonyAudioSink {
public:
    /**
     * 计算音频长度对应的数据大小
     * @param format 音频格式
     * @param duration 音频时长(单位: ms)
     * @return 数据大小(单位: byte)
     */
    static size_t durationToSize(const QAudioFormat &format, int duration);

    /**
     * 计算音频数据长度对应的数据大小
     * @param format 音频格式
     * @param duration 数据大小(单位: byte)
     * @return 音频时长(单位: ms)
     */
    static size_t sizeToDuration(const QAudioFormat &format, int duration);

    /**
     * 创建PonyAudioSink并attach到默认设备上
     * @param systemBufferSize 系统buffer大小
     * @param audioBufferSize PonyAudioSink的缓存大小
     * @param format 音频格式
     */
    PonyAudioSink(size_t systemBufferSize, size_t audioBufferSize, QAudioFormat format);

    /**
     * 析构即从deattach当前设备
     */
    ~PonyAudioSink();

    /**
     * 开始播放
     */
    void start();

    /**
     * 尽快停止播放
     */
    void stop();

    /**
     * 放弃缓冲区的内容, 立即停止播放
     */
    void abort();

    /**
     * 获取播放状态
     * @return
     */
    PlaybackState state() const;


    /**
     * 获取AudioBuffer剩余空间
     * @return 剩余空间(单位: byte)
     */
    size_t freeByte() const;

    /**
     * 写AudioBuffer, 要么写入完全成功, 要么失败. 这个操作保证在VideoThread上进行.
     * @param buf 数据源
     * @param len 长度(单位: byte)
     * @return 写入是否成功
     */
    bool write(const char* buf, size_t len);

    /**
     * 清空AudioBuffer, 将所有空间标记为可用. 这个操作保证在VideoThread上进行.
     * @return 清空数据长度(单位: byte)
     */
    size_t clear();

//    /**
//     * 获取AudioBuffer数据.
//     * @return
//     */
//    const char* data();

    /**
     * 获取当前播放的时间
     * @return 当前已播放音频的长度(单位: 秒)
     */
    double getProcessSecs() const;

    /**
     * 设置当前播放的时间,
     * @param t 新的播放时间(单位: 秒)
     */
    void setProcessSecs(double t=0.0);
};

#endif //PONYPLAYER_AUDIOSINK_H
