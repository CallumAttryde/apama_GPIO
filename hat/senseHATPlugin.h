#ifndef CORRELATOR_PLUGINS_SenseHATPlugin_H
#define CORRELATOR_PLUGINS_SenseHATPlugin_H

#include <string>
#include <epl_plugin.hpp>
#include "sense-hat.h"

class SenseHATPlugin : public com::apama::epl::EPLPlugin<SenseHATPlugin>
{
public:
	SenseHATPlugin() : base_plugin_t("SenseHATPlugin"), hat() {}
	~SenseHATPlugin() {}

	/** Link the native plugin functions to the EPL plugin actions */
	static void initialize(base_plugin_t::method_data_t &md);

	/** Sensor functions */
	double getTemperature() { return hat.get_temperature(); }
	double getPressure() { return hat.get_pressure(); }
	double getHumidity() { return hat.get_humidity(); }

	/* Display functions */
	void setPixel(int64_t x, int64_t y, int64_t r, int64_t g, int64_t b)
	{
		int rc = hat.set_pixel(x, y, r, g, b);
		if (rc != 0) throw std::runtime_error("Could not set pixel, rc="+std::to_string(rc));
	}
	void blank()
	{
		int rc = hat.blank();
		if (rc != 0) throw std::runtime_error("Could not blank the display, rc="+std::to_string(rc));
	}

private:
	SenseHAT hat;
};

APAMA_DECLARE_EPL_PLUGIN(SenseHATPlugin)

#endif
