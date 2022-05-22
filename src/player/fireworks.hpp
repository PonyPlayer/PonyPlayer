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
    Q_PROPERTY(QString filterPrefix READ getFilterPrefix)
    Q_PROPERTY(QStringList filterJsons READ getFilterJsons)
private:
    FireworksRenderer *m_renderer;
    QString m_filterPrefix;
    QStringList m_filterJsons;

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override {
        return m_renderer;
    }

public:
    Fireworks(QQuickItem *parent = nullptr): QQuickItem(parent), m_renderer(new FireworksRenderer),
        m_filterPrefix(PonyPlayer::getAssetsDir() + u"/filters"_qs), m_filterJsons() {
        QDir filterDir(m_filterPrefix);
        for(auto && filename : filterDir.entryList({"*.json"})) {
            QFile file = filterDir.filePath(filename);
            file.open(QIODevice::OpenModeFlag::ReadOnly);
            m_filterJsons.append(file.readAll());
            file.close();
        }
        this->setFlag(QQuickItem::ItemHasContents);
        connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow *win){
            qDebug() << "Window Size Changed:" << static_cast<void *>(win) << ".";
            if (win) {
                connect(this->window(), &QQuickWindow::beforeSynchronizing, m_renderer, &FireworksRenderer::sync, Qt::DirectConnection);
                connect(this->window(), &QQuickWindow::beforeRendering, m_renderer, &FireworksRenderer::init, Qt::DirectConnection);
                win->setColor(Qt::black);
            } else {
                qWarning() << "Window destroy.";
            }

        });
        qDebug() << "Create Hurricane QuickItem.";
    }
    ~Fireworks() override {
        m_renderer = nullptr;
    }

    PONY_GUARD_BY(MAIN) [[nodiscard]] QString getFilterPrefix() const { return m_filterPrefix; }

    PONY_GUARD_BY(MAIN) [[nodiscard]] QStringList getFilterJsons() const { return m_filterJsons; }

    PONY_GUARD_BY(MAIN) [[nodiscard]] GLfloat getBrightness() const { return m_renderer->getBrightness(); }

    PONY_GUARD_BY(MAIN) void setBrightness(GLfloat b) {
        m_renderer->setBrightness(b);
        emit brightnessChanged();
    }

    PONY_GUARD_BY(MAIN) [[nodiscard]] GLfloat getContrast() const {
        return m_renderer->getContrast();
    }

    PONY_GUARD_BY(MAIN) void setContrast(GLfloat c) {
        m_renderer->setContrast(c);
        emit contrastChanged();
    };

    PONY_GUARD_BY(MAIN) [[nodiscard]] GLfloat getSaturation() const { return m_renderer->getSaturation(); };

    PONY_GUARD_BY(MAIN) void setSaturation(GLfloat s) {
        m_renderer->setSaturation(s);
        emit saturationChanged();
    };

public slots:

    void setVideoFrame(const VideoFrameRef &pic) {
        // this function must be called on GUI thread
        // setImage -> sync -> render
        // since picture may use on renderer thread, we CANNOT free now
        // no change, return immediately

        if (m_renderer->setVideoFrame(pic)) {
            // make dirty
            this->update();
        }
    }

    /**
     * 设置LUT滤镜路径
     * @param path
     */
    Q_INVOKABLE void setLUTFilter(const QString& path) {
        QImage image;
        if (!path.isEmpty()) {
            image.load(QDir(m_filterPrefix).filePath(path));
        }
        image.convertTo(QImage::Format_RGB888);
        m_renderer->setLUTFilter(image);
    }

signals:
    void brightnessChanged();

    void contrastChanged();

    void saturationChanged();
};





