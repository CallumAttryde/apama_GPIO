#ifndef CORRELATOR_PLUGINS_SPI_H
#define CORRELATOR_PLUGINS_SPI_H

class SPI
{
public:
	SPI() {}
	~SPI() {}
	
	bool setup(int64_t channel, int64_t speed);

	void read();
	void write(int64_t value);
	std::string readWrite(const char* data, int64_t length);

private:
	int64_t m_channel;		// 0 or 1. Dictates which channel on the Pi to use.
	int64_t m_speed;		// 500,000 - 32,000,000, represents clock speed in Hz
};

#endif
