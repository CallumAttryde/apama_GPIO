#ifndef CORRELATOR_PLUGINS_GPIOPlugin_H
#define CORRELATOR_PLUGINS_GPIOPlugin_H

#include <epl_plugin.hpp>

#include <wiringPi.h>
#include <softPwm.h>

#include "SPIPlugin.h"

#include <array>

#define MAX_PIN 40

class GPIOPlugin;

static GPIOPlugin* pluginInstance;

class GPIOPlugin : public com::apama::epl::EPLPlugin<GPIOPlugin>
{
public:
	GPIOPlugin() : base_plugin_t("GPIOPluginPlugin") {
		pluginInstance = this;
	}
	~GPIOPlugin() {}

	/** Link the native plugin functions to the EPL plugin actions */
	static void initialize(base_plugin_t::method_data_t &md);

	void interruptCallback(int pin);

	/** Setup the pins we want to use */
	int64_t setup(const com::softwareag::connectivity::list_t &inputPins, const com::softwareag::connectivity::list_t &outputPins);

	/** Get info regarding this board */
	com::softwareag::connectivity::map_t getInfo();

	bool SpiSetup(int64_t channel, int64_t speed);
	void SpiRead();
	void SpiWrite(int64_t value);
	std::string SpiReadWrite(const char* data, int64_t length);
	/*com::softwareag::connectivity::list_t convertToBitSequence(const com::softwareag::connectivity::data_t& value);
	com::softwareag::connectivity::list_t convertToBitSequence(int64_t value);
	com::softwareag::connectivity::list_t convertToBitSequence(double value);
	com::softwareag::connectivity::list_t convertToBitSequence(com::softwareag::connectivity::decimal_t value);
	com::softwareag::connectivity::list_t convertToBitSequence(const char* value);*/

	/**  */
	void watch(int64_t pinId, int64_t eplEdge);

	/** Read the value of a pin */
	bool read(int64_t pinId);

	/** Write the value of a pin */
	void write(int64_t pinId, bool on);

	/** Block processing */
	void block(int64_t milliseconds);

	/** */
	void softPWM(int64_t pinId, double dutyCycle);

	SPI SpiInterface;

private:
	/** Table recording if each pin has softPWM enabled */
	std::array<bool, MAX_PIN> softPwmEnabled;

	/** Bounds checking on pin numbers */
	void checkPinValid(int64_t pinId)
	{
		if (pinId < 0 || pinId >= MAX_PIN) 
		{
			throw std::runtime_error("pinId '" + std::to_string(pinId) + "' outside of valid range.");
		}
	}

	/** Prepare an invocation of 'gpio' to export desired pins (so we don't have to run as root) */
	void exportPin(const int64_t pin, const char* mode);
	/** Call a process, waiting for it to complete */
	int executeWait(char *const argv[]);
};

APAMA_DECLARE_EPL_PLUGIN(GPIOPlugin)

#endif
