#include "GPIOPlugin.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

using namespace com::softwareag::connectivity;

namespace
{
	typedef void (*fptr_t)();

	// interrupt closure
	template<int PIN>
	void interrupt_cb_t()
	{
		pluginInstance->interruptCallback(PIN);
	}

	std::array<fptr_t, MAX_PIN> interruptTable 
	{
		interrupt_cb_t<0>, interrupt_cb_t<1>, interrupt_cb_t<2>, interrupt_cb_t<3>, interrupt_cb_t<4>, 
		interrupt_cb_t<5>, interrupt_cb_t<6>, interrupt_cb_t<7>, interrupt_cb_t<8>, interrupt_cb_t<9>,
		interrupt_cb_t<10>, interrupt_cb_t<11>, interrupt_cb_t<12>, interrupt_cb_t<13>, interrupt_cb_t<14>, 
		interrupt_cb_t<15>, interrupt_cb_t<16>, interrupt_cb_t<17>, interrupt_cb_t<18>, interrupt_cb_t<19>,
		interrupt_cb_t<20>, interrupt_cb_t<21>, interrupt_cb_t<22>, interrupt_cb_t<23>, interrupt_cb_t<24>, 
		interrupt_cb_t<25>, interrupt_cb_t<26>, interrupt_cb_t<27>, interrupt_cb_t<28>, interrupt_cb_t<29>,
		interrupt_cb_t<30>, interrupt_cb_t<31>, interrupt_cb_t<32>, interrupt_cb_t<33>, interrupt_cb_t<34>, 
		interrupt_cb_t<35>, interrupt_cb_t<36>, interrupt_cb_t<37>, interrupt_cb_t<38>, interrupt_cb_t<39>,
	};
} // namespace

void GPIOPlugin::interruptCallback(int pin) {
	std::ostringstream oss;
	oss << "com.apamax.rpi.gpio.Interrupt(" << pin << ")";
	getCorrelator().sendEvent(oss.str());
}
void GPIOPlugin::initialize(base_plugin_t::method_data_t & md)
{
	md.registerMethod<decltype(&GPIOPlugin::setup), &GPIOPlugin::setup>("setup", "action<sequence<integer>, sequence<integer> > returns integer");
	md.registerMethod<decltype(&GPIOPlugin::getInfo), &GPIOPlugin::getInfo>("getInfo", "action<> returns dictionary<string, string>");
	md.registerMethod<decltype(&GPIOPlugin::watch), &GPIOPlugin::watch>("watch", "action<integer, integer>");
	md.registerMethod<decltype(&GPIOPlugin::read), &GPIOPlugin::read>("read", "action<integer> returns boolean");
	md.registerMethod<decltype(&GPIOPlugin::write), &GPIOPlugin::write>("write", "action<integer, boolean>");
	md.registerMethod<decltype(&GPIOPlugin::block), &GPIOPlugin::block>("block", "action<integer>");
	md.registerMethod<decltype(&GPIOPlugin::softPWM), &GPIOPlugin::softPWM>("softPWM", "action<integer, float>");
	//md.registerMethod<decltype(&GPIOPlugin::convertToBitSequence), &GPIOPlugin::convertToBitSequence>("convertToBitSequence", "action<any> returns sequence<bool>");
	md.registerMethod<decltype(&GPIOPlugin::SpiSetup), &GPIOPlugin::SpiSetup>("SpiSetup", "action<integer, integer> returns boolean");
	md.registerMethod<decltype(&GPIOPlugin::SpiRead), &GPIOPlugin::SpiRead>("SpiRead", "action<>");
	md.registerMethod<decltype(&GPIOPlugin::SpiWrite), &GPIOPlugin::SpiWrite>("SpiWrite", "action<integer>");
	md.registerMethod<decltype(&GPIOPlugin::SpiSync), &GPIOPlugin::SpiSync>("SpiSync", "action<>");
	md.registerMethod<decltype(&GPIOPlugin::SpiReadWrite), &GPIOPlugin::SpiReadWrite>("SpiReadWrite", "action<string, integer> returns string");
}

int64_t GPIOPlugin::setup(const list_t &inputPins, const list_t &outputPins)
{
	// check what happens if we export pins more than once - do we want to only allow setup to be called once?
	for (auto it = inputPins.begin(); it != inputPins.end(); ++it)
	{
		exportPin(get<int64_t>(*it), "in");
	}
	for (auto it = outputPins.begin(); it != outputPins.end(); ++it)
	{
		exportPin(get<int64_t>(*it), "out");
	}

	return wiringPiSetupSys();
}

map_t GPIOPlugin::getInfo()
{
	int model, rev, mem, maker, overVolt;
	piBoardId(&model, &rev, &mem, &maker, &overVolt);
	map_t info
	{
		// TODO intern these names
		std::make_pair(data_t("Model"), data_t((int64_t)model)),
		std::make_pair(data_t("Revision"), data_t((int64_t)rev)),
		std::make_pair(data_t("Memory"), data_t((int64_t)mem)),
		std::make_pair(data_t("Maker"), data_t((int64_t)maker)),
		std::make_pair(data_t("OverVolt"), data_t((int64_t)overVolt))
	};
	return info;
}

bool GPIOPlugin::SpiSetup(int64_t channel, int64_t speed)
{
	SpiInterface.setup(channel, speed);
}

void GPIOPlugin::SpiRead()
{
	SpiInterface.read();
}

void GPIOPlugin::SpiSync()
{
	SpiInterface.sync();
}

void GPIOPlugin::SpiWrite(int64_t value)
{
	SpiInterface.write(value);
}

std::string GPIOPlugin::SpiReadWrite(const char* data, int64_t length)
{
	return SpiInterface.readWrite(data, length);
}

/*list_t GPIOPlugin::convertToBitSequence(const data_t& value)
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

list_t GPIOPlugin::convertToBitSequence(int64_t value)
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

list_t GPIOPlugin::convertToBitSequence(double value)
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

list_t GPIOPlugin::convertToBitSequence(decimal_t value)
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

list_t GPIOPlugin::convertToBitSequence(const char* value)
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

void GPIOPlugin::watch(int64_t pinId, int64_t eplEdge)
{
	checkPinValid(pinId);

	switch (eplEdge)
	{
	case INT_EDGE_FALLING:
		digitalWrite(pinId, 1);
		break;
	case INT_EDGE_RISING:
		digitalWrite(pinId, 0);
		break;
	default:
		throw std::runtime_error("Unknown edge type passed to watch()");
	}

	wiringPiISR(pinId, eplEdge, interruptTable[pinId]);
}

bool GPIOPlugin::read(int64_t pinId)
{
	checkPinValid(pinId);
	return digitalRead(pinId);
}

void GPIOPlugin::write(int64_t pinId, bool on)
{
	checkPinValid(pinId);
	digitalWrite(pinId, on);
}

void GPIOPlugin::block(int64_t milliseconds)
{
	delay(milliseconds);
}

void GPIOPlugin::softPWM(int64_t pinId, double dutyCycle)
{
	checkPinValid(pinId);
	int64_t value = dutyCycle * 100;

	if (softPwmEnabled[pinId])
	{
		softPwmWrite(pinId, value);
	}
	else
	{
		softPwmCreate(pinId, value, 100);
		softPwmEnabled[pinId] = true;
	}
}

void GPIOPlugin::exportPin(const int64_t pin, const char * mode)
{
	std::ostringstream os;
	os << pin;

	std::vector<char*> argv;
	argv.push_back("/usr/bin/gpio");
	argv.push_back("export");
	argv.push_back(const_cast<char*>(os.str().c_str()));
	argv.push_back(const_cast<char*>(mode));
	argv.push_back(0);
	
	int result = executeWait(&argv[0]);
	if (0 != result) 
	{
		throw std::runtime_error("Failed to export pin '" + std::to_string(pin) + "', check '/usr/bin/gpio' exists.");
	}
}

int GPIOPlugin::executeWait(char* const argv[])
{
	pid_t childPid = fork();
	if (childPid == 0) {
		execve(argv[0], &argv[0], environ);
		return errno;
	} else if (childPid > 0) {
		int ret;
		waitpid(childPid, &ret, 0);
		return errno;
	} else {
		return errno;
	}
}
