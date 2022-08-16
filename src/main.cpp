#include <Arduino.h>
#include <SoftwareSerial.h>

const int rxPin = 5;
const int txPin = 6;
const int pin_white = 9;
const int pin_yellow = 10;

const int signal_white_on = 'A';
const int signal_white_off = 'B';
const int signal_yellow_on = 'C';
const int signal_yellow_off = 'D';
const int signal_all_on = 'E';
const int signal_all_off = 'F';
const int signal_strobe_on = 'G';
const int signal_strobe_off = 'H';

bool state_white = 0;
bool state_yellow = 0;
bool state_strobe = 0;

int bt_data;

unsigned long startMillis;
unsigned long currentMillis;

SoftwareSerial bt(rxPin, txPin);

void updateSignal() {
	if (bt.available() > 0) {
		bt_data = bt.read();
		Serial.println(bt_data);

		switch (bt_data) {
			case signal_white_on:
				state_white = 1;
				break;
			case signal_white_off:
				state_white = 0;
				break;
			case signal_yellow_on:
				state_yellow = 1;
				break;
			case signal_yellow_off:
				state_yellow = 0;
				break;
			case signal_all_on:
				state_white = 1;
				state_yellow = 1;
				break;
			case signal_all_off:
				state_white = 0;
				state_yellow = 0;
				break;
			case signal_strobe_on:
				state_white = 0;
				state_yellow = 0;
				state_strobe = 1;
				break;
			case signal_strobe_off:
				state_strobe = 0;
				state_white = 0;
				state_yellow = 0;
				break;
		}
	}
	if (state_white == 1) digitalWrite(pin_white, HIGH);
	if (state_white == 0) digitalWrite(pin_white, LOW);
	if (state_yellow == 1) digitalWrite(pin_yellow, HIGH);
	if (state_yellow == 0) digitalWrite(pin_yellow, LOW);
}

void setup() {
	pinMode(pin_white, OUTPUT);
	pinMode(pin_yellow, OUTPUT);

	Serial.begin(115200);
	bt.begin(9600);
}

void loop() {
	updateSignal();
	while (state_strobe == 1) {
		currentMillis = millis();
		if (currentMillis - startMillis > 100) {
			state_white = !state_white;
			state_yellow = !state_yellow;

			startMillis = currentMillis;
		}
		updateSignal();
	}
}