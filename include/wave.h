#include <QtCore>
#include <QtQuick/QQuickItem>

class WaveView : public QQuickItem {
Q_OBJECT
private:
    QPointF m_p1;
    QPointF m_p2;
    QPointF m_p3;
    QPointF m_p4;
    int m_segmentCount;
public:
    explicit WaveView(QQuickItem *parent = 0);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
};