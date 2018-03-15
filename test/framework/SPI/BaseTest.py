from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper
import RPi.SPI as SPI
import os

class SPIBaseTest(BaseTest):

	def __init__(self, descriptor, outsubdir, runner):
		BaseTest.__init__(self, descriptor, outsubdir, runner)
		
		self.BASE_DIR = os.path.join(self.project.root, '..')

		SPI.setmode(SPI.BCM)
		SPI.setwarnings(False)

		os.environ["LD_LIBRARY_PATH"] = self.BASE_DIR + os.pathsep + os.environ["LD_LIBRARY_PATH"]

	def __del__(self):
		SPI.cleanup()

	def start(self):
		self.correlator = CorrelatorHelper(self)#, name='SPICorrelator')
		self.correlator.start()
		self.correlator.injectMonitorscript(filenames=[os.path.join(self.BASE_DIR, 'SPIPlugin.mon')])