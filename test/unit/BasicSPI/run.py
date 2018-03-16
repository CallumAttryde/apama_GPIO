from SPI.BaseTest import SPIBaseTest

class PySysTest(SPIBaseTest):

	def execute(self):
		self.start()

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		self.correlator.sendEventStrings('Step(1)')
		self.peekPin(24, 1)
	        self.waitForSignal('correlator.out', expr='Step 1 complete', errorExpr=['TEST FAILED'], timeout=10)
		self.checkPin(24, 0)

	def validate(self):
		pass
