from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):
	def execute(self):
		self.start()
		self.correlator.injectMonitorscript(filenames=['display.mon'])
		self.wait(10.0)

	def validate(self):
		pass
