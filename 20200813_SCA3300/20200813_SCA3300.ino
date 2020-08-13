/*
 Name:		_20200813_SCA3300.ino
 Created:	8/13/2020 14:40:22
 Author:	Fred
*/

#include <SPI.h>

SPISettings setting(2500000, MSBFIRST, SPI_MODE0);
const byte CS = 10;
const byte FRAME_LENGTH = 4;
const uint8_t SW_RST[] = { 0b10110100, 0b0, 0b100000, 0b10011000 };
const uint8_t CHANGE_TO_MODE3[] = { 0b10110100, 0b0, 0b10, 0b100101 };
const uint8_t READ_WHOAMI[] = { 0b1000000, 0b0, 0b0, 0b10010001 };
const uint8_t READ_STO[] = { 0b10000, 0b0, 0b0, 0b11101001 };
const uint8_t READ_STATUS_SUMMARY[] = { 0b11000,0b0,0b0,0b11100101 };

void send(const uint8_t spiFrame[FRAME_LENGTH], uint8_t ret[FRAME_LENGTH]);
void printFrame(const uint8_t frame[FRAME_LENGTH]);

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(CS, OUTPUT);
	digitalWrite(CS, HIGH);
	//Serial.begin(9600);
	SPI.begin();
	// Step 2
	//Serial.println("Step 2 : SW Reset");
	
	uint8_t data[FRAME_LENGTH];
	byte res[4];
	send(SW_RST, data);
	// Step 3
	delay(1);
	// Step 4
	//Serial.println("Step 4 : Set Measurement Mode");
	send(CHANGE_TO_MODE3, data);
	//printFrame(data);// RS should be '11'
	res[0] = data[0];
	// Step 5
	delay(15);
	// Step 6 - 8, RS should be '11', '11', '01'
	for (size_t i = 0; i < 3; ++i) {
		//Serial.printf("Step %d : Read Status Summary\n", 6 + i);
		send(READ_STATUS_SUMMARY, data);
		//printFrame(data);
		res[i+1] = data[0];
	}
	Serial.begin(9600);
	for (size_t i = 0; i < 4; ++i)
	{
		Serial.print(res[i],BIN);
		Serial.print(" ");
	}
	Serial.println();
	// one who am i
	Serial.println("Do a WHOAMI");
	send(READ_WHOAMI, data);
	send(READ_WHOAMI, data);
	printFrame(data);
}

// the loop function runs over and over again until power down or reset
void loop() {

}

void send(const uint8_t spiFrame[FRAME_LENGTH], uint8_t ret[FRAME_LENGTH]) {
	// copy data
	for (size_t i = 0; i < FRAME_LENGTH; ++i) {
		ret[i] = spiFrame[i];
	}
	// transmnit
	SPI.beginTransaction(setting);
	digitalWrite(CS, LOW);
	SPI.transfer(ret, FRAME_LENGTH);
	SPI.endTransaction();
	digitalWrite(CS, HIGH);
}

void printFrame(const uint8_t frame[FRAME_LENGTH]) {
	for (size_t i = 0; i < FRAME_LENGTH; ++i) {
		Serial.print(frame[i], BIN);
		Serial.print(" ");
	}
	Serial.println();
}
