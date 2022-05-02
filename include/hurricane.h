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

template<class T>
class UniformValue {
private:
    int location = -1;
    QOpenGLShaderProgram *shaderProgram = nullptr;
    T value = {};
    bool update = false;
public:
    void init(QOpenGLShaderProgram *program, const QString &name) {
        this->shaderProgram = program;
        this->location = program->uniformLocation(name);
    }


    void render() {
        if (this->update) {
            shaderProgram->setUniformValue(location, value);
            this->update = false;
        }
    }


    UniformValue& operator=(T v) {
        update |= this->value != v;
        this->value = v;
        return *this;
    }

};


template class UniformValue<GLfloat>;
// QuickItem lives in the GUI thread and the rendering potentially happens on the render thread.
// QuickItem may be deleted on the GUI thread while the render thread is rendering.
// Therefore, we need to separate those two objects.
class HurricaneRenderer : public QObject, public QSGRenderNode, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
    friend class Hurricane;
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

    UniformValue<GLfloat> brightness;
    UniformValue<GLfloat> contrast;
    UniformValue<GLfloat> saturation;

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
    Q_PROPERTY(GLfloat brightness READ getBrightness WRITE setBrightness)
    Q_PROPERTY(GLfloat contrast READ getContrast WRITE setContrast)
    Q_PROPERTY(GLfloat saturation READ getSaturation WRITE setSaturation)
private:
    HurricaneRenderer *renderer = nullptr;
    std::vector<Picture> cleanupPictureQueue;
protected:
    Picture picture;
    GLfloat brightness = 0.0;
    GLfloat contrast = 1.0;
    GLfloat saturation = 1.0;
public:
    Hurricane(QQuickItem *parent = nullptr);
    ~Hurricane() noexcept override;

    [[nodiscard]] GLfloat getBrightness() const { return brightness; }

    void setBrightness(GLfloat b) { Hurricane::brightness = b; }

    [[nodiscard]] GLfloat getContrast() const { return brightness; }

    void setContrast(GLfloat c) { Hurricane::contrast = c; qDebug() << "setContrast" << c; };

    [[nodiscard]] GLfloat getSaturation() const { return saturation; };

    void setSaturation(GLfloat s) { Hurricane::saturation = s; };
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
