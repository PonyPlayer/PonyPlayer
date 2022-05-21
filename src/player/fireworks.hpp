#pragma once
#include <QQuickItem>
#include <QObject>
#include <QQuickWindow>
#include <QOpenGLShaderProgram>
#include "renderer.hpp"


class Fireworks : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
private:
    FireworksRenderer *renderer;

protected:
    GLfloat brightness = 0.0;
    GLfloat contrast = 1.0;
    GLfloat saturation = 1.0;
protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override {
        return renderer;
    }

public:
    Fireworks(QQuickItem *parent = nullptr): QQuickItem(parent), renderer(new FireworksRenderer) {
        this->setFlag(QQuickItem::ItemHasContents);
        connect(this, &QQuickItem::windowChanged, this, [=](QQuickWindow *win){
            qDebug() << "Window Size Changed:" << static_cast<void *>(win) << ".";
            if (win) {
                connect(this->window(), &QQuickWindow::beforeSynchronizing, renderer, &FireworksRenderer::sync, Qt::DirectConnection);
                connect(this->window(), &QQuickWindow::beforeRendering, renderer, &FireworksRenderer::init, Qt::DirectConnection);
                win->setColor(Qt::black);
            } else {
                qWarning() << "Window destroy.";
            }

        });
        qDebug() << "Create Hurricane QuickItem.";
    }
    ~Fireworks() override {
        renderer = nullptr;
    }

    [[nodiscard]] GLfloat getBrightness() const { return brightness; }

    void setBrightness(GLfloat b) { Fireworks::brightness = b; }

    [[nodiscard]] GLfloat getContrast() const { return brightness; }

    void setContrast(GLfloat c) { Fireworks::contrast = c; qDebug() << "setContrast" << c; };

    [[nodiscard]] GLfloat getSaturation() const { return saturation; };

    void setSaturation(GLfloat s) { Fireworks::saturation = s; };

public slots:



    void setVideoFrame(const VideoFrameRef &pic) {
        // this function must be called on GUI thread
        // setImage -> sync -> render
        // since picture may use on renderer thread, we CANNOT free now
        // no change, return immediately

        if (renderer->setVideoFrame(pic)) {
            // make dirty
            this->update();
        }


    }
};





