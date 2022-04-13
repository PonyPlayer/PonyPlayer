#include "logger.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
#include <libavformat/avformat.h>

#pragma GCC diagnostic pop
}
void hello()
{
    AVFormatContext *fmt_ctx = avformat_alloc_context();//创建对象并初始化
    int ret=0;
    const char* fileName="*.mp4";//文件地址

    do{
        //打开文件
        if ((ret = avformat_open_input(&fmt_ctx, fileName, NULL, NULL))<0)
            break;//Cannot open video file

        //查找流信息（音频流和视频流）
        if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
            printf("Cannot find stream information\n");
            break;
        }

        av_dump_format(fmt_ctx,0,fileName,0);//输出视频信息
    }while(0);

    avformat_close_input(&fmt_ctx);//关闭文件
}
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qInstallMessageHandler(logMessageHandler);

    const QUrl url(u"qrc:/view/main.qml"_qs);
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);

    hello();
    return QGuiApplication::exec();
}


