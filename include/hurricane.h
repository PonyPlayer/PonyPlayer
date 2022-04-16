#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

const GLfloat VERTEX_POS[] = {
        +1, +1, -1,
        +1, -1, -1,
        -1, +1, -1,
        -1, -1, -1,
};
const GLuint VERTEX_INDEX[] = {
        0, 1, 2,
        1, 2, 3,
};

class HurricaneRenderer : public QObject, protected QOpenGLFunctions {
Q_OBJECT
private:
    GLuint vao;
    QOpenGLBuffer* vbo;
    QOpenGLBuffer m_vbo;
    GLuint ebo;
    GLuint texture;
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
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
    QImage image;
    QColor clearColor = Qt::darkCyan;
    QPoint lastPos;
    int xRot = 0;
    int yRot = 0;
    int zRot = 0;
    QOpenGLTexture *img_texture = nullptr;
    QOpenGLShaderProgram *program = nullptr;

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
