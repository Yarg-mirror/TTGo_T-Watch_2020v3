#include <Wire.h>
#include <axp20x.h>
#include "config.h"

AXP20X_Class axp;

void setup() {
    Serial.begin(115200);
    Wire.begin(i2c_sda, i2c_scl);

    int ret = axp.begin(Wire);

    if (ret == AXP_FAIL) {
        Serial.println("AXP Power begin failed");
        while (1);
    }

    // enable pek key press irq channel
    axp.enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);

    // attachInterrupt to gpio 35
    pinMode(axp_irq_pin, INPUT);
    attachInterrupt(axp_irq_pin, [] {
        axpIrq = 1;
    }, FALLING);
    axp.clearIRQ();
}

void loop() {
    if (axpIrq) {
        axpIrq = 0;
        axp.readIRQ();
        if (axp.isPEKShortPressIRQ()) {
            Serial.printf("AXP202 PEK key Click\n");
        }
        axp.clearIRQ();
    }
}
