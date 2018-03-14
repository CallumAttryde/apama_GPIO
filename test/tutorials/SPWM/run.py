from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()
		self.initialPinSetup(lowPins=[17])

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		self.waitForSignal('correlator.out', expr='LED off', errorExpr=['TEST FAILED'], timeout=10)
		self.checkPin(17, 0)
		print "LED OFF"
		self.correlator.sendEventStrings('ActivateSoftPWM(0, 100)')
		self.waitForSignal('correlator.out', expr='Test Complete', errorExpr=['Error'], timeout=60)

	def validate(self):
		pass
