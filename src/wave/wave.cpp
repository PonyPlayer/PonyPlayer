#include "wave.h"
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>

WaveView::WaveView(QQuickItem *parent) : QQuickItem(parent), m_p1(0, 0), m_p2(1, 0), m_p3(0, 1), m_p4(1, 1),
                                         m_segmentCount(32) {
    setFlag(ItemHasContents, true);
}

QSGNode *WaveView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *) {
    auto *node = dynamic_cast<QSGGeometryNode *>(oldNode);
    if (!node) node = new QSGGeometryNode();
    if (!node->geometry()) {
        auto *m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_segmentCount);
        m_geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        node->setGeometry(m_geometry);
        node->setFlag(QSGNode::OwnsGeometry);
    }
    if (!node->material()) {
        auto *m_material = new QSGFlatColorMaterial;
        m_material->setColor(Qt::red);
        node->setMaterial(m_material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    QSGGeometry *m_geometry = node->geometry();
    QSGGeometry::Point2D *v = m_geometry->vertexDataAsPoint2D();
    const QRectF rect = boundingRect();
    QPointF center = rect.center();
    double radius = qMin(rect.width(),rect.height()) / 2;
    int numPoints = m_geometry->vertexCount();
    for (int i = 0; i < numPoints; ++i) {
        float theta = i * 2 * M_PI / (numPoints - 2);
        v[i].x = center.x() + radius * cos(theta);
        v[i].y = center.y() - radius * sin(theta);
    }
    node->markDirty(QSGNode::DirtyGeometry);
    qDebug() << "DRAWING";
    return node;
}
