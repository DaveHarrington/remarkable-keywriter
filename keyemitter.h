#ifndef KEYEMITTER_H
#define KEYEMITTER_H
#include <QObject>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QtQuick>
class KeyEmitter : public QObject
{
    Q_OBJECT
public:
    KeyEmitter(QObject* parent=nullptr) : QObject(parent) {}
public slots:
    void keyPressed(QVariant _) {
        Qt::Key k = Qt::Key_K;
        qDebug() << "In keyemitter: " << k;
        QQuickItem *r = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());
        QKeyEvent keyPressEvent = QKeyEvent(QEvent::Type::KeyPress, (Qt::Key)k, Qt::NoModifier, QKeySequence(k).toString());
        QCoreApplication::sendEvent(r, &keyPressEvent);
        QKeyEvent keyReleaseEvent = QKeyEvent(QEvent::Type::KeyRelease, (Qt::Key)k, Qt::NoModifier, QKeySequence(k).toString());
        QCoreApplication::sendEvent(r, &keyReleaseEvent);
    }
};
#endif // KEYEMITTER_H
