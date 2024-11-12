// ToastMessage.h
#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPalette>

class ToastMessage : public QLabel {
    Q_OBJECT
public:
    explicit ToastMessage(const QString& message, QWidget* parent = nullptr) : QLabel(parent) {
        setText(message);
        
        setStyleSheet(
            "background-color: palette(midlight); "  
            "color: palette(toolTipText); "            
            "border-radius: 5px; "                    
            "padding: 5px;"
        );
        

        setAlignment(Qt::AlignCenter);

        QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(3000);  // Duration of the fade-out effect
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);

        connect(animation, &QPropertyAnimation::finished, this, &ToastMessage::deleteLater);
        animation->start();

        QTimer::singleShot(2500, this, [this, animation]() {
            animation->start();  // Start fade-out after 2.5 seconds
            });
    }
};