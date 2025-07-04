// buttoncontroller.h
#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include <QObject>
#include <QTimer> // For polling the button state (conceptual)

// Forward declaration of the global flag from the C embedded code
extern volatile uint8_t buttonPressedFlag;

class ButtonController : public QObject
{
    Q_OBJECT
    // Q_PROPERTY to expose the button state to QML
    Q_PROPERTY(bool isButtonPressed READ isButtonPressed NOTIFY isButtonPressedChanged)

public:
    explicit ButtonController(QObject *parent = nullptr);
    bool isButtonPressed() const { return m_isButtonPressed; }

signals:
    // Signal emitted when the button state changes
    void isButtonPressedChanged();

private slots:
    // Slot to periodically check the button state from the embedded system
    void checkButtonState();

private:
    bool m_isButtonPressed;
    QTimer m_pollTimer; // Timer for polling (in a real MCU setup, this might be event-driven)
};

#endif // BUTTONCONTROLLER_H
