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

		void send(const uint8_t spiFrame[FRAME_LENGTH], uint8_t ret[FRAME_LENGTH]) const;
		
		/*
		 * spiFrame[0] & 0b11
		 */
		const uint8_t getReturnStatus(const uint8_t spiFrame[FRAME_LENGTH]) const;

		/*
		 * convert data section of the frame back to signed 16-bit integer
		 */
		const int16_t convertData(const uint8_t spiFrame[FRAME_LENGTH])const;
	public:
		/*
		 * The constructor will only set up instance variable.
		 * User are responsible for calling initChip() to initialize the chip
		 */
		SCA3300(uint8_t chipSelect, uint32_t spiSpeed, OperationMode operationMode);

		/*
		 * Initialize the chip.
		 * If initialization succeed, the method returns ture and vice versa.
		 */
		bool initChip(bool showLog) const;

		/*
		 * Change operation mode and reinitialize the chip
		 * If change succeed, the method returns true and vice versa.
		 */
		bool chageMode(OperationMode mode);

		/*
		 * This method calls READ_STO 2 times and parse the return status
		 * If return status = 01, the method returns true and false otherwise.
		 */
		const bool getReturnStatus() const;

		/*
		 * Pass in the interested axis.
		 * This method converts raw data back to # of g(m/s^2) based on the set operation mode
		 * If RS of spi frame is not normal, then this method returns 0.0
		*/
		double getAccel(Axis axis) const;

		/*
		 * This method converts temperature data back to degree celsius.
		 * If RS of spi frame is not normal, then this method returns 0.0
		 */
		double getTemp() const;
	};

};

#endif
