#!/usr/bin/python

import ROOT as root
import sys, array, click
import neriX_analysis, neriX_copy_files, neriX_config
from rootpy.plotting import Hist, Hist2D, Canvas, Legend

lPossibleCalibrations = ['s1_eff', 'vd']

@click.command()
@click.option('--calibration', prompt='Choose a calibration %s.' % str(lPossibleCalibrations), type=click.Choice(['s1_eff', 'vd']))
@click.option('--overwrite', is_flag=True, expose_value=True, prompt='Would you like to overwrite finished calibrations?')
@click.option('--download', is_flag=True, expose_value=True, prompt='Download missing files?')
@click.option('--run', prompt='What run?', type=int)
@click.option('--anode', prompt='What anode setting?', type=float)
@click.option('--cathode', prompt='What cathode setting?', type=float)
@click.option('--degree', prompt='What degree setting?', type=int)
def run_cal(calibration, overwrite, download, run, anode, cathode, degree):
	print '\n\nhello\n\n'
	print calibration, overwrite, download, run, anode, cathode, degree

if __name__ == '__main__':
	run_cal()
