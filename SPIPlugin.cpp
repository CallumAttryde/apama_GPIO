#include "SPIPlugin.h"

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

void SPI::sync()
{
	digitalWrite(CLK_PIN, 1);
	digitalWrite(CLK_PIN, 0);	
}

void SPI::write(int64_t value)
{
	digitalWrite(DAT_PIN, value);
	sync();
}

std::string SPI::readWrite(const char* data, int64_t length)
{
	unsigned char buffer[1024];
	int size = length < 1024 ? length : 1024;
	for(int i(0); i < size; i++)
	{
		buffer[i] = data[i];
	}
	int res = wiringPiSPIDataRW(m_channel, buffer, size);
	std::string returnData = data;
	return returnData;
}
