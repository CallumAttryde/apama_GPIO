#include "SPIPlugin.h"

using namespace com::softwareag::connectivity;

bool SPI::setup(int64_t channel, int64_t speed)
{
	int res = wiringPiSPISetup(channel, speed);
	if(res == -1)
	{
		// TODO: log something
		return false;
	}

	m_channel = channel;
	m_speed = speed;

	return true;
}

void SPI::read()
{
	
}

void SPI::write(int64_t value)
{
	digitalWrite(value);
}

std::string SPI::readWrite(const char* data, int64_t length)
{
	int res = wiringPiSPIDataRW(m_channel, data, length);
}
