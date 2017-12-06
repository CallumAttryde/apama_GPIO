from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()
		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])		
		self.wait(30)

	def validate(self):
		pass
