#include <WiFi.h>
#include <FirebaseESP32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "a.c"
#include "b.c"

#define API_KEY "****************************************"
#define DATABASE_URL "***********************************"

const char *ssid = "**********";
const char *password = "**********";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org");

// Time variables
unsigned long epochTime;
int monthDay, currentMonth, currentYear, currentHour, currentMinute, currentSecond, pc = 0;

void setup() {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(14, INPUT_PULLUP); // Interrupt pin
    attachInterrupt(digitalPinToInterrupt(14), ISR, FALLING);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, LOW);

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    Firebase.begin(DATABASE_URL, API_KEY);

    timeClient.begin();
    timeClient.setTimeOffset(19800);

    ads.setGain(GAIN_ONE);
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS.");
        while (1);
    }
}

void loop() {
    timeClient.update();
    epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    monthDay = ptm->tm_mday;
    currentMonth = ptm->tm_mon + 1;
    currentYear = ptm->tm_year + 1900;

    currentHour = timeClient.getHours();
    currentMinute = timeClient.getMinutes();
    currentSecond = timeClient.getSeconds();

    voltage();
    CURRENt();

    Serial.println("Voltage: " + String(Veff) + " volt");
    Serial.println("EnergyImpulse: " + String(cnt));
    Serial.println("Current: " + String(current) + " amp");
    Serial.println(String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear));
    Serial.println(String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond));
    Serial.println(".........");

    if (currentMinute - pc == 1 || pc == 0) {
        Firebase.setInt(fbdo, "/SEM/EIMP", cnt);
        Firebase.setInt(fbdo, "/SEM/VOLTAGE", Veff);
        Firebase.setFloat(fbdo, "/SEM/CURRENT", current);

        Firebase.setInt(fbdo, "/SEM/DD", monthDay);
        Firebase.setInt(fbdo, "/SEM/MO", currentMonth);
        Firebase.setInt(fbdo, "/SEM/YY", currentYear);

        Firebase.setInt(fbdo, "/SEM/HH", currentHour);
        Firebase.setInt(fbdo, "/SEM/MI", currentMinute);

        pc = currentMinute;
    }

    delay(500);
}
