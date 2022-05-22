#pragma once
#include <QQuickItem>
#include <QObject>
#include <QQuickWindow>
#include <QOpenGLShaderProgram>
#include "renderer.hpp"
#include "platform.hpp"


class Fireworks : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GLfloat brightness READ getBrightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(GLfloat contrast READ getContrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(GLfloat saturation READ getSaturation WRITE setSaturation NOTIFY saturationChanged)
private:
    FireworksRenderer *renderer;

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

    PONY_GUARD_BY(MAIN) [[nodiscard]] GLfloat getBrightness() const { return renderer->getBrightness(); }

    PONY_GUARD_BY(MAIN) void setBrightness(GLfloat b) {
        renderer->setBrightness(b);
        emit brightnessChanged();
    }

    PONY_GUARD_BY(MAIN) [[nodiscard]] GLfloat getContrast() const {
        return renderer->getContrast();
    }

    PONY_GUARD_BY(MAIN) void setContrast(GLfloat c) {
        renderer->setContrast(c);
        emit contrastChanged();
    };

    PONY_GUARD_BY(MAIN) [[nodiscard]] GLfloat getSaturation() const { return renderer->getSaturation(); };

    PONY_GUARD_BY(MAIN) void setSaturation(GLfloat s) {
        renderer->setSaturation(s);
        emit saturationChanged();
    };

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

signals:
    void brightnessChanged();

    void contrastChanged();

    void saturationChanged();
};





