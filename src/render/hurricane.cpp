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
    qDebug() << "Load test.jpeg"  << "w:" << image.width() << "h:" << image.height() << "." << "Format:" << image.format();
    connect(timer, &QTimer::timeout, [=]{
        auto * frame = demuxer->videoFrameQueueFront();
        demuxer->videoFrameQueuePop();
        QImage img = QImage(frame->frame->data[0], frame->width, frame->height, frame->frame->linesize[0], QImage::Format::Format_RGB888);
        img.convertTo(QImage::Format::Format_RGB888);
        this->setImage(img);
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
    renderer->setImageView(image);
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

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &pbo);
    glBindTexture(GL_TEXTURE_2D, pbo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    viewMatrix.setToIdentity();
//    viewMatrix.
//    viewMatrix.ortho(0, 1, 0, 1, 0, 1);
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
    glBindTexture(GL_TEXTURE_2D, pbo);

    glActiveTexture(GL_TEXTURE0);
    if (flagUpdateImageSize) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageSize.width(), imageSize.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, imageView.bits());
    } else if (flagUpdateImageContent) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageSize.width(), imageSize.height(), GL_RGB, GL_UNSIGNED_BYTE, imageView.bits());
    }

    glDrawElements(GL_TRIANGLES, sizeof(VERTEX_INDEX) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glDisable(GL_SCISSOR_TEST);

    program->release();
    quickItem->window()->endExternalCommands();
}

const QImage &HurricaneRenderer::getImageView() const {
    return imageView;
}

void HurricaneRenderer::setImageView(const QImage &img) {
    flagUpdateImageContent = true;
    if (img.size() != imageSize)
        flagUpdateImageSize = true;
    HurricaneRenderer::imageSize = img.size();
    HurricaneRenderer::imageView = img;
//    quickItem->update();
}


