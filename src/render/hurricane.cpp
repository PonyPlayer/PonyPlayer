#include "hurricane.h"

#include <QtQuick/qquickwindow.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <QtCore/QRunnable>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

Hurricane::Hurricane()
        : m_t(0), m_renderer(nullptr) {
    connect(this, &QQuickItem::windowChanged, this, &Hurricane::handleWindowChanged);
}


void Hurricane::setT(qreal t) {
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}


void Hurricane::handleWindowChanged(QQuickWindow *win) const {
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &Hurricane::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &Hurricane::cleanup, Qt::DirectConnection);
        win->setColor(Qt::black);
    }
}


void Hurricane::cleanup() {
    delete m_renderer;
    m_renderer = nullptr;
}

class CleanupJob : public QRunnable {
public:
    explicit CleanupJob(HurricaneRenderer *renderer) : m_renderer(renderer) {}

    void run() override { delete m_renderer; }

private:
    HurricaneRenderer *m_renderer;
};

void Hurricane::releaseResources() {
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

HurricaneRenderer::~HurricaneRenderer(){
    delete m_program;
}


void Hurricane::sync() {
    if (!m_renderer) {
        m_renderer = new HurricaneRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &HurricaneRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &HurricaneRenderer::paint,
                Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setWindow(window());
}

void Hurricane::loadImage(const QUrl &url) {
    image.load(url.path());
}





HurricaneRenderer::HurricaneRenderer() : m_t(0), m_program(nullptr), m_window(nullptr) {
    initializeOpenGLFunctions();
    qDebug() << "GL version: "
             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    qDebug() << "GSLS version: "
             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    program = new QOpenGLShaderProgram;
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, u":/render/shader/vertex.vsh"_qs);
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, u":/render/shader/fragment.fsh"_qs);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("texture", 0);
}

void HurricaneRenderer::init() {
    program->bind();

    float arrVertex[] = {
            //   position                 color
            0.0f, 0.707f, -3.0f,     1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -3.0f,     0.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  -3.0f,     0.0f, 0.0f, 1.0f,
    };

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(arrVertex, sizeof(arrVertex));

    int attr = program->attributeLocation("aCol");
    program->setAttributeBuffer(0, GL_FLOAT, 0,
                                  3, sizeof(float) * 6);
    program->enableAttributeArray(0);

    program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float),
                                  3, sizeof(float) * 6);
    program->enableAttributeArray(1);

    m_vbo.release();

//    vbo = new QOpenGLBuffer();
//    vbo->create();
//    vbo->allocate(VERTEX_POS, sizeof(VERTEX_POS));
//    vbo->bind();
//    vbo->write(0, VERTEX_POS, sizeof(VERTEX_POS));
////    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
////    program->setAttributeBuffer(1, GL_FLOAT, 0, 2, 3 * sizeof(GLfloat));
////    program->enableAttributeArray(0);
////    program->enableAttributeArray(1);
//    this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
//    this->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
//    this->glEnableVertexAttribArray(0);
//    this->glEnableVertexAttribArray(1);
//
////    this->glGenBuffers(1, &vao);
////    this->glBindBuffer(GL_ARRAY_BUFFER, vao);
////    this->glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_POS), VERTEX_POS, GL_STATIC_DRAW);
////
////    this->glGenBuffers(1, &ebo);
////    this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
////    this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VERTEX_INDEX), VERTEX_INDEX, GL_STATIC_DRAW);
//
//
////    this->glGenTextures(1, &texture);
////    this->glActiveTexture(GL_TEXTURE0);
////    this->glBindTexture(GL_TEXTURE_2D, texture);
//
//    this->glEnable(GL_BLEND);
//    this->glEnable(GL_DEPTH_TEST);
//
//    QMatrix4x4 view;
//    view.setToIdentity();
//    view.ortho(-1, 1, -1, 1, -10, 10);
//    this->program->setUniformValue("view", view);
//
//    this->image.load(":/render/test.jpeg");
}

void HurricaneRenderer::paint() {
    m_window->beginExternalCommands();
//    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
////    this->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
////                 image.width(), image.height(), 0,
////                 GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
////    this->glDrawElements(GL_TRIANGLES, sizeof(VERTEX_INDEX) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
//    this->vbo->bind();
//    this->glDrawArrays(GL_TRIANGLES, 0, 3);
//    qDebug() << "paint";
//    m_window->endExternalCommands();

    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT);

    program->bind();

    m_vbo.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_vbo.release();

    program->release();
    m_window->endExternalCommands();
}

