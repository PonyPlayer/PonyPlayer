#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

class HurricaneRenderer : public QObject, protected QOpenGLFunctions {
Q_OBJECT
public:
    HurricaneRenderer();

    ~HurricaneRenderer() override;

    void setT(qreal t) { m_t = t; }

    void setViewportSize(const QSize &size) { m_viewportSize = size; }

    void setWindow(QQuickWindow *window) { m_window = window; }

public slots:

    void init();

    void paint();

private:
    void makeObject();
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
    QImage image;
    QColor clearColor = Qt::black;
    QPoint lastPos;
    int xRot = 0;
    int yRot = 0;
    int zRot = 0;
    QOpenGLTexture *img_texture = nullptr;
    QOpenGLShaderProgram *program = nullptr;
    QOpenGLBuffer vbo;

};


class Hurricane : public QQuickItem {
Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    QML_ELEMENT

public:
    Hurricane();

    [[nodiscard]] qreal t() const { return m_t; }

    void setT(qreal t);

signals:

    void tChanged();

public slots:

    void sync();

    void cleanup();

    void loadImage(const QUrl &url);

private slots:

    void handleWindowChanged(QQuickWindow *win) const;

private:
    void releaseResources() override;

    qreal m_t;
    HurricaneRenderer *m_renderer;
    QImage image;
};

#endif // SQUIRCLE_H
