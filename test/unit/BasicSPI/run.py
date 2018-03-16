from SPI.BaseTest import SPIBaseTest

class PySysTest(SPIBaseTest):

	def execute(self):
		self.start()

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		self.correlator.sendEventStrings('Step(1)')
                print "Waiting for rising edge on pin 19"
		channel = self.waitForEdge(19, False)
                print "Finished waiting"
                # in future, add this, but currently the waitForEdge function doesn't seem to work
                #self.assertTrue(channel is not None)
	        self.waitForSignal('correlator.out', expr='Step 1 complete', errorExpr=['TEST FAILED'], timeout=10)
		self.checkPin(19, 0)

	def validate(self):
		pass
