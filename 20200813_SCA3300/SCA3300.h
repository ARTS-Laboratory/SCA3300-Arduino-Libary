/*
 * Copyright 2020 ARTS Lab @ University of South Carolina
 * Author: Hung-Tien Huang
 */

#ifndef _SCA3300_h
#define _SCA3300_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>
namespace sca3300_library {
	enum class OperationMode {
		MODE1, MODE2, MODE3, MODE4
	};

	enum class Axis {
		X, Y, Z
	};

	class SCA3300
	{
	private:
		// Table 14 Operations and equivalent SPI frames
		static const byte FRAME_LENGTH = 4;
		static const uint8_t ERROR_VALUE = 255;
		static const uint8_t READ_ACC_X[];
		static const uint8_t READ_ACC_Y[];
		static const uint8_t READ_ACC_Z[];
		static const uint8_t READ_STO[];
		static const uint8_t READ_TEMPERATURE[];
		static const uint8_t READ_STATUS_SUMMARY[];
		static const uint8_t READ_CMD[];
		static const uint8_t CHANGE_TO_MODE1[];
		static const uint8_t CHANGE_TO_MODE2[];
		static const uint8_t CHANGE_TO_MODE3[];
		static const uint8_t CHANGE_TO_MODE4[];
		static const uint8_t SET_POWER_DOWN_MODE[];
		static const uint8_t WAKE_UP_FROM_POWER_DOWN_MODE[];
		static const uint8_t SW_RESET[];
		static const uint8_t READ_WHOAMI[];
		static const uint8_t READ_SERIAL1[];
		static const uint8_t READ_SERIAL2[];
		static const uint8_t READ_CURRENT_BANK[];
		static const uint8_t SWITCH_TO_BANK_ZERO[];
		static const uint8_t SWITCH_TO_BANK_ONE[];

		const uint8_t chipSelect;
		const SPISettings spiSettings;
		OperationMode operationMode;
		bool showLog;

		void send(const uint8_t spiFrame[FRAME_LENGTH], uint8_t ret[FRAME_LENGTH]) const;

		/*
		 * spiFrame[0] & 0b11
		 */
		static const uint8_t getReturnStatus(const uint8_t spiFrame[FRAME_LENGTH]);

		/*
		 * convert data section of the frame back to signed 16-bit integer
		 */
		static const int16_t convertData(const uint8_t spiFrame[FRAME_LENGTH]);

		/*
		 * calculate crc for the given spiFrame
		 * spec p23
		 */
		static const bool checkCRC(const uint8_t spiFrame[FRAME_LENGTH]);

		static const uint8_t crc8(uint8_t bitValue, uint8_t crc);
	public:
		/*
		 * The constructor will only set up instance variable.
		 * User are responsible for calling initChip() to initialize the chip
		 */
		SCA3300(uint8_t chipSelect, uint32_t spiSpeed, OperationMode operationMode, bool showlog = false);

		/*
		 * Initialize the chip.
		 * If initialization succeed, the method returns ture and vice versa.
		 */
		bool initChip(/*bool showLog*/) const;

		/*
		 * Change operation mode and reinitialize the chip
		 * If change succeed, the method returns true and vice versa.
		 */
		bool chageMode(OperationMode mode);

		/*
		 * This method calls READ_STO 2 times and parse the return status
		 * 
		 * @return status = 01, the method returns true and false otherwise.
		 */
		const bool getReturnStatus() const;

		/*
		 * Pass in the interested axis and returns # of g(9.8 m/s^2)
		 * 
		 * @return acceleration in # of g(9.8m/s^2), 255 if spi abnormal
		 */
		double getAccel(Axis axis) const;

		/*
		 * Pass in the interested axis and returns acceleration
		 * 
		 * @ return raw acceleration data, 255 if spi abnormal
		 */
		int16_t getAccelRaw(Axis axis) const;

		/*
		 * This method converts temperature data back to degree celsius.
		 *
		 * @return temperatur in celsius, 255 if spi abnormal
		 */
		double getTemp() const;

		/*
		 * This method gets raw temperature data.
		 * 
		 * @ return raw temperature data, 255 if spi abnormal
		 */
		int16_t getTempRaw() const;

		/*
		 * Read who am i
		 * returned value should be 0x51
		 */
		const uint16_t getWhoAmI() const;
	};

};

#endif
