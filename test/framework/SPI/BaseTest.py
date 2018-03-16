from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper
import RPi.GPIO as GPIO
import os

class SPIBaseTest(BaseTest):

	def __init__(self, descriptor, outsubdir, runner):
		BaseTest.__init__(self, descriptor, outsubdir, runner)
		
		self.BASE_DIR = os.path.join(self.project.root, '../SPI')

		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)

		os.environ["LD_LIBRARY_PATH"] = self.BASE_DIR + os.pathsep + os.environ["LD_LIBRARY_PATH"]

	def __del__(self):
		GPIO.cleanup()

	def start(self):
		self.correlator = CorrelatorHelper(self)#, name='SPICorrelator')
		self.correlator.start()
		self.correlator.injectMonitorscript(filenames=[os.path.join(self.BASE_DIR, 'SPIPlugin.mon')])

	def checkPin(self, pinID, expectedValue):
		GPIO.setup(pinID, GPIO.IN)
		self.assertTrue(expectedValue == GPIO.input(pinID))

	def peekPin(self, pinID, expectedValue):
		GPIO.Setup(pinID, GPIO.IN)
		result = false;
		for i in range(100):
			result = result or expectedValue == GPIO.input(pinID)
		selt.assertTrue(result)