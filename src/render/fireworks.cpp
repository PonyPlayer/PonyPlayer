#include "hurricane.h"

#include <QOpenGLShaderProgram>
#include <QDir>
#include <QtCore/QRunnable>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFramebufferObjectFormat>
#include <QSGRenderNode>

//#define DEBUG_FlAG_PAINT_LOG
const void* ZERO_OFFSET = nullptr;

const static GLfloat VERTEX_POS[] = {
        1, 1, 0, 1, 0,
        1, 0, 0, 1, 1,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 1,
};
const static GLuint VERTEX_INDEX[] = {
        0, 1, 2,
        1, 2, 3,
};

Fireworks::Fireworks(QQuickItem *parent) : QQuickItem(parent) {
    this->setFlag(QQuickItem::ItemHasContents);
    connect(this, &QQuickItem::windowChanged, this, &Fireworks::handleWindowChanged);
    qDebug() << "Create Hurricane QuickItem.";
}


Fireworks::~Fireworks() noexcept {
    picture.free();
    for(auto && pic : cleanupPictureQueue) {
        pic.free();
    }
}

void Fireworks::handleWindowChanged(QQuickWindow *win)  {
    qDebug() << "Window Size Changed:" << static_cast<void *>(win) << ".";
    if (win) {
//        connect(this->window(), &QQuickWindow::sceneGraphInitialized, this->renderer, &HurricaneRenderer::init, Qt::DirectConnection);
        connect(this->window(), &QQuickWindow::beforeSynchronizing, this, &Fireworks::sync, Qt::DirectConnection);
//        connect(this->window(), &QQuickWindow::widthChanged, this, &Hurricane::updateViewport);
//        connect(this->window(), &QQuickWindow::heightChanged, this, &Hurricane::updateViewport);
        win->setColor(Qt::black);
    } else {
        qWarning() << "Window destroy.";
    }
}


void Fireworks::sync() {
    // call from renderer thread while GUI thread is blocking
    if (!renderer) {
        renderer = new HurricaneRenderer(this);
        connect(window(), &QQuickWindow::beforeRendering, renderer, &HurricaneRenderer::init, Qt::DirectConnection);
        //
        //        connect(window(), &QQuickWindow::afterRenderPassRecording, renderer, &HurricaneRenderer::paint, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, &Fireworks::cleanupPicture);
    }
    // sync state
    if (picture.isValid()) {
        renderer->setImageView(picture);
    }
    // sync uniform
    renderer->brightness = this->brightness;
    renderer->contrast = this->contrast;
    renderer->saturation = this->saturation;

}


void Fireworks::cleanupPicture() {
    // call on GUI thread
    for(auto && pic : cleanupPictureQueue) {
        pic.free();
    }
    cleanupPictureQueue.clear();
}

QSGNode *Fireworks::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) {
    return renderer;
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
    brightness.init(program, "brightness");
    contrast.init(program, "contrast");
    saturation.init(program, "saturation");

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_POS), VERTEX_POS, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), ZERO_OFFSET);
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
}

void HurricaneRenderer::render(const RenderState *state) {

    // call on render thread
    if (!imageY || !imageU || !imageV) { return; }
    // since QuickItem position is relative position of its parent, we need to convert to scene coordinate
    QPointF qtPos = quickItem->mapToScene(quickItem->position());
    // scale to physical pixel
    qreal ratio =  quickItem->window()->devicePixelRatio();
    QRect glRect = {
            static_cast<int>(qtPos.x() * ratio),
            // in qt cartesian coordinate system, (0, 0) is at the left top corner
            // while in OpenGL coordinate system, (0, 0) is at the left bottom corner
            static_cast<int>((quickItem->window()->height() - quickItem->height() - qtPos.y()) * ratio),
            static_cast<int>(quickItem->width() * ratio),
            static_cast<int>(quickItem->height() * ratio),
    };
//    quickItem->window()->beginExternalCommands();
#ifdef DEBUG_FlAG_PAINT_LOG
    qDebug() << "Paint" << "x =" << glRect.x() << "y =" << glRect.y() << "w =" << glRect.width() << "h =" << glRect.height();
#endif
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(glRect.x(), glRect.y(), glRect.width(), glRect.height());

//    glScissor(glRect.x(), glRect.y(), glRect.width(), glRect.height());
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    program->bind();
    brightness.render();
    contrast.render();
    saturation.render();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    if (flagUpdateImageSize) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureY);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, imageY);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureU);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize / 2, imageHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageU);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureV);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, lineSize / 2, imageHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, imageV);
        // since FFmpeg may pad frame to align, we need to clip invalid data
        viewMatrix.setToIdentity();
        viewMatrix.ortho(0, static_cast<float>(imageWidth) / static_cast<float>(lineSize), 0, 1, -1, 1);
        program->setUniformValue("view", viewMatrix);
    } else if (flagUpdateImageContent) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureY);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize, imageHeight, GL_RED, GL_UNSIGNED_BYTE, imageY);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureU);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize / 2, imageHeight / 2, GL_RED, GL_UNSIGNED_BYTE, imageU);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureV);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lineSize / 2, imageHeight / 2, GL_RED, GL_UNSIGNED_BYTE, imageV);
    }
    glDrawElements(GL_TRIANGLES, sizeof(VERTEX_INDEX) / sizeof(GLuint), GL_UNSIGNED_INT, ZERO_OFFSET);
    glDisable(GL_SCISSOR_TEST);
    program->release();
//    quickItem->window()->endExternalCommands();
}

void HurricaneRenderer::setImageView(const Picture &pic) {
    // should call on sync stage
    flagUpdateImageContent = true;
    imageY = pic.getY();
    imageU = pic.getU();
    imageV = pic.getV();
    if (pic.getWidth() != imageWidth || pic.getHeight() != imageHeight || pic.getLineSize() != lineSize) {
        imageWidth = pic.getWidth();
        imageHeight = pic.getHeight();
        lineSize = pic.getLineSize();
        flagUpdateImageSize = true;
    }
}



