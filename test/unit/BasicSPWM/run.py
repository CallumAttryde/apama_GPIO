from GPIO.BaseTest import GPIOBaseTest

class PySysTest(GPIOBaseTest):

	def execute(self):
		self.start()

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		self.waitForSignal('correlator.out', expr='LED off', errorExpr=['TEST FAILED'], timeout=10)
		self.correlator.sendEventStrings('ActivateSoftPWM({500:1500,501:5000,790:800})')
		# Expected results ~ 33.33% 10% and 98.75%
		self.waitForSignal('correlator.out', expr='Test Complete', errorExpr=['Error','Test Failed'], timeout=60)

	def validate(self):
		self.assertGrep(file="correlator.out", expr="Test Passed");
