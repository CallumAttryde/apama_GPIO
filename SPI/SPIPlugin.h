#ifndef CORRELATOR_PLUGINS_SPI_H
#define CORRELATOR_PLUGINS_SPI_H

#include <epl_plugin.hpp>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <string>

class SPIPlugin;

static SPIPlugin* pluginInstance;

class SPIPlugin : public com::apama::epl::EPLPlugin<SPIPlugin>
{
public:
	SPIPlugin() : base_plugin_t("SPIPluginPlugin") {
		pluginInstance = this;
	}
	~SPIPlugin() {}
	
	/** Link the native plugin functions to the EPL plugin actions */
	static void initialize(base_plugin_t::method_data_t &md);

	bool setup(int64_t channel, int64_t speed);

	bool read();
	void write(int64_t value);
	void sync();		// Toggles the clock pin, sending data
	com::softwareag::connectivity::list_t readWrite(const com::softwareag::connectivity::list_t& data, int64_t length);

	/*com::softwareag::connectivity::list_t convertToBitSequence(const com::softwareag::connectivity::data_t& value);
	com::softwareag::connectivity::list_t convertToBitSequence(int64_t value);
	com::softwareag::connectivity::list_t convertToBitSequence(double value);
	com::softwareag::connectivity::list_t convertToBitSequence(com::softwareag::connectivity::decimal_t value);
	com::softwareag::connectivity::list_t convertToBitSequence(const char* value);*/

	/** Block processing */
	void block(int64_t milliseconds);

	static const int CE1_PIN = 15;
	static const int CE0_PIN = 17;
	static const int MOSI_PIN = 19;		// Master Out, Slave In
	static const int MISO_PIN = 21;		// Master In, Slave Out
	static const int SCLK_PIN = 23;		// Clock pin, used to synchronise data

private:
	int64_t m_channel;		// 0 or 1. Dictates which channel on the Pi to use.
	int64_t m_speed;		// 500,000 - 32,000,000, represents clock speed in Hz
};

APAMA_DECLARE_EPL_PLUGIN(SPIPlugin)

#endif
