#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include "twilio.hpp"
static const char *ssid = "POCO M4 Pro";
static const char *password = "Aditya586";
// Values from Twilio (find them on the dashboard)
static const char *account_sid = "AC37d6e7b2dc12c19c214c6db766fa4f19";
static const char *auth_token = "0999d35e5ff2a2ae00ee338478c87d24";
static const char *from_number = "+13375105934";
static const char *to_number = "+918008157736";
static const char *message = "Hi";
Twilio *twilio;
void setup() {
    Serial.begin(9600);
    Serial.print("Connecting to WiFi network '");
    Serial.print(ssid);
    Serial.println("'...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting...");
        delay(500);
    }
    Serial.println("Connected!");
    twilio = new Twilio(account_sid, auth_token);
    delay(1000);
    String response;
    bool success = twilio->send_message(to_number, from_number, message, response);
    if (success) {
        Serial.println("Sent message successfully!");
    } else {
        Serial.println(response);
    }
}
void loop() {
    int volt = analogRead(A0); // read the input
    double voltage = map(volt, 0, 1023, 0, 2500) + offset; // map 0-1023 to 0-2500 and add correction offset
    voltage /= 100; // divide by 100 to get the decimal values
    float unit = pow(voltage, 2) / 30;
    float rs = 0, t = 0, tot = 0;
    if (unit <= 100) {
        rs = (unit > 50) ? ((unit - 50) * 3.10 + 97.5) : (unit * 1.95);
    } else if (unit <= 200) {
        rs = (unit > 100) ? ((unit - 100) * 4.8 + 340) : (unit * 3.4);
    } else {
        while (unit > 0) {
            if (unit > 800) {
                t = (unit - 800) * 10;
                rs += t;
                unit -= (unit - 800);
            } else if (unit > 400) {
                t = (unit - 400) * 9.5;
                rs += t;
                unit -= (unit - 400);
            } else if (unit > 300) {
                t = (unit - 300) * 9;
                rs += t;
                unit -= (unit - 300);
            } else if (unit > 200) {
                t = (unit - 200) * 7.7;
                rs += t;
                unit -= (unit - 200);
            } else {
                t = unit * 5.1;
                rs += t;
                break;
            }
        }
    }
    Serial.print("Units: ");
    Serial.println(unit);
    tot = 10 + rs + (0.06 * unit);
    Serial.print("Bill: ");
    Serial.println(tot);
    delay(100000);
}
