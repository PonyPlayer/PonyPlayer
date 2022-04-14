#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class SquircleRenderer : public QObject, protected QOpenGLFunctions {
Q_OBJECT
public:
    SquircleRenderer();

    ~SquircleRenderer() override;

    void setT(qreal t) { m_t = t; }

    void setViewportSize(const QSize &size) { m_viewportSize = size; }

    void setWindow(QQuickWindow *window) { m_window = window; }

    void loadImage(QImage &image);

public slots:

    void init();

    void paint();

private:
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
    QImage image;
};


class Squircle : public QQuickItem {
Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    QML_ELEMENT

public:
    Squircle();

    [[nodiscard]] qreal t() const { return m_t; }

    void setT(qreal t);

signals:

    void tChanged();

public slots:

    void sync();

    void cleanup();

    void loadImage(const QUrl& url);

private slots:

    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    qreal m_t;
    SquircleRenderer *m_renderer;
    QImage image;
};

#endif // SQUIRCLE_H
