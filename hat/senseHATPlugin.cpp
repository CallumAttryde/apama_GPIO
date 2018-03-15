#include "senseHATPlugin.h"

using namespace com::softwareag::connectivity;

void SenseHATPlugin::initialize(base_plugin_t::method_data_t &md)
{
	md.registerMethod<decltype(&SenseHATPlugin::blank), &SenseHATPlugin::blank>("blank", "action<> returns integer");
	md.registerMethod<decltype(&SenseHATPlugin::setPixel), &SenseHATPlugin::setPixel>("setPixel", "action<integer, integer, integer, integer, integer> returns integer");
}
