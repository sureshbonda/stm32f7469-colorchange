// buttoncontroller.cpp
#include "buttoncontroller.h"
// In a real Qt for MCUs project, you'd have a way to link to the embedded C code
// and access `buttonPressedFlag`. This might involve shared memory, a custom
// communication driver, or direct linking if compiled as one application.

// Initialize the global flag (assuming it's defined in the C code)
// volatile uint8_t buttonPressedFlag = 0; // This should be defined in the C embedded code

ButtonController::ButtonController(QObject *parent)
    : QObject(parent),
      m_isButtonPressed(false)
{
    // Set up a timer to periodically check the button state.
    // In a real Qt for MCUs application, you might have a more direct
    // event-driven mechanism (e.g., a custom event posted from an IO thread)
    // instead of polling.
    m_pollTimer.setInterval(50); // Check every 50ms
    connect(&m_pollTimer, &QTimer::timeout, this, &ButtonController::checkButtonState);
    m_pollTimer.start();
}

void ButtonController::checkButtonState()
{
    // Read the current state from the embedded system's flag
    bool currentButtonState = (buttonPressedFlag == 1);

    if (m_isButtonPressed != currentButtonState)
    {
        m_isButtonPressed = currentButtonState;
        emit isButtonPressedChanged(); // Notify QML about the change
    }

    // If the button was just pressed, clear the flag in the embedded side
    // This assumes the Qt application is responsible for clearing it after reading.
    // In a more robust system, the embedded side would manage its own state.
    if (buttonPressedFlag == 1) {
        buttonPressedFlag = 0; // Clear the flag after processing
    }
}
