from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper
import RPi.GPIO as GPIO
import os

class GPIOBaseTest(BaseTest):

	def __init__(self, descriptor, outsubdir, runner):
		BaseTest.__init__(self, descriptor, outsubdir, runner)
		
		self.BASE_DIR = os.path.join(self.project.root, '..')

		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)

		os.environ["LD_LIBRARY_PATH"] = self.BASE_DIR + os.pathsep + os.environ["LD_LIBRARY_PATH"]

	def start(self):
		self.correlator = CorrelatorHelper(self)#, name='GPIOCorrelator')
		self.correlator.start()
		self.correlator.injectMonitorscript(filenames=[os.path.join(self.BASE_DIR, 'GPIOPlugin.mon')])

	def checkPin(self, pinID, expectedValue):
		GPIO.setup(pinID, GPIO.IN)
		self.assertTrue(expectedValue == GPIO.input(pinID))

	def writePin(self, pinID, value):
		GPIO.setup(pinID, GPIO.OUT)
		GPIO.output(pinID, value)
