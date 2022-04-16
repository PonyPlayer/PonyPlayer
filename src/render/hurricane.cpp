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


void HurricaneRenderer::init() {
    initializeOpenGLFunctions();

    vbo.bind();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    program->bind();
}

void HurricaneRenderer::paint() {
    m_window->beginExternalCommands();
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    m.rotate(0.0f, 1.0f, 0.0f, 0.0f);
    m.rotate(0.0f, 0.0f, 1.0f, 0.0f);
    m.rotate(0.0f, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    img_texture->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    m_window->endExternalCommands();
}

HurricaneRenderer::HurricaneRenderer() : m_t(0), m_program(nullptr), m_window(nullptr) {
    initializeOpenGLFunctions();
    makeObject();
    program = new QOpenGLShaderProgram;
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, u":/render/shader/vertex.vsh"_qs);
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, u":/render/shader/fragment.fsh"_qs);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("texture", 0);
}

void HurricaneRenderer::makeObject() {
    static const int coords[4][3] = {
            {+1, -1, -1},
            {-1, -1, -1},
            {-1, +1, -1},
            {+1, +1, -1}
    };
    QImage img = QImage(":/render/test.jpeg").mirrored();

    img_texture = new QOpenGLTexture(img);

    double ratio = 1.0 * img.size().height() / img.size().width();
    qDebug() << "ratio is " << ratio;

    QList<GLfloat> vertData;
    for (int j = 0; j < 4; ++j) {
        // vertex position
        vertData.append(static_cast<float>(0.3 * coords[j][0]));
        vertData.append(static_cast<float>(0.3 * coords[j][1]));
        vertData.append(static_cast<float>(1 * coords[j][2]));
        // texture coordinate
        vertData.append(j == 0 || j == 3);
        vertData.append(j == 0 || j == 1);
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(),
                 static_cast<int>((static_cast<unsigned long long>(vertData.count()) * sizeof(GLfloat))));
}
