#pragma once
#include <QWidget>


// Overlay is a semi-transparent black widget that can be used with popup
class Overlay : public QWidget {
public:
    Overlay(QWidget* parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents); // Let mouse events pass through
        setStyleSheet("background-color: rgba(0, 0, 0, 128);"); // Semi-transparent black
    }

    void resizeToParent() {
        if (parentWidget()) {
            resize(parentWidget()->size());
            move(parentWidget()->pos());
        }
    }
};