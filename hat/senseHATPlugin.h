#ifndef CORRELATOR_PLUGINS_GPIOPlugin_H
#define CORRELATOR_PLUGINS_GPIOPlugin_H

#include <epl_plugin.hpp>
#include <sense-hat.h>

class SenseHATPlugin : public com::apama::epl::EPLPlugin<SenseHATPlugin>
{
public:
	SenseHATPlugin() : base_plugin_t("SenseHATPlugin"), hat() {}
	~SenseHATPlugin() {}

	/** Link the native plugin functions to the EPL plugin actions */
//	static void initialize(base_plugin_t::method_data_t &md)
//	{
//		md.registerMethod<decltype(&SenseHATPlugin::getTemperature), &SenseHATPlugin::getTemperature>("getTemperature", "action<> returns integer");
//	}

	/** Sensor functions */
	double getTemperature() { return hat.get_temperature(); }
	double getPressure() { return hat.get_pressure(); }
	double getHumidity() { return hat.get_humidity(); }

	/* Display functions */
	int setPixel(int x, int y, int r, int g, int b)
	{
		return hat.set_pixel(x, y, r, g, b);
	}
	int blank() { return hat.blank(): }

private:
	SenseHAT hat;
}

APAMA_DECLARE_EPL_PLUGIN(SenseHATPlugin)

#endif
