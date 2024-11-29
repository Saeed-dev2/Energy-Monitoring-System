#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// Current variables
int16_t sensorVALue2 = 0;
int VAL[100];
int max_i = 0;
double ImaxD = 0;
float current = 0;
double Ieff = 0;

// Energy variables
volatile int cnt = 0;

void CURRENt() {
    for (int i = 0; i < 100; i++) {
        sensorVALue2 = ads.readADC_SingleEnded(1);
        if (sensorVALue2 > 0) {
            VAL[i] = sensorVALue2;
        } else {
            VAL[i] = 0;
        }
        delay(1);
    }

    max_i = 0;

    for (int i = 0; i < 100; i++) {
        if (VAL[i] > max_i) {
            max_i = VAL[i];
        }
        VAL[i] = 0;
    }

    if (max_i != 0) {
        ImaxD = max_i;
        Ieff = ImaxD * 0.125;
        current = ((pow(10, -3) * 6.870027753458 * sq(Ieff)) - (5.38066 * Ieff) + 1482.40) / 1000;
    } else {
        current = 0;
    }

    ImaxD = 0;
}

void ISR() { // Interrupt Service Routine for energy meter
    cnt++;
}
