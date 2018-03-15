from senseHAT.BaseTest import SenseHATBaseTest
from random import randint

class PySysTest(SenseHATBaseTest):
	def execute(self):
		self.start()
		self.correlator.injectMonitorscript(filenames=['sensors.mon'])

		self.waitForSignal('correlator.out', expr='Finished loading')

		print self.getTemperature()
		print self.getHumidity()
		print self.getPressure()

	def validate(self):
		pass

