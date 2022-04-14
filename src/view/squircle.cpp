#include "squircle.h"

#include <QtQuick/qquickwindow.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QtCore/QRunnable>


Squircle::Squircle()
        : m_t(0), m_renderer(nullptr) {
    connect(this, &QQuickItem::windowChanged, this, &Squircle::handleWindowChanged);
}


void Squircle::setT(qreal t) {
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}


void Squircle::handleWindowChanged(QQuickWindow *win) {
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &Squircle::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &Squircle::cleanup, Qt::DirectConnection);
        win->setColor(Qt::black);
    }
}


void Squircle::cleanup() {
    delete m_renderer;
    m_renderer = nullptr;
}

class CleanupJob : public QRunnable {
public:
    CleanupJob(SquircleRenderer *renderer) : m_renderer(renderer) {}

    void run() override { delete m_renderer; }

private:
    SquircleRenderer *m_renderer;
};

void Squircle::releaseResources() {
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

SquircleRenderer::~SquircleRenderer() {
    delete m_program;
}



void Squircle::sync() {
    if (!m_renderer) {
        m_renderer = new SquircleRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &SquircleRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &SquircleRenderer::paint,
                Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setWindow(window());
}

void Squircle::loadImage(const QUrl &url) {
    image.load(url.path());
}


void SquircleRenderer::init() {
}

void SquircleRenderer::paint() {
    // Play nice with the RHI. Not strictly needed when the scenegraph uses
    // OpenGL directly.
    m_window->beginExternalCommands();

    m_program->bind();


//    m_program->enableAttributeArray(0);
//
//    float values[] = {
//            -1, -1,
//            1, -1,
//            -1, 1,
//            1, 1
//    };
//
//    // This example relies on (deprecated) client-side pointers for the vertex
//    // input. Therefore, we have to make sure no vertex buffer is bound.
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
//    m_program->setUniformValue("t", static_cast<float>( m_t));
//
//    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
//
//    glDisable(GL_DEPTH_TEST);
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//    m_program->disableAttributeArray(0);
//    m_program->release();

    m_window->endExternalCommands();
}

SquircleRenderer::SquircleRenderer() : m_t(0), m_window(nullptr) {
    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram();
//    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
//                                                "attribute highp vec4 vertices;"
//                                                "varying highp vec2 coords;"
//                                                "void main() {"
//                                                "    gl_Position = vertices;"
//                                                "    coords = vertices.xy;"
//                                                "}");
//    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
//                                                "uniform lowp float t;"
//                                                "varying highp vec2 coords;"
//                                                "void main() {"
//                                                "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
//                                                "    i = smoothstep(t - 0.8, t + 0.8, i);"
//                                                "    i = floor(i * 20.) / 20.;"
//                                                "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
//                                                "}");

//    m_program->bindAttributeLocation("vertices", 0);
    m_program->link();
}

