from SPI.BaseTest import SPIBaseTest

class PySysTest(SPIBaseTest):

	def execute(self):
		self.start()

		self.correlator.injectMonitorscript(filenames=['tutorial.mon'])
		
	def validate(self):
		pass
