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
	md.registerMethod<decltype(&GPIOPlugin::softPWMCreate), &GPIOPlugin::softPWMCreate>("softPWMCreate", "action<integer, float, float>");
	md.registerMethod<decltype(&GPIOPlugin::softPWMWrite), &GPIOPlugin::softPWMWrite>("softPWMWrite", "action<integer, float>");
	md.registerMethod<decltype(&GPIOPlugin::softPWMStop), &GPIOPlugin::softPWMStop>("softPWMStop", "action<integer>");
	md.registerMethod<decltype(&GPIOPlugin::getDigitalRead), &GPIOPlugin::getDigitalRead>("getDigitalRead", "action<integer> returns integer");
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

void GPIOPlugin::softPWMCreate(int64_t pinId, double dutyCycle, double range)
{
	checkPinValid(pinId);

	softPwmCreate(pinId, dutyCycle, range);
	softPwmEnabled[pinId] = true;
}

void GPIOPlugin::softPWMWrite(int64_t pinId, double dutyCycle)
{
	checkPinValid(pinId);

	if (softPwmEnabled[pinId])
	{
		softPwmWrite(pinId, dutyCycle);
	}
	else
	{
		// change to exception
		pluginInstance->logger.error("This pin needs to first have a SoftPWM created on it with a valid range");		
	}
}

void GPIOPlugin::softPWMStop(int64_t pinId)
{
	checkPinValid(pinId);

	softPwmStop(pinId);
	softPwmEnabled[pinId] = false;
}

int64_t GPIOPlugin::getDigitalRead(int64_t pinId){
	checkPinValid(pinId);
	return (digitalRead(pinId)==LOW? 0:1);
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
