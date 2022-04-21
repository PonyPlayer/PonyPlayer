#include "hurricane.h"
#include "demuxer.h"

#include <QOpenGLShaderProgram>

#include <QDir>
#include <QtCore/QRunnable>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFramebufferObjectFormat>
#include <QTimer>

#define IGNORE_PAINT_DEBUG_LOG

const static GLfloat VERTEX_POS[] = {
        +1, +1, 0, 1, 0,
        +1, -1, 0, 1, 1,
        -1, +1, 0, 0, 0,
        -1, -1, 0, 0, 1,
};
const static GLuint VERTEX_INDEX[] = {
        0, 1, 2,
        1, 2, 3,
};

Hurricane::Hurricane(QQuickItem *parent) : QQuickItem(parent) {
    this->setFlag(QQuickItem::ItemHasContents);
    connect(this, &QQuickItem::windowChanged, this, &Hurricane::handleWindowChanged);
    qDebug() << "Create Hurricane QuickItem.";
    auto *timer = new QTimer(this);
    timer->setInterval(1000/30);
    auto *demuxer = new Demuxer;
    demuxer->openFile(QDir::homePath().append(u"/143468776-1-208.mp4"_qs).toStdString());
    demuxer->initDemuxer();
    connect(timer, &QTimer::timeout, [=]{
        auto * f = demuxer->videoFrameQueueFront();
        demuxer->videoFrameQueuePop();
        setImage(f);
//        demuxer->videoFrameQueuePop();
    });
    timer->start();
}


Hurricane::~Hurricane() noexcept {

}

void Hurricane::handleWindowChanged(QQuickWindow *win)  {
    qDebug() << "Window Size Changed:" << static_cast<void *>(win) << ".";
    if (win) {
//        connect(this->window(), &QQuickWindow::sceneGraphInitialized, this->renderer, &HurricaneRenderer::init, Qt::DirectConnection);
        connect(this->window(), &QQuickWindow::sceneGraphInvalidated, this, &Hurricane::cleanup, Qt::DirectConnection);
        connect(this->window(), &QQuickWindow::beforeSynchronizing, this, &Hurricane::sync, Qt::DirectConnection);
//        connect(this->window(), &QQuickWindow::widthChanged, this, &Hurricane::updateViewport);
//        connect(this->window(), &QQuickWindow::heightChanged, this, &Hurricane::updateViewport);
        win->setColor(Qt::black);
    }
}


void Hurricane::sync() {
    // call from renderer thread while GUI thread is blocking
    if (!renderer) {
        renderer = new HurricaneRenderer(this);
        connect(window(), &QQuickWindow::beforeRendering, renderer, &HurricaneRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, renderer, &HurricaneRenderer::paint, Qt::DirectConnection);
    }
    // sync state
    if (frame) {
        int w = frame->frame->width;
        int h = frame->frame->height;
        unsigned char * y  = frame->frame->data[0];
        unsigned char * u  = frame->frame->data[1];
        unsigned char * v  = frame->frame->data[2];
        renderer->setImageView({w, h}, y, u, v);
    }

}

void Hurricane::cleanup() {
    // called from renderer thread
    qDebug() << "SceneGraphInvalidated, delete renderer: " << static_cast<void *>(renderer) << ".";
    delete renderer;
    renderer = nullptr;
}


void Hurricane::releaseResources() {
    // call on GUI thread
    class CleanupJob : public QRunnable {
    private:
        HurricaneRenderer *renderer;
    public:
        explicit CleanupJob(HurricaneRenderer *renderer) : renderer(renderer) {}
        void run() override {
            qDebug() << "CleanupJob deletes renderer:" << static_cast<void*>(renderer) << ".";
            delete renderer;
        }
    };
    qDebug() << "ReleaseResources, schedule cleanup job.";
    window()->scheduleRenderJob(new CleanupJob(renderer), QQuickWindow::BeforeRenderingStage);
    Hurricane::renderer = nullptr;
}


HurricaneRenderer::HurricaneRenderer(QQuickItem *item) : quickItem(item) {
    qDebug() << "Create Hurricane Renderer:" << static_cast<void *>(this) << ".";
}

HurricaneRenderer::~HurricaneRenderer(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    program->release();
    qDebug() << "Deconstruct Hurricane Renderer:" << static_cast<void *>(this) << ".";
    delete program;
    program = nullptr;
}

void inline createTextureBuffer(GLuint *texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void HurricaneRenderer::init() {
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

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_POS), VERTEX_POS, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
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

    viewMatrix.setToIdentity();
    program->setUniformValue("view", viewMatrix);
}

void HurricaneRenderer::paint() {
    // prepare
    qreal ratio = quickItem->window()->devicePixelRatio();
    auto x = static_cast<GLint>(quickItem->x() * ratio);
    auto y = static_cast<GLint>(quickItem->y() * ratio);
    auto w = static_cast<GLsizei>(quickItem->width() * ratio);
    auto h = static_cast<GLsizei>(quickItem->height() * ratio);

    quickItem->window()->beginExternalCommands();
#ifndef IGNORE_PAINT_DEBUG_LOG
    qDebug() << "Paint" << "x =" << x << "y =" << y << "w =" << w << "h =" << h;
#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(x, y, w, h);
    glScissor(x, y, w, h);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    program->bind();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


    glActiveTexture(GL_TEXTURE0);
    if (flagUpdateImageSize) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureY);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imageSize.width(), imageSize.height(), 0, GL_RED, GL_UNSIGNED_BYTE, imageY);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureU);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imageSize.width() / 2, imageSize.height() / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageU);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureV);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imageSize.width() / 2, imageSize.height() / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageV);
    } else if (flagUpdateImageContent) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureY);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageSize.width(), imageSize.height(), GL_RED, GL_UNSIGNED_BYTE, imageY);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureU);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageSize.width() / 2, imageSize.height() / 2, GL_RED, GL_UNSIGNED_BYTE, imageU);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureV);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageSize.width() / 2, imageSize.height() / 2, GL_RED, GL_UNSIGNED_BYTE, imageV);
    }
    glDrawElements(GL_TRIANGLES, sizeof(VERTEX_INDEX) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glDisable(GL_SCISSOR_TEST);

    program->release();
    quickItem->window()->endExternalCommands();
}


void HurricaneRenderer::setImageView(QSize sz, GLubyte *y, GLubyte *u, GLubyte *v) {
    flagUpdateImageContent = true;
    if (sz != imageSize)
        flagUpdateImageSize = true;
    imageSize = sz;
    imageY = y;
    imageU = u;
    imageV = v;
//    quickItem->update();
}


