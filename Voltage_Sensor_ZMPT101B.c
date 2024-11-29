#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// Voltage variables
int16_t sensorValue1 = 0;
int val[100];
int max_v = 0;
double VmaxD = 0, Veff = 0;

void voltage() {
    for (int i = 0; i < 100; i++) {
        sensorValue1 = ads.readADC_SingleEnded(0);
        if (sensorValue1 > 13200) {
            val[i] = sensorValue1;
        } else {
            val[i] = 0;
        }
        delay(1);
    }

    max_v = 0;

    for (int i = 0; i < 100; i++) {
        if (val[i] > max_v) {
            max_v = val[i];
        }
        val[i] = 0;
    }

    if (max_v != 0) {
        VmaxD = max_v;
        Veff = map(VmaxD, 17660, 17740, 230, 235);
        if (Veff < 230 || Veff > 235) {
            Veff = 0;
        }
    } else {
        Veff = 0;
    }

    VmaxD = 0;
}
