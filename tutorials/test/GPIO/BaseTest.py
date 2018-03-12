from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper
import os

class GPIOBaseTest(BaseTest):

	def __init__(self, descriptor, outsubdir, runner):
		BaseTest.__init__(self, descriptor, outsubdir, runner)
		
		self.BASE_DIR = os.path.join(self.project.root, '..')
		
		os.environ["LD_LIBRARY_PATH"] = self.BASE_DIR + os.pathsep + os.environ["LD_LIBRARY_PATH"]

	def start(self):
		# TODO: Path to GPIOPlugin
		self.correlator = CorrelatorHelper(self)#, name='GPIOCorrelator')
		self.correlator.start()

		self.correlator.injectMonitorscript(filenames=[os.path.join(self.BASE_DIR, 'GPIOPlugin.mon')])