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
#include <QVector>
#include <QSGSimpleTextureNode>
#include <QSGRenderNode>
#include "frame_queue.h"

// QuickItem lives in the GUI thread and the rendering potentially happens on the render thread.
// QuickItem may be deleted on the GUI thread while the render thread is rendering.
// Therefore, we need to separate those two objects.
class HurricaneRenderer : public QObject, public QSGRenderNode, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    QOpenGLShaderProgram *program = nullptr; // late init
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLuint textureY = 0, textureU = 0, textureV = 0;
    GLubyte * imageY = nullptr, *imageU = nullptr, *imageV = nullptr;
    QMatrix4x4 viewMatrix;
    QQuickItem *quickItem;

    // update flag
    GLsizei imageWidth;
    GLsizei imageHeight;
    GLsizei lineSize;
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

public:
    HurricaneRenderer(QQuickItem *item);
    void render(const RenderState *state) override;
    ~HurricaneRenderer() override;

    void setImageView(const Picture &pic);
};


class Hurricane : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
private:
    HurricaneRenderer *renderer = nullptr;
    std::vector<Picture> cleanupPictureQueue;
protected:
    Picture picture;
public:
    Hurricane(QQuickItem *parent = nullptr);
    ~Hurricane() noexcept override;


protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

public slots:
    void handleWindowChanged(QQuickWindow *win);
    void sync();
    void cleanupPicture();


    void setImage(const Picture &pic) {
        // this function must be called on GUI thread
        // setImage -> sync -> render
        // since picture may use on renderer thread, we CANNOT free now
        if (pic.frame != picture.frame) {
            cleanupPictureQueue.push_back(picture);
            picture = pic;
            // make dirty
            this->update();
        }

    }
};

#endif // SQUIRCLE_H
