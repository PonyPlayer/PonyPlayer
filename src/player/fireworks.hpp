#pragma once
#include <QQuickItem>
#include <QObject>
#include <QQuickWindow>
#include <QOpenGLShaderProgram>
#include "renderer.hpp"

class VideoFrameRef {
    VideoFrame frame;
};

class Fireworks : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GLfloat brightness READ getBrightness WRITE setBrightness)
    Q_PROPERTY(GLfloat contrast READ getContrast WRITE setContrast)
    Q_PROPERTY(GLfloat saturation READ getSaturation WRITE setSaturation)
private:
    FireworksRenderer *renderer = nullptr;
    bool updateVideoFrame;
protected:
    VideoFrame picture;
    GLfloat brightness = 0.0;
    GLfloat contrast = 1.0;
    GLfloat saturation = 1.0;
protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override {
        return renderer;
    }

public:
    Fireworks(QQuickItem *parent = nullptr): QQuickItem(parent) {
        this->setFlag(QQuickItem::ItemHasContents);
        connect(this, &QQuickItem::windowChanged, this, [=](QQuickWindow *win){
            qDebug() << "Window Size Changed:" << static_cast<void *>(win) << ".";
            if (win) {
                connect(this->window(), &QQuickWindow::beforeSynchronizing, this, &Fireworks::sync, Qt::DirectConnection);
                win->setColor(Qt::black);
            } else {
                qWarning() << "Window destroy.";
            }
        });
        qDebug() << "Create Hurricane QuickItem.";
    }
    ~Fireworks() noexcept override {
        picture.free();
    }

    [[nodiscard]] GLfloat getBrightness() const { return brightness; }

    void setBrightness(GLfloat b) { Fireworks::brightness = b; }

    [[nodiscard]] GLfloat getContrast() const { return brightness; }

    void setContrast(GLfloat c) { Fireworks::contrast = c; qDebug() << "setContrast" << c; };

    [[nodiscard]] GLfloat getSaturation() const { return saturation; };

    void setSaturation(GLfloat s) { Fireworks::saturation = s; };

public slots:

    void sync() {
        // call from renderer thread while GUI thread is blocking
        if (!renderer) {
            renderer = new FireworksRenderer(this);
            connect(window(), &QQuickWindow::beforeRendering, renderer, &FireworksRenderer::init, Qt::DirectConnection);
        }

        // sync state
        if (updateVideoFrame) {
            renderer->setPictureRef(picture);
            picture.makeInvalid();
            updateVideoFrame = false;
        }


        // sync uniform
        renderer->brightness = this->brightness;
        renderer->contrast = this->contrast;
        renderer->saturation = this->saturation;


    }

    void setImage(const VideoFrame &pic) {
        // this function must be called on GUI thread
        // setImage -> sync -> render
        // since picture may use on renderer thread, we CANNOT free now

        // no change, return immediately
        if (pic == picture) { return; }
        updateVideoFrame = true;
        // local picture has not been used, free
        // see sync
        if (picture.isValid()) { picture.free(); }
        // update ref
        picture = pic;
        // make dirty
        this->update();

    }
};


