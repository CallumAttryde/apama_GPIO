from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()
		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])		
		self.waitForSignal('correlator.out', expr='.*Monitor Loaded.*')
		self.writePin(22, 1)
		self.waitForSignal('correlator.out', expr='Button Pressed')

	def validate(self):
		pass
