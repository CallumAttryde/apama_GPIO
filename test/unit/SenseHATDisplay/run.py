from senseHAT.BaseTest import SenseHATBaseTest
from random import randint

class PySysTest(SenseHATBaseTest):
	def execute(self):
		self.clearPixels()
		self.start()
		self.correlator.injectMonitorscript(filenames=['display.mon'])

		verifyArray = []
		setterList = 'SetterList(['
		for x in range(0, 7):
			for y in range(0, 8):
				r = randint(0, 255)
				g = randint(0, 255)
				b = randint(0, 255)
				setter = 'Setter(%d,%d,%d,%d,%d)'%(x, y, r, g, b)
				setterList += setter + ','
				verifyArray.append([x, y, [r, g, b]])
				
		setterList = setterList[:-1] + '])'
		self.correlator.sendEventStrings(setterList)

		self.waitForSignal('correlator.out', expr='Finished loading')

		self.checkPixel(7, 0, [0, 0, 0])
		self.checkPixel(7, 1, [0, 255, 255])
		self.checkPixel(7, 2, [255, 0, 255])
		self.checkPixel(7, 3, [255, 255, 255])
		self.checkPixel(7, 4, [255, 255, 0])
		self.checkPixel(7, 5, [255, 0, 0])
		self.checkPixel(7, 6, [0, 255, 0])
		self.checkPixel(7, 7, [0, 0, 255])

		for values in verifyArray:
			self.checkPixel(values[0], values[1], values[2])

		self.clearPixels()

	def validate(self):
		pass
