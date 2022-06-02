//
// Created by ColorsWind on 2022/5/7.
//
#pragma once
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
#include <utility>
#include "updatevalue.hpp"
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

struct RenderSettings {
    // For memory visibility, sync when both GUI and render threads are blocked.
    // We have to RenderSettings, one can be modified on GUI thread (e.g. by QML),
    // and the other will be synchronized in SYNC stage.
    UpdateValueVideoFrameRef videoFrame;
    UpdateValue<GLfloat> brightness{0.0F};
    UpdateValue<GLfloat> contrast{1.0F};
    UpdateValue<GLfloat> saturation{1.0F};
    UpdateValue<QImage> lutFilter;
    UpdateValue<bool> keepFrameRate{true};

    void updateBy(RenderSettings &settings) {
        videoFrame.updateBy(settings.videoFrame);
        brightness.updateBy(settings.brightness);
        contrast.updateBy(settings.contrast);
        saturation.updateBy(settings.saturation);
        lutFilter.updateBy(settings.lutFilter);
        keepFrameRate.updateBy(settings.keepFrameRate);
    }

};


// QuickItem lives in the GUI thread and the rendering potentially happens on the render thread.
// QuickItem may be deleted on the GUI thread while the render thread is rendering.
// Therefore, we need to separate those two objects.
class FireworksRenderer : public QObject, public QSGRenderNode, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
    Q_PROPERTY(GLfloat brightness READ getBrightness WRITE setBrightness)
    Q_PROPERTY(GLfloat contrast READ getContrast WRITE setContrast)
    Q_PROPERTY(GLfloat saturation READ getSaturation WRITE setSaturation)
    Q_PROPERTY(bool keepFrameRate READ isKeepFrameRate WRITE setKeepFrameRate)
    friend class Fireworks;

PONY_GUARD_BY(MAIN) private:
    // properties

    [[nodiscard]] bool isKeepFrameRate() const { return mainSettings.keepFrameRate; }

    void setKeepFrameRate(bool keep) { mainSettings.keepFrameRate = keep; }

    [[nodiscard]] GLfloat getBrightness() const { return mainSettings.brightness; }

    void setBrightness(GLfloat brightness) { mainSettings.brightness = brightness; }

    [[nodiscard]] GLfloat getSaturation() const { return mainSettings.saturation; }

    void setSaturation(GLfloat saturation) { mainSettings.saturation = saturation; }

    [[nodiscard]] GLfloat getContrast() const { return mainSettings.contrast; }

    void setContrast(GLfloat contrast) { mainSettings.contrast = contrast; }


private:
    QOpenGLShaderProgram *program = nullptr; // late init
    QMatrix4x4 viewMatrix;

    PONY_GUARD_BY(MAIN)   RenderSettings mainSettings;
    PONY_GUARD_BY(RENDER) RenderSettings renderSettings;

    void inline createTextureBuffer(GLuint *texture) {
        QOpenGLFunctions_3_3_Core::glGenTextures(1, texture);
        QOpenGLFunctions_3_3_Core::glBindTexture(GL_TEXTURE_2D, *texture);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        QOpenGLFunctions_3_3_Core::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

private:
    // opengl location
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLuint textureY = 0, textureU = 0, textureV = 0, textureLUT = 0;
    GLint brightnessLoc = -1, contrastLoc = -1,  saturationLoc = -1;
public  slots:
    void init() {
        if (program) { return; } // already initialized
        program = new QOpenGLShaderProgram;
        QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();
        static bool info = false;
        if (!info) {
            qInfo() << "OpenGL version:"
                    << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
            qInfo() << "GSLS version:"
                    << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
            qInfo() << "Vendor:" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            qInfo() << "Renderer:" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            info = true;
        }
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, u":/player/shader/vertex.vsh"_qs);
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, u":/player/shader/fragment.fsh"_qs);
        program->link();
        program->bind();
        brightnessLoc = program->uniformLocation("brightness");
        contrastLoc = program->uniformLocation("contrast");
        saturationLoc = program->uniformLocation("saturation");

//        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
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
        program->setUniformValue("tex_lut", 3);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        createTextureBuffer(&textureY);
        glActiveTexture(GL_TEXTURE1);
        createTextureBuffer(&textureU);
        glActiveTexture(GL_TEXTURE2);
        createTextureBuffer(&textureV);
        glActiveTexture(GL_TEXTURE3);
        createTextureBuffer(&textureLUT);
        glBindTexture(GL_TEXTURE_2D, textureLUT);
    };

    void sync() {
        renderSettings.updateBy(mainSettings);
    }
public:
    PONY_GUARD_BY(MAIN) FireworksRenderer() {
        qDebug() << "Create Hurricane Renderer:" << static_cast<void *>(this) << ".";
    }

    PONY_GUARD_BY(MAIN) bool setVideoFrame(const VideoFrameRef &frame) {
        if (static_cast<const VideoFrameRef&>(mainSettings.videoFrame) == frame) {
            return false;
        } {
            mainSettings.videoFrame = frame;
            return true;
        }
    }

    PONY_GUARD_BY(MAIN) void setLUTFilter(const QImage& image) {
        mainSettings.lutFilter = image;
    }


public:

    [[nodiscard]] RenderingFlags flags() const override {
        return BoundedRectRendering;
    }

    [[nodiscard]] StateFlags changedStates() const override {
        return BlendState | ScissorState | StencilState;
    }

    void render(const RenderState *state) override  {
        // call on render thread

        // Due to QTBUG-97589, we are not able to get model-view matrix
        // https://bugreports.qt.io/browse/QTBUG-97589
        // workaround, assume parent clip hurricane
        const QRect r = state->scissorRect();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (state->scissorEnabled()) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(r.x(), r.y(), r.width(), r.height());
        } else {
            ILLEGAL_STATE("Scissor Test must be enabled. For example: wrap Fireworks "
                                     "in a Rectangle and set clip: true. ");
        }
        if (state->stencilEnabled()) {
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, state->stencilValue(), 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }


        program->bind();

        program->setUniformValue(saturationLoc, renderSettings.saturation.getUpdate());
        program->setUniformValue(contrastLoc, renderSettings.contrast.getUpdate());
        program->setUniformValue(brightnessLoc, renderSettings.brightness.getUpdate());
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        const VideoFrameRef &videoFrame = renderSettings.videoFrame;
        if (!videoFrame.isValid()) { return; }
        int lineSize = videoFrame.getLineSize();
        int imageHeight = videoFrame.getHeight();
        int imageWidth = videoFrame.getWidth();
        auto *imageY = videoFrame.getY();
        auto *imageU = videoFrame.getU();
        auto *imageV = videoFrame.getV();
        if (renderSettings.keepFrameRate) {
            double rate = static_cast<double>(imageHeight) / static_cast<double>(imageWidth);
            double h = r.width() * rate;
            if (h >= r.height()) {
                double wFit = r.height() / rate;
                double wPad = r.width() - wFit;
                glViewport(r.x() + static_cast<GLsizei>(wPad) / 2, r.y(), static_cast<GLsizei>(wFit), r.height());
            } else {
                double hFit = r.width() * rate;
                double hPad =  r.height() - hFit;
                glViewport(r.x(), r.y()  + static_cast<GLsizei>(hPad) / 2, r.width(), static_cast<GLsizei>(hFit));
            }
        }


        QImage lutTexture = renderSettings.lutFilter;

        if (renderSettings.videoFrame.isUpdateSize()) {
            // since FFmpeg may pad frame to align, we need to clip invalid data
            viewMatrix.setToIdentity();
            viewMatrix.ortho(0, static_cast<float>(imageWidth) / static_cast<float>(lineSize), 0, 1, -1, 1);
//            viewMatrix.translate(0, 0, -0.7F);
            program->setUniformValue("view", viewMatrix);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureY);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, imageY);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureU);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize / 2, imageHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageU);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textureV);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize / 2, imageHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageV);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, textureLUT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lutTexture.width(), lutTexture.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, lutTexture.constBits());
        } else if (renderSettings.videoFrame.isUpdate()) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureY);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize, imageHeight, GL_RED, GL_UNSIGNED_BYTE, imageY);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureU);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize / 2, imageHeight / 2, GL_RED, GL_UNSIGNED_BYTE, imageU);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textureV);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize / 2, imageHeight / 2, GL_RED, GL_UNSIGNED_BYTE, imageV);
            glBindTexture(GL_TEXTURE_2D, textureLUT);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, textureLUT);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lutTexture.width(), lutTexture.height(), GL_RED, GL_UNSIGNED_BYTE, lutTexture.constBits());
        }
        glDrawElements(GL_TRIANGLES, sizeof(VERTEX_INDEX) / sizeof(GLuint), GL_UNSIGNED_INT, ZERO_OFFSET);
        program->release();
    }

    ~FireworksRenderer() override {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        program->release();
        qDebug() << "Deconstruct Hurricane Renderer:" << static_cast<void *>(this) << ".";
    }


};