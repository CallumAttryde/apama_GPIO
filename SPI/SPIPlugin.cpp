#include "SPIPlugin.h"

using namespace com::softwareag::connectivity;

void SPIPlugin::initialize(base_plugin_t::method_data_t & md)
{
	//md.registerMethod<decltype(&SPIPlugin::convertToBitSequence), &SPIPlugin::convertToBitSequence>("convertToBitSequence", "action<any> returns sequence<bool>");
	md.registerMethod<decltype(&SPIPlugin::setup), &SPIPlugin::setup>("setup", "action<integer, integer> returns boolean");
	md.registerMethod<decltype(&SPIPlugin::read), &SPIPlugin::read>("read", "action<> returns boolean");
	md.registerMethod<decltype(&SPIPlugin::write), &SPIPlugin::write>("write", "action<integer>");
	md.registerMethod<decltype(&SPIPlugin::sync), &SPIPlugin::sync>("sync", "action<>");
	md.registerMethod<decltype(&SPIPlugin::readWrite), &SPIPlugin::readWrite>("readWrite", "action<sequence<integer>, integer> returns sequence<integer>");
	md.registerMethod<decltype(&SPIPlugin::block), &SPIPlugin::block>("block", "action<integer>");
}

bool SPIPlugin::setup(int64_t channel, int64_t speed)
{
	int res = wiringPiSPISetup(channel, speed);
	if(res == -1)
	{
		return false;
	}

	m_channel = channel;
	m_speed = speed;

	return true;
}

bool SPIPlugin::read()
{
	return digitalRead(MISO_PIN);
}

void SPIPlugin::sync()
{
	digitalWrite(SCLK_PIN, 1);
//	digitalWrite(SCLK_PIN, 0);	
}

void SPIPlugin::write(int64_t value)
{
	digitalWrite(MOSI_PIN, value);
	sync();
}

com::softwareag::connectivity::list_t SPIPlugin::readWrite(const com::softwareag::connectivity::list_t& data, int64_t length)
{
	unsigned char buffer[1024];
	int size = length < 1024 ? length : 1024;
	for(int i(0); i < size; i++)
	{
		buffer[i] = (int)get<int64_t>(data[i]);
	}
	int res = wiringPiSPIDataRW(m_channel, buffer, size);
	com::softwareag::connectivity::list_t returnData;
	for(int i(0); i < size; i++)
		returnData.push_back(com::softwareag::connectivity::data_t((int64_t)buffer[i]).copy());
	return returnData;
}

void SPIPlugin::block(int64_t milliseconds)
{
	delay(milliseconds);
}

/*list_t SPIPlugin::convertToBitSequence(const data_t& value)
{
	switch( value.type_tag() )
	{
		case SAG_DATA_INTEGER:
		{
			convertToBitSequence(value.get<int64_t>());
		}
		break;
		case SAG_DATA_DOUBLE:
		{
			convertToBitSequence(value.get<double>());
		}
		break;
		case SAG_DATA_DECIMAL:
		{
			convertToBitSequence(value.get<decimal_t>());
		}
		break;
		case SAG_DATA_STRING:
		{
			convertToBitSequence(value.get<const char*>());
		}
		break;
	}
}

list_t SPIPlugin::convertToBitSequence(int64_t value)
{
	list_t result;
	int numBits = numeric_limits<int64_t>::digits;
	int64_t maxMask = 1 << (numBits - 1);
	for (int b = 0; b < numBits; b++)
	{
		result.push_back(value & (maxMask >> b));		
	}

	return result;
}

list_t SPIPlugin::convertToBitSequence(double value)
{
	list_t result;
	int numBits = numeric_limits<double>::digits;
	double maxMask = 1 << (numBits - 1);
	for (int b = 0; b < numBits; b++)
	{
		result.push_back(value & (maxMask >> b));		
	}

	return result;
}

list_t SPIPlugin::convertToBitSequence(decimal_t value)
{
	list_t result;
	int numBits = numeric_limits<decimal_t>::digits;
	decimal_t maxMask = 1 << (numBits - 1);
	for (int b = 0; b < numBits; b++)
	{
		result.push_back(value & (maxMask >> b));		
	}

	return result;
}

list_t SPIPlugin::convertToBitSequence(const char* value)
{
	list_t result;
	int numBits = 4;
	char maxMask = 1 << (numBits - 1);
	for (int b = 0; b < numBits; b++)
	{
		result.push_back(value & (maxMask >> b));		
	}

	return result;
}*/
