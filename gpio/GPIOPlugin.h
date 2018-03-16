#ifndef CORRELATOR_PLUGINS_GPIOPlugin_H
#define CORRELATOR_PLUGINS_GPIOPlugin_H

#include <epl_plugin.hpp>

#include <wiringPi.h>
#include <softPwm.h>

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

	/**  */
	void watch(int64_t pinId, int64_t eplEdge);

	/** Read the value of a pin */
	bool read(int64_t pinId);

	/** Write the value of a pin */
	void write(int64_t pinId, bool on);

	/** Block processing */
	void block(int64_t milliseconds);

	/** Set a Pin up for Software Pulse Width Modulation */
	void softPWMCreate(int64_t pinId, double dutyCycle, double range=0);

	/** Change the duty cycle on a Pin already set up for Soft PWM */
	void softPWMWrite(int64_t pinId, double dutyCycle);

	/** Stop SPWM on this pin */
	void softPWMStop(int64_t pinId);

	int64_t getDigitalRead(int64_t pin);
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
