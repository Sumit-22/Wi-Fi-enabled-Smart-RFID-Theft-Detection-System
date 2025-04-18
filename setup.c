#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ---------------- CONFIG ------------------
#define WIFI_SSID "<WIFI_SSID>"
#define WIFI_PASSWORD "<WIFI_PASSWORD>"
#define AIO_USERNAME "<USERNAME>"
#define AIO_KEY "<KEY>"
#define AIO_FEED "unauthorised"
#define AIO_FEED2 "authorised"
#define STAT_FEED "stat_feed"

// ---------- Pins Setup ----------
#define IR_SENSOR_PIN D4
#define GREEN_LED_PIN D3
#define ALERT_PIN D0

#define RST_PIN 3
#define SS_PIN D8

// -------------- Global Objects ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, AIO_USERNAME, AIO_KEY);

// to send alerts for unauthorised access
Adafruit_MQTT_Publish alertFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_FEED);
// to send access granted notices
Adafruit_MQTT_Publish accessFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_FEED2);

// to send control status indicator
Adafruit_MQTT_Publish statusFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" STAT_FEED);

// Authorized RFID tags
String authorizedTags[] = {"83129B29", "33CE1B29", "F3D57D14"};

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // Pin modes
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(ALERT_PIN, OUTPUT);

    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(ALERT_PIN, LOW);

    // LCD init
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting WiFi");

    // WiFi connect
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("CW");
    }
    lcd.clear();
    lcd.print("WiFi Connected");
    Serial.println("Wifi Connected");

    // SPI + RFID init
    SPI.begin();
    mfrc522.PCD_Init();

    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("System Ready");
    delay(10000);
}

void loop()
{
    MQTT_connect();
    delay(1000);

    if (digitalRead(IR_SENSOR_PIN) == LOW) // when motion detected
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Motion Detected");
        Serial.println("Motion Detected");

        delay(500);
        lcd.setCursor(0, 1);
        lcd.print("Scan your card");

        unsigned long startTime = millis();
        bool scanned = false;

        while (millis() - startTime < 8000)
        { // 8 seconds to scan
            if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
            {
                delay(100);
                continue;
            }

            // Parsing the tag
            String tag = "";
            for (byte i = 0; i < mfrc522.uid.size; i++)
            {
                tag += String(mfrc522.uid.uidByte[i], HEX);
            }
            tag.toUpperCase();
            Serial.print("Scanned Tag: ");
            Serial.println(tag);
            lcd.clear();
            lcd.print("ID: " + tag);

            bool authorized = false;
            for (String t : authorizedTags)
            {
                if (t == tag)
                {
                    authorized = true;
                    break;
                }
            }

            // Validating tag
            if (authorized)
            {
                lcd.setCursor(0, 1);
                lcd.print("Access Granted");
                digitalWrite(GREEN_LED_PIN, HIGH);
                accessFeed.publish(("Access Granted: " + tag).c_str());
                statusFeed.publish(1);
                delay(2000);
                digitalWrite(GREEN_LED_PIN, LOW);
            }
            else // When an unauthorised RFID tag is scanned
            {
                lcd.setCursor(0, 1);
                lcd.print("Unauthorized!");
                digitalWrite(GREEN_LED_PIN, LOW);
                digitalWrite(ALERT_PIN, HIGH);
                alertFeed.publish(("ALERT: Unauthorized access\nTag: " + tag).c_str());
                statusFeed.publish(0);
                delay(6000);
                digitalWrite(ALERT_PIN, LOW);
            }

            scanned = true;
            break;
        }

        if (!scanned) // When no RFID tag is scanned
        {
            lcd.clear();
            lcd.print("ALERT: No Tag!");
            digitalWrite(GREEN_LED_PIN, LOW);
            digitalWrite(ALERT_PIN, HIGH);
            alertFeed.publish("ALERT: Theft detected!");
            statusFeed.publish(0);
            delay(4000);
            digitalWrite(ALERT_PIN, LOW);
        }
        lcd.clear();
        lcd.print("System Ready");
    }

    delay(100);
}

// MQTT connect function
void MQTT_connect()
{
    int8_t ret;
    if (mqtt.connected())
        return;

    Serial.print("Connecting to MQTT...");
    while ((ret = mqtt.connect()) != 0)
    {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT in 5 seconds...");
        mqtt.disconnect();
        delay(5000);
    }
    Serial.println("MQTT Connected!");
    lcd.clear();
    lcd.print("MQTT Connected");
}
