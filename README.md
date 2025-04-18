# Wi-Fi-enabled-Smart-RFID-Theft-Detection-System


This project is an **RFID-based Theft Detection System** with **motion detection** and **cloud reporting** via **MQTT**. It integrates **Wi-Fi**, **Adafruit IO** for cloud connectivity, and utilizes **LCD displays**, **LEDs**, and **buzzer alerts** to enhance security and monitoring.

## 📌 Features
- **RFID Authentication**: Allows authorized RFID tags to grant access, while unauthorized tags trigger alerts.
- **Motion Detection**: Uses an IR sensor to detect motion and trigger RFID scanning.
- **Real-Time Cloud Reporting**: Sends unauthorized access alerts and system status to **Adafruit IO** via **MQTT**.
- **Local Feedback**: Displays real-time status on an **LCD screen** and triggers visual/audio alerts via **LED** and **buzzer** for unauthorized access.
- **Unauthorized Access Alert**: Sends immediate notifications upon unauthorized access attempts.

## 🛠️ Tech Stack
- **Microcontroller**: ESP8266
- **RFID**: MFRC522 Module
- **Cloud**: Adafruit IO (MQTT protocol)
- **LCD Display**: 16x2 I2C LCD
- **Sensors**: PIR Motion Sensor
- **Networking**: MQTT, Wi-Fi (via ESP8266)
- **Programming Language**: C/C++
- **Libraries Used**:
  - `MFRC522`: RFID library for the MFRC522 module
  - `Adafruit_MQTT`: Adafruit MQTT client
  - `Wire`, `LiquidCrystal_I2C`: For LCD display functionality
  - `ESP8266WiFi`: For Wi-Fi functionality


## 🔌 Hardware Setup
- **ESP8266** microcontroller
- **MFRC522** RFID reader module
- **PIR** motion sensor
- **LED & Buzzer** for alerts
- **LCD Display (16x2)** for system feedback

### Pin Connections:
| Component               | ESP8266 Pin  |
|-------------------------|--------------|
| **IR Sensor**            | D4           |
| **Green LED**            | D3           |
| **Alert Buzzer**         | D0           |
| **RFID SS Pin**          | D8           |
| **RFID RST Pin**         | D3           |
| **LCD SDA**              | D2           |
| **LCD SCL**              | D1           |

## 🚀 Setup & Installation
### Prerequisites
- **Arduino IDE** with ESP8266 board installed
- **Adafruit IO** account (for MQTT)
- **Wi-Fi credentials** and **Adafruit IO key**

### Steps to Set Up
**1. Clone this Repository**:
   ```bash
   git clone https://github.com/Sumit-22/rfid-theft-detection-system.git
   ```

**2. Install Necessary Libraries**:

  - `MFRC522`: RFID library for the MFRC522 module
  - `Adafruit_MQTT`: Adafruit MQTT client
  - `Wire`, `LiquidCrystal_I2C`: For LCD display functionality
  - `ESP8266WiFi`: For Wi-Fi functionality

**3. Configure Wi-Fi and Adafruit IO**:
 - Replace <WIFI_SSID>, <WIFI_PASSWORD>, <USERNAME>, and <KEY> in the code with your actual credentials and Adafruit IO key.

**4. Upload the Code to the ESP8266 via Arduino IDE.**

**5. Monitor Serial Output** :
 - Open the Serial Monitor to check real-time feedback and status messages.

**6. Run the System** :
 - Once powered up, the system will connect to Wi-Fi, initialize the RFID reader, and start detecting motion. If an unauthorized RFID tag is scanned, an alert will be sent to Adafruit IO, and the system will display the status on the LCD.