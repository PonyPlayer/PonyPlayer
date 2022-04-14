//
// Created by ColorsWind on 2022/4/13.
//

#include <video.h>
#include <QQuickWindow>
#include <QOffscreenSurface>


OpenGLItem::OpenGLItem() : m_render(nullptr) {
    m_timer.start(12, this);
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow *window) {
        if (window) {
            connect(window, &QQuickWindow::beforeSynchronizing, this, &OpenGLItem::sync,
                    Qt::DirectConnection);
            connect(window, &QQuickWindow::sceneGraphInvalidated, this, &OpenGLItem::cleanup,
                    Qt::DirectConnection);

//            window->setClearBeforeRendering(false);
        } else return;
    });
}

OpenGLItem::~OpenGLItem() {

}

void OpenGLItem::sync() {
    if (!m_render) {

        m_render = new PonyPlayerRender();
        auto m_context = new QOpenGLContext();
        m_context->create();
        m_context->makeCurrent(window());
        m_render->initializeGL();
        m_render->resizeGL(window()->width(), window()->height());
        connect(window(), &QQuickWindow::beforeRendering, this, [this]() {
            //window()->resetOpenGLState();
            m_render->render();
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, [this]() {
            //渲染后调用，计算fps
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::widthChanged, this, [this]() {
            m_render->resizeGL(window()->width(), window()->height());
        });
        connect(window(), &QQuickWindow::heightChanged, this, [this]() {
            m_render->resizeGL(window()->width(), window()->height());
        });
    }
}

void OpenGLItem::cleanup() {
    if (m_render) {
        delete m_render;
        m_render = nullptr;
    }
}

void OpenGLItem::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    window()->update();
}

void PonyPlayerRender::render() {
    glClearColor(0.2F, 0.3F, 0.3F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void BaseRender::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void BaseRender::initializeGL() {
//    QSurfaceFormat format;
//    m_context->makeCurrent(m_surface);
    initializeOpenGLFunctions();
    initializeShader();
}

void BaseRender::initializeShader() {}

BaseRender::BaseRender() {
//    m_context = new QOpenGLContext;
//    m_context->create();
}

BaseRender::~BaseRender() {

}
