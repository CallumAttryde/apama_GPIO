#include "senseHATPlugin.h"

using namespace com::softwareag::connectivity;

void SenseHATPlugin::initialize(base_plugin_t::method_data_t &md)
{
	md.registerMethod<decltype(&SenseHATPlugin::blank), &SenseHATPlugin::blank>("blank", "action<>");
	md.registerMethod<decltype(&SenseHATPlugin::setPixel), &SenseHATPlugin::setPixel>("setPixel", "action<integer, integer, integer, integer, integer>");
	md.registerMethod<decltype(&SenseHATPlugin::getHumidity), &SenseHATPlugin::getHumidity>("getHumidity", "action<> returns float");
	md.registerMethod<decltype(&SenseHATPlugin::getPressure), &SenseHATPlugin::getPressure>("getPressure", "action<> returns float");
	md.registerMethod<decltype(&SenseHATPlugin::getTemperature), &SenseHATPlugin::getTemperature>("getTemperature", "action<> returns float");
}
