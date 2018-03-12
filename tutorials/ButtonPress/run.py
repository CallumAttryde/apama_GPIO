from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()
		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		self.wait(4.0) # wait for monitor to load - currently investigating a strange corruption issue when using waitForSignal
		self.writePin(22, 1)
		self.waitForSignal('correlator.out', expr='Button Pressed')

	def validate(self):
		pass
