from SPI.BaseTest import SPIBaseTest

class PySysTest(SPIBaseTest):

	def execute(self):
		self.start()

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
	        self.wait(10)

	def validate(self):
		pass
