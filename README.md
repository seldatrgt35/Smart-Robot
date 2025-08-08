# Smart-Robot

This project is an **Arduino-based smart robot** that senses its environment and avoids obstacles using multiple sensors. It features **DHT11**, **MQ135**, **ultrasonic sensors**, and a **16x2 I2C LCD** display to monitor and respond to environmental conditions.

## Features

- **Obstacle Avoidance:** Uses 3 ultrasonic sensors (front, left, right) to detect obstacles
- **Temperature & Humidity Monitoring:** Via DHT11 sensor
- **Air Quality Detection:** With MQ135 gas sensor
- **Alert System:** Buzzer activates when temperature or air pollution exceeds safe levels
- **LCD Display:** Real-time display of temperature, humidity, air quality, and sensor readings
- **Directional Movement:** Robot turns left/right or moves forward based on obstacle positions


## Setup Instructions

1. Open the code in Arduino IDE.
2. Ensure the following libraries are installed:
    - `Wire.h`
    - `LiquidCrystal_I2C.h`
    - `DHT.h`
3. Upload the code to the Arduino Uno board.
4. Connect the components according to the wiring diagram.
5. Power up the system and observe the LCD display and robot behavior.
