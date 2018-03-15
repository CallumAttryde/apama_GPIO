from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper
from sense_hat import SenseHat
import os
import math

class SenseHATBaseTest(BaseTest):

	def __init__(self, descriptor, outsubdir, runner):
		BaseTest.__init__(self, descriptor, outsubdir, runner)
		self.BASE_DIR = os.path.join(self.project.root, '..')
		os.environ["LD_LIBRARY_PATH"] = self.BASE_DIR + os.pathsep + os.environ["LD_LIBRARY_PATH"]
		self.sense = SenseHat()

	def start(self):
		self.correlator = CorrelatorHelper(self)
		self.correlator.start()

	def clearPixels(self):
		self.sense.clear()

	def getPixel(self, x, y):
		return self.sense.get_pixel(x, y)

	# 3 bits lost for red, 2 for green and 3 for blue
	def checkPixel(self, x, y, values):
		values[0] = int(8*math.floor(values[0] / 8))
		values[1] = int(4*math.floor(values[1] / 4))
		values[2] = int(8*math.floor(values[2] / 8))
		self.assertTrue(self.getPixel(x, y) == values)

	def getTemperature(self):
		return self.sense.get_temperature()

	def getHumidity(self):
		return self.sense.get_humidity()

	def getPressure(self):
		return self.sense.get_pressure()
