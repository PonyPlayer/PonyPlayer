#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#ifdef __APPLE__
// suppress apple warning
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#undef GL_SILENCE_DEPRECATION
#endif

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
#include "frame_queue.h"

// QuickItem lives in the GUI thread and the rendering potentially happens on the render thread.
// QuickItem may be deleted on the GUI thread while the render thread is rendering.
// Therefore, we need to separate those two objects.
class HurricaneRenderer : public QObject, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    QOpenGLShaderProgram *program = nullptr; // late init
    GLuint vao, vbo, ebo, pbos[3];
    GLuint textureY, textureU, textureV;
    GLubyte * imageY, *imageU, *imageV;
    QMatrix4x4 viewMatrix;
    QQuickItem *quickItem;

    // update flag
    QSize imageSize = {};
    char* imageYUV[3];
    bool flagUpdateImageContent = false;
    bool flagUpdateImageSize = false;

    void inline createTextureBuffer(GLuint *texture) {
        QOpenGLFunctions_3_3_Core::glGenTextures(1, texture);
        QOpenGLFunctions_3_3_Core::glBindTexture(GL_TEXTURE_2D, *texture);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
public slots:
    void init();
    void paint();
public:
    HurricaneRenderer(QQuickItem *item);

    ~HurricaneRenderer() override;

    void setImageView(QSize sz, GLubyte *y, GLubyte *u, GLubyte *v);
};


class Hurricane : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
private:
    HurricaneRenderer *renderer = nullptr;
    Frame* frame = nullptr;
public:
    Hurricane(QQuickItem *parent = nullptr);
    ~Hurricane() noexcept override;
    void setImage(Frame *f) {
        frame = f;
        // make dirty
        this->update();
    }

protected:
    void releaseResources() override;


public slots:
    void handleWindowChanged(QQuickWindow *win);
    void sync();
    void cleanup();
};

#endif // SQUIRCLE_H
