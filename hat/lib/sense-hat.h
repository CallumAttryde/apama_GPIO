/*
 *
 */

#include <stdint.h>

class RTHumidity;
class RTPressure;

class SenseHAT {
public:
	SenseHAT();

	// Methods for manipulating LED matrix
	int blank();
	int set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

	// Sensor methods
	double get_temperature();
	double get_humidity();
	double get_temperature_from_humidity();
	double get_pressure();
	double get_temperature_from_pressure();

private:
	int fbfd;

	int init_fb();

	RTHumidity* humidity;
	RTPressure* pressure;
};
