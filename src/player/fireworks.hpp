#pragma once
#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#ifdef __APPLE__
// suppress apple warning
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#undef GL_SILENCE_DEPRECATION
#endif

#include <QQuickItem>
#include <QQuickWindow>
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
#include "frame.hpp"

const static void* ZERO_OFFSET = nullptr;

const static GLfloat VERTEX_POS[] = {
        1, 1, 0, 1, 0,
        1, 0, 0, 1, 1,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 1,
};
const static GLuint VERTEX_INDEX[] = {
        0, 1, 2,
        1, 2, 3,
};


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
class FireworksRenderer : public QObject, public QSGRenderNode, protected QOpenGLFunctions_3_3_Core {
Q_OBJECT
    friend class Fireworks;
private:
    QOpenGLShaderProgram *program = nullptr; // late init
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLuint textureY = 0, textureU = 0, textureV = 0;
    std::byte * imageY = nullptr, *imageU = nullptr, *imageV = nullptr;
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
    void init() {
        if (program) {
            // already initialized
            return;
        }
        program = new QOpenGLShaderProgram;
        initializeOpenGLFunctions();
        qDebug() << "OpenGL version:"
                 << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        qDebug() << "GSLS version:"
                 << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
        qDebug() << "Vendor:" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        qDebug() << "Renderer:" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, u":/render/shader/vertex.vsh"_qs);
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, u":/render/shader/fragment.fsh"_qs);
        program->link();
        program->bind();
        brightness.init(program, "brightness");
        contrast.init(program, "contrast");
        saturation.init(program, "saturation");

        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);


        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_POS), VERTEX_POS, GL_STATIC_DRAW);

        glBindVertexArray(vao);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), ZERO_OFFSET);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                              reinterpret_cast<const void *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VERTEX_INDEX), VERTEX_INDEX, GL_STATIC_DRAW);

        program->setUniformValue("tex_y", 0);
        program->setUniformValue("tex_u", 1);
        program->setUniformValue("tex_v", 2);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        createTextureBuffer(&textureY);
        glActiveTexture(GL_TEXTURE1);
        createTextureBuffer(&textureU);
        glActiveTexture(GL_TEXTURE2);
        createTextureBuffer(&textureV);
    };

public:
    FireworksRenderer(QQuickItem *item)  : quickItem(item) {

        qDebug() << "Create Hurricane Renderer:" << static_cast<void *>(this) << ".";
    }
    void render(const RenderState *state) override  {

        // call on render thread
        if (!imageY || !imageU || !imageV) { return; }
        // since QuickItem position is relative position of its parent, we need to convert to scene coordinate
        QPointF qtPos = quickItem->mapToScene(quickItem->position());
        // scale to physical pixel
        qreal ratio =  quickItem->window()->devicePixelRatio();
        QRect glRect = {
                static_cast<int>(qtPos.x() * ratio),
                // in qt cartesian coordinate system, (0, 0) is at the left top corner
                // while in OpenGL coordinate system, (0, 0) is at the left bottom corner
                static_cast<int>((quickItem->window()->height() - quickItem->height() - qtPos.y()) * ratio),
                static_cast<int>(quickItem->width() * ratio),
                static_cast<int>(quickItem->height() * ratio),
        };
//    quickItem->window()->beginExternalCommands();
#ifdef DEBUG_FlAG_PAINT_LOG
        qDebug() << "Paint" << "x =" << glRect.x() << "y =" << glRect.y() << "w =" << glRect.width() << "h =" << glRect.height();
#endif
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(glRect.x(), glRect.y(), glRect.width(), glRect.height());

//    glScissor(glRect.x(), glRect.y(), glRect.width(), glRect.height());
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        program->bind();
        brightness.render();
        contrast.render();
        saturation.render();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        if (flagUpdateImageSize) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureY);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, imageY);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureU);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize / 2, imageHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageU);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textureV);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize / 2, imageHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageV);
            // since FFmpeg may pad frame to align, we need to clip invalid data
            viewMatrix.setToIdentity();
            viewMatrix.ortho(0, static_cast<float>(imageWidth) / static_cast<float>(lineSize), 0, 1, -1, 1);
            program->setUniformValue("view", viewMatrix);
        } else if (flagUpdateImageContent) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureY);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize, imageHeight, GL_RED, GL_UNSIGNED_BYTE, imageY);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureU);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize / 2, imageHeight / 2, GL_RED, GL_UNSIGNED_BYTE, imageU);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textureV);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize / 2, imageHeight / 2, GL_RED, GL_UNSIGNED_BYTE, imageV);
        }
        glDrawElements(GL_TRIANGLES, sizeof(VERTEX_INDEX) / sizeof(GLuint), GL_UNSIGNED_INT, ZERO_OFFSET);
        glDisable(GL_SCISSOR_TEST);
        program->release();
//    quickItem->window()->endExternalCommands();
    }

    ~FireworksRenderer() override {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        program->release();
        qDebug() << "Deconstruct Hurricane Renderer:" << static_cast<void *>(this) << ".";
    }

    void setImageView(const VideoFrame &pic) {
        // should call on sync stage
        flagUpdateImageContent = true;
        imageY = pic.getY();
        imageU = pic.getU();
        imageV = pic.getV();
        if (pic.getWidth() != imageWidth || pic.getHeight() != imageHeight || pic.getLineSize() != lineSize) {
            imageWidth = pic.getWidth();
            imageHeight = pic.getHeight();
            lineSize = pic.getLineSize();
            flagUpdateImageSize = true;
        }
    }
};


class Fireworks : public QQuickItem {
Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GLfloat brightness READ getBrightness WRITE setBrightness)
    Q_PROPERTY(GLfloat contrast READ getContrast WRITE setContrast)
    Q_PROPERTY(GLfloat saturation READ getSaturation WRITE setSaturation)
private:
    FireworksRenderer *renderer = nullptr;
    std::vector<VideoFrame> cleanupPictureQueue;
protected:
    VideoFrame picture;
    GLfloat brightness = 0.0;
    GLfloat contrast = 1.0;
    GLfloat saturation = 1.0;
public:
    Fireworks(QQuickItem *parent = nullptr) {
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
        for(auto && pic : cleanupPictureQueue) {
            pic.free();
        }
    }

    [[nodiscard]] GLfloat getBrightness() const { return brightness; }

    void setBrightness(GLfloat b) { Fireworks::brightness = b; }

    [[nodiscard]] GLfloat getContrast() const { return brightness; }

    void setContrast(GLfloat c) { Fireworks::contrast = c; qDebug() << "setContrast" << c; };

    [[nodiscard]] GLfloat getSaturation() const { return saturation; };

    void setSaturation(GLfloat s) { Fireworks::saturation = s; };
protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override {
        return renderer;
    }

public slots:

    void sync() {
        // call from renderer thread while GUI thread is blocking
        if (!renderer) {
            renderer = new FireworksRenderer(this);
            connect(window(), &QQuickWindow::beforeRendering, renderer, &FireworksRenderer::init, Qt::DirectConnection);
            //
            //        connect(window(), &QQuickWindow::afterRenderPassRecording, renderer, &HurricaneRenderer::paint, Qt::DirectConnection);
            connect(window(), &QQuickWindow::afterRendering, this, &Fireworks::cleanupPicture);
        }
        // sync state
        if (picture.isValid()) {
            renderer->setImageView(picture);
        }
        // sync uniform
        renderer->brightness = this->brightness;
        renderer->contrast = this->contrast;
        renderer->saturation = this->saturation;
    }

    void cleanupPicture() {
        // call on GUI thread
        for(auto && pic : cleanupPictureQueue) {
            pic.free();
        }
        cleanupPictureQueue.clear();
    }


    void setImage(const VideoFrame &pic) {
        // this function must be called on GUI thread
        // setImage -> sync -> render
        // since picture may use on renderer thread, we CANNOT free now
        if (pic != picture) {
            cleanupPictureQueue.push_back(picture);
            picture = pic;
            // make dirty
            this->update();
        }

    }
};






#endif // SQUIRCLE_H
