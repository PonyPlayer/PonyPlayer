//
// Created by ColorsWind on 2022/5/10.
//

#pragma once

#include <QQmlContext>
#include "fireworks.hpp"
#include "hurricane.hpp"
#include "preview.hpp"

class Thumbnail : public Fireworks {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString player READ getPlayer WRITE setPlayer)
private:
    Preview *preview;
    QString player;
    Hurricane *hurricane = nullptr;
public:
    Thumbnail(QQuickItem *parent= nullptr) : Fireworks(parent) {
        preview = new Preview(this);
        connect(this, &Thumbnail::signalPreviewRequest, preview, &Preview::previewRequest);
        connect(this, &QQuickItem::windowChanged, [this](QQuickWindow *window){
            if (!window) { return; }
            auto *context = qmlContext(this);
            while(!hurricane && context) {
                hurricane = reinterpret_cast<Hurricane *>(context->objectForName(player));
                context = context->parentContext();
            }
            if (!hurricane) {
                throw std::runtime_error("Cannot not get Hurricane by id:" + player.toStdString());
            }
            connect(hurricane, &Hurricane::signalOpenFile, preview, &Preview::openFile);
            connect(hurricane, &Hurricane::signalClose, preview, &Preview::close);
            connect(preview, &Preview::previewResponse, this, &Thumbnail::slotPreviewResponse);
        });
    }

    [[nodiscard]] const QString &getPlayer() const {
        return player;
    }

    void setPlayer(const QString &player) {
        Thumbnail::player = player;
        qDebug() << "Set" << player;
    }

    virtual ~Thumbnail() override {
        preview->close();
        preview->deleteLater();
    }
public slots:

    /**
     * 请求预览, 当预览结果准备好之后, 将通过信号通知
     * @param pos 请求预览的位置(单位: s)
     * @see Hurricane::previewResponse
     */
    Q_INVOKABLE void previewRequest(qreal pos) {
        emit signalPreviewRequest(pos, QPrivateSignal());
    }
private slots:
    void slotPreviewResponse(qreal pos, const VideoFrame& frame) {
        setVisible(false);
        setImage(frame);
        emit previewResponse(pos, QPrivateSignal());
    };
signals:
    /**
     * 预览结果图像
     * @param pos 位置(单位: s), 不一定和请求位置一致
     */
    void previewResponse(qreal pos, QPrivateSignal);

    void signalPreviewRequest(qreal pos, QPrivateSignal);

};
