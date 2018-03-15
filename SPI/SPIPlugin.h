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

	void read();
	void write(int64_t value);
	void sync();		// Toggles the clock pin, sending data
	std::string readWrite(const char* data, int64_t length);

	/*com::softwareag::connectivity::list_t convertToBitSequence(const com::softwareag::connectivity::data_t& value);
	com::softwareag::connectivity::list_t convertToBitSequence(int64_t value);
	com::softwareag::connectivity::list_t convertToBitSequence(double value);
	com::softwareag::connectivity::list_t convertToBitSequence(com::softwareag::connectivity::decimal_t value);
	com::softwareag::connectivity::list_t convertToBitSequence(const char* value);*/

	/** Block processing */
	void block(int64_t milliseconds);

	static const int CLK_PIN = 27;	// Clock pin, used to synchronise data
	static const int DAT_PIN = 28;	// Data pin, sends value over SPI when clock pin is active

private:
	int64_t m_channel;		// 0 or 1. Dictates which channel on the Pi to use.
	int64_t m_speed;		// 500,000 - 32,000,000, represents clock speed in Hz
};

APAMA_DECLARE_EPL_PLUGIN(SPIPlugin)

#endif
