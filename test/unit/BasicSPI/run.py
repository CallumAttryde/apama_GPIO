from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		
	def validate(self):
		pass
