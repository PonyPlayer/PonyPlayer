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
    Q_PROPERTY(QImage imageView READ getImageView WRITE setImageView)
private:
    QOpenGLShaderProgram *program = nullptr; // late init
    GLuint vao, vbo, ebo, pbo;
    QMatrix4x4 viewMatrix;
    QQuickItem *quickItem;

    // update flag
    QSize imageSize = {};
    QImage imageView;
    bool flagUpdateImageContent = false;
    bool flagUpdateImageSize = false;
public:
    const QImage &getImageView() const;

    void setImageView(const QImage &imageView);

public slots:
    void init();
    void paint();
public:
    HurricaneRenderer(QQuickItem *item);

    ~HurricaneRenderer() override;
};


class Hurricane : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
private:
    HurricaneRenderer *renderer = nullptr;
    QImage image;
public:
    Hurricane(QQuickItem *parent = nullptr);
    ~Hurricane() noexcept override;
    const QImage &getImage() {
        return image;
    }
    void setImage(const QImage &img) {
        image = img;
        this->update();
//        this->window()->update();
        qDebug() << "setImage";
    }

protected:
    void releaseResources() override;


public slots:
    void handleWindowChanged(QQuickWindow *win);
    void sync();
    void cleanup();
};

#endif // SQUIRCLE_H
