/*
 * Copyright 2020 ARTS Lab @ University of South Carolina
 * Author: Hung-Tien Huang
 */

#include "SCA3300.h"

namespace sca3300_library {
	// private spi frames declarations
	const uint8_t SCA3300::READ_ACC_X[] = { 0b100,0,0,0b11110111 };
	const uint8_t SCA3300::READ_ACC_Y[] = { 0b1000,0,0,0b11111101 };
	const uint8_t SCA3300::READ_ACC_Z[] = { 0b1100,0,0,0b11111011 };
	const uint8_t SCA3300::READ_STO[] = { 0b10000,0,0,0b11101001 };
	const uint8_t SCA3300::READ_TEMPERATURE[] = { 0b10100,0,0,0b11101111 };
	const uint8_t SCA3300::READ_STATUS_SUMMARY[] = { 0b11000,0b0,0b0,0b11100101 };
	const uint8_t SCA3300::READ_CMD[] = { 0b110100,0,0,0b11011111 };
	const uint8_t SCA3300::CHANGE_TO_MODE1[] = { 0b10110100,0,0,0b11111 };
	const uint8_t SCA3300::CHANGE_TO_MODE2[] = { 0b10110100,0,0b1,0b10 };
	const uint8_t SCA3300::CHANGE_TO_MODE3[] = { 0b10110100,0,0b10,0b100101 };
	const uint8_t SCA3300::CHANGE_TO_MODE4[] = { 0b10110100,0,0b11,0b111000 };
	const uint8_t SCA3300::SET_POWER_DOWN_MODE[] = { 0b10110100,0,0b100,0b1101011 };
	const uint8_t SCA3300::WAKE_UP_FROM_POWER_DOWN_MODE[] = { 0b10110100,0,0,0b11111 };
	const uint8_t SCA3300::SW_RESET[] = { 0b10110100, 0b0, 0b100000, 0b10011000 };
	const uint8_t SCA3300::READ_WHOAMI[] = { 0b1000000, 0b0, 0b0, 0b10010001 };
	const uint8_t SCA3300::READ_SERIAL1[] = { 0b1100100,0,0,0b10100111 };
	const uint8_t SCA3300::READ_SERIAL2[] = { 0b1101000,0,0,0b10101101 };
	const uint8_t SCA3300::READ_CURRENT_BANK[] = { 0b1111100,0,0,0b10110011 };
	const uint8_t SCA3300::SWITCH_TO_BANK_ZERO[] = { 0b11111100,0,0,0b1110011 };
	const uint8_t SCA3300::SWITCH_TO_BANK_ONE[] = { 0b11111100,0,0b1,0b1101110 };

	// constructor
	SCA3300::SCA3300(const uint8_t chipSelect, const uint32_t spiSpeed, const OperationMode operationMode, bool showLog) :chipSelect(chipSelect), spiSettings(spiSpeed, MSBFIRST, SPI_MODE0), operationMode(operationMode), showLog(showLog)
	{
		pinMode(chipSelect, OUTPUT);
	}

	// public methods
	bool SCA3300::initChip() const
	{
		uint32_t start = micros();
		digitalWrite(chipSelect, HIGH);
		SPI.begin();
		// step 2 write sw reset command
		if (showLog) {
			Serial.println("Step 2 : SW Reset");
		}
		uint8_t data[FRAME_LENGTH];
		send(SW_RESET, data);
		if (showLog) {
			for (size_t i = 0; i < FRAME_LENGTH; ++i)
			{
				Serial.print(data[i], BIN);
				Serial.print(" ");
			}
			Serial.println();
		}
		// step 3 wait 1 ms
		delay(1);
		// step 4 set measurement mode
		if (showLog)
		{
			Serial.println("Step 4 : Set Measurement Mode");
		}
		// step 5 wait 15 ms (M1-M3) or 100 ms (M4)
		//uint8_t returnStatus[4];// Table 10, 4 RS to be checked
		switch (operationMode)
		{
		case OperationMode::MODE1:
			send(CHANGE_TO_MODE1, data);
			delay(15);
			break;
		case OperationMode::MODE2:
			send(CHANGE_TO_MODE2, data);
			delay(15);
			break;
		case OperationMode::MODE3:
			send(CHANGE_TO_MODE3, data);
			delay(15);
			break;
		case OperationMode::MODE4:
			send(CHANGE_TO_MODE4, data);
			delay(100);
			break;
		}
		if (showLog) {
			for (size_t i = 0; i < FRAME_LENGTH; ++i)
			{
				Serial.print(data[i], BIN);
				Serial.print(" ");
			}
			Serial.println();
		}
		//returnStatus[0] = getReturnStatus(data);
		// Step 6 - 8, RS should be '11', '11', '01'
		if (showLog)
		{
			Serial.println("Step 6 - 8 : Get Return Status");
		}
		for (size_t i = 0; i < 3; ++i) {
			send(READ_STATUS_SUMMARY, data);
			//returnStatus[i + 1] = getReturnStatus(data);
			if (showLog) {
				for (size_t i = 0; i < FRAME_LENGTH; ++i)
				{
					Serial.print(data[i], BIN);
					Serial.print(" ");
				}
				Serial.println();
			}
		}
		//if (showLog)
		//{
		//	Serial.println("Return Status");
		//	for (size_t i = 0; i < 4; ++i)
		//	{
		//		Serial.print(returnStatus[i] + " ");
		//	}
		//	Serial.println();
		//}
		// check RS to verify whether setup successfully
		//if (returnStatus[0] == 0b11 && returnStatus[1] == 0b11 && returnStatus[2] == 0b11 && returnStatus[3] == 0b01)
		uint32_t end = micros();
		if (showLog == true)
		{
			Serial.printf("Start %ld End %lu\n", start, end);
		}
		if (getReturnStatus(data) == 0b01)
		{
			if (showLog == true)
			{
				Serial.println("Initialization Succeed");
			}
			return true;
		}
		if (showLog == true)
		{
			Serial.println("Initialization Failed");
		}
		return false;
	}
	bool SCA3300::chageMode(OperationMode mode)
	{
		this->operationMode = mode;
		return initChip();
	}
	OperationMode SCA3300::getOperationMode() const
	{
		return operationMode;
	}
	const bool SCA3300::getReturnStatus() const
	{
		uint8_t data[FRAME_LENGTH];
		send(READ_STATUS_SUMMARY, data);
		send(READ_STATUS_SUMMARY, data);
		uint8_t RS = getReturnStatus(data);
		if ((RS & 0b11) == 0b01)
		{
			return true;
		}
		return false;
	}
	double SCA3300::getAccel(Axis axis) const
	{
		uint8_t data[FRAME_LENGTH];
		switch (axis)
		{
		case sca3300_library::Axis::X:
			send(READ_ACC_X, data);
			send(READ_ACC_X, data);
			break;
		case sca3300_library::Axis::Y:
			send(READ_ACC_Y, data);
			send(READ_ACC_Y, data);
			break;
		case sca3300_library::Axis::Z:
			send(READ_ACC_Z, data);
			send(READ_ACC_Z, data);
			break;
		default:
			return static_cast<double>(ERROR_VALUE);
		}
		//send(READ_WHOAMI, data);
		if (getReturnStatus(data) == 0b01)
		{
			//int16_t rawAccel = (static_cast<uint16_t>(data[1]) << 8) | (static_cast<uint8_t>(data[2]));
			int16_t rawAccel = convertData(data);
			//switch (operationMode)
			//{
			//case OperationMode::MODE1:
			//	return rawAccel / 2700.0;
			//case OperationMode::MODE2:
			//	return rawAccel / 1350.0;
			//case OperationMode::MODE3:
			//case OperationMode::MODE4:
			//	return rawAccel / 5400.0;
			//}
			return convertRawAccelToAccel(rawAccel, operationMode);
		}
		initChip();
		return getAccel(axis);
	}

	int16_t SCA3300::getAccelRaw(Axis axis) const
	{
		uint8_t data[FRAME_LENGTH];
		switch (axis)
		{
		case sca3300_library::Axis::X:
			send(READ_ACC_X, data);
			send(READ_ACC_X, data);
			break;
		case sca3300_library::Axis::Y:
			send(READ_ACC_Y, data);
			send(READ_ACC_Y, data);
			break;
		case sca3300_library::Axis::Z:
			send(READ_ACC_Z, data);
			send(READ_ACC_Z, data);
			break;
		default:
			return 0.0;
		}
		//send(READ_WHOAMI, data);
		if (getReturnStatus(data) == 0b01)
		{
			//int16_t rawAccel = (static_cast<uint16_t>(data[1]) << 8) | (static_cast<uint8_t>(data[2]));
			return convertData(data);
		}
		initChip();
		return getAccelRaw(axis);
	}

	double SCA3300::getTemp() const
	{
		uint8_t data[FRAME_LENGTH];
		send(READ_TEMPERATURE, data);
		send(READ_WHOAMI, data);
		int16_t rawTemp = convertData(data);
		if (getReturnStatus(data) == 0b01)
		{
			//return -273.0 + (rawTemp / 18.9);
			return convertRawTempToTemp(rawTemp);
		}
		initChip();
		return getTemp();
	}

	int16_t SCA3300::getTempRaw() const
	{
		uint8_t data[FRAME_LENGTH];
		send(READ_TEMPERATURE, data);
		send(READ_WHOAMI, data);
		if (getReturnStatus(data) == 0b01)
		{
			return convertData(data);
		}
		initChip();
		return getTempRaw();
	}

	const uint16_t SCA3300::getWhoAmI() const
	{
		uint8_t data[FRAME_LENGTH];
		send(READ_WHOAMI, data);
		send(READ_WHOAMI, data);
		return static_cast<uint16_t>(convertData(data));
	}

	double SCA3300::convertRawAccelToAccel(uint16_t rawAccel, const OperationMode& operationMode)
	{
		switch (operationMode)
		{
		case OperationMode::MODE1:
			return rawAccel / 2700.0;
		case OperationMode::MODE2:
			return rawAccel / 1350.0;
		case OperationMode::MODE3:
		case OperationMode::MODE4:
			return rawAccel / 5400.0;
		}
		return 0.0;
	}

	double SCA3300::convertRawTempToTemp(uint16_t rawTemp)
	{
		return -273.0 + (rawTemp / 18.9);
	}

	// private instance methods
	void SCA3300::send(const uint8_t spiFrame[FRAME_LENGTH], uint8_t ret[FRAME_LENGTH]) const
	{
		// copy data
		for (size_t i = 0; i < FRAME_LENGTH; ++i) {
			ret[i] = spiFrame[i];
		}
		// transmnit
		SPI.beginTransaction(spiSettings);
		digitalWrite(chipSelect, LOW);
		SPI.transfer(ret, FRAME_LENGTH);
		SPI.endTransaction();
		digitalWrite(chipSelect, HIGH);
	}

	// private static methods
	const uint8_t SCA3300::getReturnStatus(const uint8_t spiFrame[FRAME_LENGTH])
	{
		return spiFrame[0] & 0b11; // Table 13, RS [25:24] of the frame
	}
	const int16_t SCA3300::convertData(const uint8_t data[FRAME_LENGTH])
	{
		return static_cast<int16_t>((static_cast<uint16_t>(data[1]) << 8) | (static_cast<uint16_t>(data[2])));
	}

	const bool SCA3300::checkCRC(const uint8_t spiFrame[FRAME_LENGTH])
	{
		uint8_t crc = 0xFF;
		for (size_t i = 0; i < FRAME_LENGTH - 1; ++i)
		{
			for (size_t i = 0; i < 8; ++i)
			{
				uint8_t bitValue = (spiFrame[0] >> (8 - 1 - i)) & 0x01;
				crc = crc8(bitValue, crc);
			}
		}
		crc = (uint8_t)~crc;
		return crc == spiFrame[3];
	}

	const uint8_t SCA3300::crc8(uint8_t bitValue, uint8_t crc)
	{
		uint8_t temp = crc & 0x00;
		if (bitValue == 0x01) {
			temp ^= 0x80;
		}
		crc <<= 1;
		if (temp > 0) {
			crc ^= 0x1D;
		}
		return crc;
	}

};
