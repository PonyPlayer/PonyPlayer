//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer.h"

#include <QUrl>
#include <QTimer>
#include <QDir>


//#define DEBUG_FLAG_AUTO_OPEN

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent), videoPlayWorker() {
    videoThread = new QThread;
    videoThread->setObjectName("VideoThread");
    videoPlayWorker.moveToThread(videoThread);
    videoThread->start();
    connect(this, &HurricanePlayer::signalPlayerInitializing, &videoPlayWorker, &VideoPlayWorker::slotThreadInit);
    connect(this, &HurricanePlayer::signalResume, &videoPlayWorker, &VideoPlayWorker::slotOnWork);
    connect(this, &HurricanePlayer::signalPause, &videoPlayWorker, &VideoPlayWorker::slotPause);
    connect(this, &HurricanePlayer::signalVolumeChanging, &videoPlayWorker, &VideoPlayWorker::slotVolumeChanged);
    connect(this, &HurricanePlayer::signalOpenFile, &videoPlayWorker, &VideoPlayWorker::slotOpenFile);
    connect(this, &HurricanePlayer::signalClose, &videoPlayWorker, &VideoPlayWorker::slotClose);
    connect(&videoPlayWorker, &VideoPlayWorker::signalImageChanged, this, &HurricanePlayer::setImage);
    connect(&videoPlayWorker, &VideoPlayWorker::signalStateChanged, this, &HurricanePlayer::slotStateChanged);
    connect(&videoPlayWorker, &VideoPlayWorker::signalVolumeChanged, this, &HurricanePlayer::slotVolumeChanged);

    // seek
    connect(this, &HurricanePlayer::signalSeek, &videoPlayWorker, &VideoPlayWorker::slotSeek);
    connect(&videoPlayWorker, &VideoPlayWorker::signalPositionChangedBySeek, this, &HurricanePlayer::slotPositionChangedBySeek);

    // debug
    connect(videoThread, &QThread::destroyed, []{ qDebug() << "Video Thread delete.";});
    connect(this, &HurricanePlayer::stateChanged, [=]{
        qDebug() << "State Changed to" << QVariant::fromValue(state).toString();});
    emit signalPlayerInitializing(QPrivateSignal());
#ifdef DEBUG_FLAG_AUTO_OPEN
    openFile(QUrl::fromLocalFile(QDir::homePath().append(u"/581518754-1-208.mp4"_qs)).url());
#endif
}


void HurricanePlayer::openFile(const QString &path, bool autoClose) {
    if (autoClose && state == HurricaneState::PAUSED) {
        emit close();
    }
    if (state == HurricaneState::CLOSING || state == HurricaneState::INVALID) {
        if (state == HurricaneState::CLOSING) {
            // wait for the operation to complete
            QCoreApplication::processEvents();
        }
        state = HurricaneState::LOADING;
        emit stateChanged();
        emit signalOpenFile(path, QPrivateSignal());
        qDebug() << "HurricanePlayer: Open file:" << path;
    }
}

void HurricanePlayer::start() {
    if (state == HurricaneState::PAUSED || state == HurricaneState::PRE_PAUSE) {
        state = HurricaneState::PLAYING;
        emit stateChanged();
        emit signalResume(QPrivateSignal());
        qDebug() << "HurricanePlayer: Start play video.";
    }
}

void HurricanePlayer::pause() {
    if (state == HurricaneState::PLAYING || state == HurricaneState::PRE_PLAY) {
        state = HurricaneState::PRE_PAUSE;
        emit stateChanged();
        emit signalPause(QPrivateSignal());
        qDebug() << "HurricanePlayer: Pause.";
    }
}


void HurricanePlayer::close() {
    if (state == HurricaneState::PRE_PAUSE || state == HurricaneState::PAUSED) {
        state = HurricaneState::CLOSING;
        emit stateChanged();
        this->setImage(Picture());
        // wait renderer thread to sync to ensure origin picture data is not used anymore
        QCoreApplication::processEvents();
        emit signalClose(QPrivateSignal());
    }
}

HurricanePlayer::~HurricanePlayer() {
    emit signalPause(QPrivateSignal());
    videoThread->quit();
    videoThread->deleteLater();
}

void HurricanePlayer::slotStateChanged(HurricaneState s) {
    // some pre-state maybe be interrupted, ignore.
    if (s == HurricaneState::PLAYING && state != HurricaneState::PRE_PLAY)
        return;
    if (s == HurricaneState::PAUSED && state != HurricaneState::PRE_PAUSE
        && state != HurricaneState::LOADING)
        return;
    if (s == HurricaneState::INVALID && state != HurricaneState::CLOSING)
        return;
    state = s;
    emit stateChanged();
}


time_point VideoPlayWorker::getAudioPlayingUSecs() {
    return audioOutput->elapsedUSecs() - idleDurationSum;
}

void VideoPlayWorker::syncTo(double pts) {
    auto target = static_cast<int64_t>(pts * 1000 * 1000); // us
    auto current = getAudioPlayingUSecs() + seekPoint; // us
    auto duration = target - current;
    qDebug() << "Target" << target / 1000 / 1000;
    qDebug() << "Current" << current / 1000 / 1000;
    if (duration > 10 * 1000L * 100L) {
        qWarning() << "Sleep long duration" << duration << "us";
    }

    if (duration > 0) {
        QThread::usleep(static_cast<unsigned long>(duration));
    } else {
        qWarning() << "Sleep negative duration" << duration << "us";
    }
}

void VideoPlayWorker::onAudioStateChanged(QAudio::State state) {
    auto curr = audioOutput->elapsedUSecs();
    qDebug() << "QAudioSink State changed:" << audioOutput->state() << state << "Curr" << curr << "Sum" << idleDurationSum;
    switch(state) {
        case QAudio::ActiveState:
            if (idlePoint > 0) {
                idleDurationSum += curr - idlePoint;
                idlePoint = -1;
            } else {
                qDebug() << "VideoPlayerWorker: State changed to ActiveState, but fail to determine last time of IdleState or SuspendState.";
                throw std::runtime_error("a");
            }
            break;
        case QAudio::StoppedState:
            if (audioOutput->error() != QAudio::NoError) {
                qWarning() << "VideoPlayerWorker: Error occurs" << audioOutput->error();
            }
            idlePoint = -1;
            break;
        case QAudio::SuspendedState:
        case QAudio::IdleState:
            // Start: IdleState -> ActiveState
            // Pause: ActiveState -> SuspendedState
            // Resume: SuspendedState -> IdleState -> ActiveState
            if (idlePoint <= 0)
                idlePoint = curr;
            break;
    }
}

void VideoPlayWorker::slotThreadInit() {
    // this function must be called on VideoPlayWorker's thread
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);
    audioOutput = new QAudioSink(format, this);
    connect(audioOutput, &QAudioSink::stateChanged, this, &VideoPlayWorker::onAudioStateChanged, Qt::ConnectionType::DirectConnection);
    audioOutput->setVolume(100);
    audioOutput->setBufferSize(19200 * 10);
}

void VideoPlayWorker::slotOnWork() {
    qDebug() << "Start Video Work" << QThread::currentThread()->objectName();
    Picture currFrame;
    pauseRequested = false;
    audioOutput->resume();
    emit signalStateChanged(HurricaneState::PLAYING);
    while(!pauseRequested && (currFrame = demuxer->getPicture(true), currFrame.isValid())) {
        demuxer->popPicture();
        emit signalImageChanged(currFrame);
        for (int i = 0; i < 5 && audioOutput->bytesFree() > 19200; ++i) {
            Sample sample = demuxer->getSample(true);
            audioInput->write(reinterpret_cast<const char *>(sample.data), sample.len);
            demuxer->popSample();
            sample.free();
        }
        // process all events such as setVolume and pause request
        QCoreApplication::processEvents();
        Picture nextFrame = demuxer->getPicture(true);
        if (nextFrame.isValid()) {
            syncTo(nextFrame.pts);
        }

    }
    qDebug() << "Stop work";
    audioOutput->suspend();
    pauseRequested = true;
    emit signalStateChanged(HurricaneState::PAUSED);
}


void VideoPlayWorker::slotOpenFile(const QString &path) {
    seekPoint = 0;
    idleDurationSum = 0;
    audioInput = audioOutput->start();
    audioOutput->suspend();
    QUrl url(path);
    QString localPath = url.toLocalFile();
    qDebug() << "Hurricane Player: Open file" << localPath;
    int ret = demuxer->openFile(localPath.toStdString());
    HurricaneState state;
    if (ret) {
        state = HurricaneState::PAUSED;
        Picture pic = demuxer->getPicture(true);
        emit signalImageChanged(pic);
    } else {
        state = HurricaneState::INVALID;
        qWarning() << "Hurricane Player: Fail to open video." << ret;
    }
    emit signalStateChanged(state);
}

void VideoPlayWorker::slotClose() {
    pauseRequested = true;
    if (audioInput) {
        audioOutput->stop();
        audioInput = nullptr;

    }
    emit signalStateChanged(HurricaneState::INVALID);
}

void VideoPlayWorker::slotSeek(qreal pos) {
    qDebug() << "Receive Seek request" << pos << QThread::currentThread()->objectName();
    // 1. Stop audio and clean buffer (invoke closeAudio)
    // 2. Set seekPoint so syncTo can handle pts correctly
    // 3. Clear invalid frame
    auto t = static_cast<time_point>(pos * 1000 * 1000);
    bool isSuspended = audioOutput->state() == QAudio::SuspendedState;
    seekPoint = t;
    closeAudio();

    // time-consuming task
    demuxer->seek(t);
    while(demuxer->getPicture(true).getPTS() < pos) {
        qDebug() << "Ignore frame" << demuxer->getPicture(true).getPTS();
        demuxer->popPicture();
    }
    qDebug() << "Current Frame" << demuxer->getPicture(true).getPTS();
    while(demuxer->getSample(true).getPTS() < pos) {
        demuxer->popSample();
    }
    qDebug() << "Warning1" << getAudioPlayingUSecs() + seekPoint - t;
    audioInput = audioOutput->start();
    if (isSuspended)
        audioOutput->suspend();
    emit signalPositionChangedBySeek();
    qDebug() << "Finished seek request" << pos;
}

void VideoPlayWorker::slotPause() {
    pauseRequested = true;
    emit signalStateChanged(HurricaneState::PAUSED);
}

void VideoPlayWorker::closeAudio() {
    // close audio and reset idleDurationSum
    audioOutput->stop();
    audioInput = nullptr;
    idleDurationSum = 0;
}
