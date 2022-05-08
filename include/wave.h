#pragma once

#include <QtCore>
#include <QtQuick/QQuickItem>

class WaveView : public QQuickItem {
Q_OBJECT
    Q_PROPERTY(float radiusRatio READ getRadiusRatio WRITE setRadiusRatio NOTIFY notifyRadiusRatio)
    QML_ELEMENT
private:
    float m_radiusRatio{};
public:
    explicit WaveView(QQuickItem *parent = nullptr);

    Q_INVOKABLE [[nodiscard]] float getRadiusRatio() const { return m_radiusRatio; };
public slots:
    Q_INVOKABLE void setRadiusRatio(float radiusRatio) {
        m_radiusRatio = radiusRatio; emit notifyRadiusRatio();
    };
signals:
    void notifyRadiusRatio();
};