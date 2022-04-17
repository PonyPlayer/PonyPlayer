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


class HurricaneRenderer : public QObject, public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions_3_3_Core {
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
    HurricaneRenderer();

    ~HurricaneRenderer() override;

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

protected:
    void render() override;
};


class Hurricane : public QQuickFramebufferObject {
    Q_OBJECT
    QML_ELEMENT
private:

public:
    Hurricane(QQuickItem *parent = nullptr);


    [[nodiscard]] Renderer *createRenderer() const override;

    ~Hurricane() noexcept override;


};

#endif // SQUIRCLE_H
