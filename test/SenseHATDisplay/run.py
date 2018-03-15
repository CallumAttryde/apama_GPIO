from senseHAT.BaseTest import SenseHATBaseTest

class PySysTest(SenseHATBaseTest):
	def execute(self):
		self.clearPixels()
		self.start()
		self.correlator.injectMonitorscript(filenames=['display.mon'])

		self.waitForSignal('correlator.out', expr='Finished loading')

		self.checkPixel(5, 4, [255, 255, 0])
		self.checkPixel(5, 5, [255, 0, 0])
		self.checkPixel(5, 6, [0, 255, 0])
		self.checkPixel(5, 7, [0, 0, 255])

		self.clearPixels()

	def validate(self):
		pass
