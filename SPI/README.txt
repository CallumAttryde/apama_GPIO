Current status of SPI plugin:
    This plugin was tested without an SPI device. As a workaround to test if code was working, an LED was plugged into the MOSI pin. This is obviously an imperfect solution but gave an idea of if data was being sent using the SPI protocol. The LED lights up based on data being sent using the plugin, so it is at least partially functional. It would also light up if connected to the CLK pin.

    The tests currently just look for output to determine a success. Attempts were made to use both the GPIOPlugins watch method, and the rpi.gpio watch_for_edge function. Neither could successfully read the state of the pin, and we ran out of time to properly investigate further.

Future goals:
    Create a way to read state of pins so we can automate the tests
    Test this plugin with an SPI enabled device, and see that all functionality is actually working as expected.
    Improve the tests after performing the previously mentioned goals