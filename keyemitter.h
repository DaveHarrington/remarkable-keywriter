#ifndef KEYEMITTER_H
#define KEYEMITTER_H
#include <QObject>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QtQuick>

struct keyobj {
  Qt::Key key;
  QFlags<Qt::KeyboardModifier> modifier;
  QString repr;
} ;

class KeyEmitter : public QObject
{
  Q_OBJECT
  public:
    explicit KeyEmitter(QObject* parent = 0);
    public slots:
      void keyPressed(QVariant keystring);
  private:
    void initKeyMap();

    QMap<QString, keyobj> keymap;
};
#endif // KEYEMITTER_H
