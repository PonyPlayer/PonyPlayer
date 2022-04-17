#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>

// QuickItem lives in the GUI thread and the rendering potentially happens on the render thread.
// QuickItem may be deleted on the GUI thread while the render thread is rendering.
// Therefore, we need to separate those two objects.
class HurricaneRenderer : public QObject, protected QOpenGLFunctions_3_3_Core {
Q_OBJECT
private:
    QOpenGLShaderProgram program;
    GLuint vao, vbo, ebo, pbo;
    QImage image;
    QSize imageSize;
    QMatrix4x4 viewMatrix;
public slots:
    void init();
    void paint();
public:
    HurricaneRenderer(QQuickWindow *pWindow);

    ~HurricaneRenderer() override;

    QQuickWindow *window;
};


class Hurricane : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
private:
    HurricaneRenderer *renderer = nullptr;
public:
    Hurricane(QQuickItem *parent = nullptr);

    ~Hurricane() noexcept override;

public slots:
    void handleWindowChanged(QQuickWindow *win);
    void sync();
    void initRenderer();
};

#endif // SQUIRCLE_H
