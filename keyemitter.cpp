#include <QCoreApplication>

#include "keyemitter.h"

KeyEmitter::KeyEmitter(QObject *parent) : QObject(parent) {
    initKeyMap();
}

void KeyEmitter::keyPressed(QVariant keyvar) {
    QString keystring = keyvar.toString();
    if (keymap.contains(keystring)) {
        keyobj key = keymap.value(keystring);
        qDebug() << "KE: " << key.key << " " << key.repr;
        QQuickItem *r = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

        if (key.modifier == Qt::ControlModifier) {
            QKeyEvent ctrlPressEvent = QKeyEvent(QEvent::Type::KeyPress, Qt::Key_Control, key.modifier);
            QCoreApplication::sendEvent(r, &ctrlPressEvent);
            QKeyEvent keyPressEvent = QKeyEvent(QEvent::Type::KeyPress, key.key, key.modifier, key.repr);
            QCoreApplication::sendEvent(r, &keyPressEvent);
            QKeyEvent keyReleaseEvent = QKeyEvent(QEvent::Type::KeyRelease, key.key, key.modifier, key.repr);
            QCoreApplication::sendEvent(r, &keyReleaseEvent);
            QKeyEvent ctrlReleaseEvent = QKeyEvent(QEvent::Type::KeyRelease, Qt::Key_Control, key.modifier);
            QCoreApplication::sendEvent(r, &ctrlReleaseEvent);
        } else {
            QKeyEvent keyPressEvent = QKeyEvent(QEvent::Type::KeyPress, key.key, key.modifier, key.repr);
            QCoreApplication::sendEvent(r, &keyPressEvent);
            QKeyEvent keyReleaseEvent = QKeyEvent(QEvent::Type::KeyRelease, key.key, key.modifier, key.repr);
            QCoreApplication::sendEvent(r, &keyReleaseEvent);
        }
    }
}

void KeyEmitter::initKeyMap() {
    keymap = QMap<QString, keyobj> {
        {" ", (keyobj){Qt::Key_Space, Qt::NoModifier, " "}},
        {"\r\n", (keyobj){Qt::Key_Return, Qt::NoModifier, "\r\n"}},
        {"\u001B", (keyobj){Qt::Key_Escape, Qt::NoModifier, ""}},
        {"\u007F", (keyobj){Qt::Key_Backspace, Qt::NoModifier, ""}},
        {"\x1B[A", (keyobj){Qt::Key_Up, Qt::NoModifier, ""}},
        {"\x1B[B", (keyobj){Qt::Key_Down, Qt::NoModifier, ""}},
        {"\x1B[C", (keyobj){Qt::Key_Right, Qt::NoModifier, ""}},
        {"\x1B[D", (keyobj){Qt::Key_Left, Qt::NoModifier, ""}},

        {"\x1B[1~", (keyobj){Qt::Key_Home, Qt::NoModifier, ""}},
        {"\u001B[1;2H", (keyobj){Qt::Key_Home, Qt::ShiftModifier, ""}},
        {"\x1B[4~", (keyobj){Qt::Key_End, Qt::NoModifier, ""}},
        {"\u001B[1;2F", (keyobj){Qt::Key_End, Qt::ShiftModifier, ""}},
        {"\x1B[5~", (keyobj){Qt::Key_PageUp, Qt::NoModifier, ""}},
        {"\x1B[6~", (keyobj){Qt::Key_PageDown, Qt::NoModifier, ""}},

        {"\u001B[1;5C", (keyobj){Qt::Key_Right, Qt::ControlModifier, ""}},
        {"\u001B[1;5D", (keyobj){Qt::Key_Left, Qt::ControlModifier, ""}},
        {"\u001B[1;2C", (keyobj){Qt::Key_Right, Qt::ShiftModifier, ""}},
        {"\u001B[1;2D", (keyobj){Qt::Key_Left, Qt::ShiftModifier, ""}},
        {"\u001B[1;6C", (keyobj){Qt::Key_Right, (Qt::ShiftModifier | Qt::ControlModifier), ""}},
        {"\u001B[1;6D", (keyobj){Qt::Key_Left, (Qt::ShiftModifier | Qt::ControlModifier), ""}},
    };
    for (int i = Qt::Key_Exclam; i <= Qt::Key_AsciiTilde; i++) {
        Qt::Key k = (Qt::Key) i;
        QString k_repr = QKeySequence(k).toString();
        if (97 <= i && i <= 122) { // Small letters
            k_repr = k_repr.toLower();
            k = (Qt::Key) (i - 32);
        }
        keymap.insert(k_repr, (keyobj) {k, Qt::NoModifier, k_repr});
    }
    // CTRL A-Z
    for (int i = 1; i <= 26; i++) {
        Qt::Key k = (Qt::Key) (i + 64);
        QByteArray d; d.append((char) i);
        QString k_repr = QString::fromLatin1(d);
        keymap.insert(k_repr, (keyobj) {k, Qt::ControlModifier, ""});
    }

    // Override Emacs keybindings
    keymap.insert("\u0001", (keyobj){Qt::Key_Home, Qt::NoModifier, ""});
    keymap.insert("\u0005", (keyobj){Qt::Key_End, Qt::NoModifier, ""});
    keymap.insert("\u0006", (keyobj){Qt::Key_Right, Qt::NoModifier, ""});
    keymap.insert("\u0002", (keyobj){Qt::Key_Left, Qt::NoModifier, ""});
    keymap.insert("\u001Bf", (keyobj){Qt::Key_Right, Qt::ControlModifier, ""});
    keymap.insert("\u001Bb", (keyobj){Qt::Key_Left, Qt::ControlModifier, ""});


    keymap.insert("\r", (keyobj){Qt::Key_Return, Qt::NoModifier, "\r\n"});
}

