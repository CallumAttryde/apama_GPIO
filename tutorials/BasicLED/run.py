from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()

		self.checkPin(17, 0)
		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		self.checkPin(17, 1)
		self.wait(10)

	def validate(self):
		pass
