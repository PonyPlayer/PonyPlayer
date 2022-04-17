#include "hurricane.h"

#include <QOpenGLShaderProgram>


#include <QtCore/QRunnable>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLDebugLogger>

const static GLfloat VERTEX_POS[] = {
        1, 1, 0,
        1, 0, 0,
        0, 1, 0,
        0, 0, 0,
};
const static GLuint VERTEX_INDEX[] = {
        0, 1, 2,
        1, 2, 3,
};

Hurricane::Hurricane(QQuickItem *parent)
        : QQuickFramebufferObject(parent) {
//    connect(this, &QQuickItem::windowChanged, this, &Hurricane::handleWindowChanged);
    qDebug() << "Create Hurricane QuickItem.";
}


Hurricane::~Hurricane() noexcept {

}

QQuickFramebufferObject::Renderer *Hurricane::createRenderer() const {
    return new HurricaneRenderer();
}



HurricaneRenderer::HurricaneRenderer() {
    qDebug() << "Create Hurricane Renderer.";
    image.load(":/render/test.jpeg");
    image.convertTo(QImage::Format::Format_RGB888);
    qDebug() << "Load test.jpeg" << "Format:" << image.format() << ".";
    init();
}

HurricaneRenderer::~HurricaneRenderer(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    program.release();
}

void HurricaneRenderer::init() {
    QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();
    qDebug() << "OpenGL version:"
             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    qDebug() << "GSLS version:"
             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    qDebug() << "Vendor:" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    qDebug() << "Renderer:" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, u":/render/shader/vertex.vsh"_qs);
    program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, u":/render/shader/fragment.fsh"_qs);
    program.link();
    program.bind();

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_POS), VERTEX_POS, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),0);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    viewMatrix.setToIdentity();
    viewMatrix.ortho(0, 1, 0, 1, 0, 1);
    program.setUniformValue("view", viewMatrix);
}

void HurricaneRenderer::paint() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindTexture(GL_TEXTURE_2D, pbo);
    glDrawElements(GL_TRIANGLES, sizeof(VERTEX_INDEX) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

QOpenGLFramebufferObject *HurricaneRenderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

void HurricaneRenderer::render() {
    this->paint();
}

