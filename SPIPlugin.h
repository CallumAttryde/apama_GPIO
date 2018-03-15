#ifndef CORRELATOR_PLUGINS_SPI_H
#define CORRELATOR_PLUGINS_SPI_H

#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <string>

class SPI
{
public:
	SPI() {}
	~SPI() {}
	
	bool setup(int64_t channel, int64_t speed);

	void read();
	void write(int64_t value);
	void sync();		// Toggles the clock pin, sending data
	std::string readWrite(const char* data, int64_t length);

	static const int CLK_PIN = 27;	// Clock pin, used to synchronise data
	static const int DAT_PIN = 28;	// Data pin, sends value over SPI when clock pin is active

private:
	int64_t m_channel;		// 0 or 1. Dictates which channel on the Pi to use.
	int64_t m_speed;		// 500,000 - 32,000,000, represents clock speed in Hz
};

#endif
