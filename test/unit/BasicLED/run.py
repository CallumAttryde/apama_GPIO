from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()
		self.initialPinSetup(lowPins=[17])

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		self.waitForSignal('correlator.out', expr='LED on', errorExpr=['TEST FAILED'], timeout=10)
		self.checkPin(17, 1)
		self.correlator.sendEventStrings('Step(1)')
		self.waitForSignal('correlator.out', expr='LED off', errorExpr=['TEST FAILED'], timeout=10)
		self.checkPin(17, 0)

	def validate(self):
		pass
