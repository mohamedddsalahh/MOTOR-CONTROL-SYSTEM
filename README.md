# MOTOR-CONTROL-SYSTEM
Vehicle Motor Fault Control System that handles fault conditions (High Temperature - Communication Loss - Error in Temperature Readings)

The project composed of 2 ECUs (Tiva-C), one is responsible for monitoring and controlling while the other is responsible for measuring motor temperature.
The application is implemented in Real Time Operating System and uses various drivers such as: GPIO, PORT, GPTM, SPI, UART, FLASH and CAN Communication Protocol.
Both GPIO and PORT Drivers are based on AUTOSAR Standards.
FLASH Driver is used to save the DTC in the Flash Memory.
The Operating System used is implemented from scratch not the standard RTOS Library.
