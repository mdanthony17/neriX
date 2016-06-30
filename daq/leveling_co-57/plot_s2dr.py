from ROOT import *
from nerixcore import *
import numpy as np
import os

# parameters
x_pars = [40, -40., 40.]
y_pars = [40, -40., 40.]
s2w_range = [0., 3]

execfile('datasets.py')
set_root_files_directory('/home/mweber/nerix/data/')
set_run('run12')
set_current_file(root_file)

define_aliases()

from nerixcore import _current_tree as tree

dataset = root_file[:-5]
results_dir = 'results/' + dataset

if not os.path.exists(results_dir):
	os.mkdir(results_dir)

# cuts
Xdt0 = TCut('(-dt_us > 15) && (-dt_us < 10)')
Xs1range0 = TCut('(s1stot[0] < 2000)')
Xs2range0 = TCut('(s2stot[1] > 5000)')
Xs2peak0 = TCut('nbs2peaks == 2')

basic_cuts = TCut('')
#basic_cuts += Xdt0
basic_cuts += Xs1range0
basic_cuts += Xs2range0
basic_cuts += Xs2peak0

tree.Draw('xp1_mm:yp1_mm:xp0_mm:yp0_mm', basic_cuts, 'goff')

xp1_vals = np.copy(np.ndarray(tree.GetSelectedRows(), dtype=np.double, buffer=tree.GetV1()))
yp1_vals = np.copy(np.ndarray(tree.GetSelectedRows(), dtype=np.double, buffer=tree.GetV2()))
xp0_vals = np.copy(np.ndarray(tree.GetSelectedRows(), dtype=np.double, buffer=tree.GetV3()))
yp0_vals = np.copy(np.ndarray(tree.GetSelectedRows(), dtype=np.double, buffer=tree.GetV4()))

s2dr_lines = []
s2dr_markers = []

#for i in range(0, xp0_vals.size):
for i in range(0, tree.GetSelectedRows()-1):
	line = TLine(xp0_vals[i], yp0_vals[i], xp1_vals[i], yp1_vals[i])
	s2dr_lines.append(line)

	marker = TMarker(xp0_vals[i], yp0_vals[i], 20)
	s2dr_markers.append(marker)

for s2dr_line in s2dr_lines:
	s2dr_line.SetLineWidth(1)
	s2dr_line.SetLineColor(kRed)

for s2dr_marker in s2dr_markers:
	s2dr_marker.SetMarkerColor(kRed)
	s2dr_marker.SetMarkerSize(0.6)

# canvas
c1 = TCanvas('c1', '', 50, 20, 550, 500)
c1.SetLeftMargin(0.112)
c1.SetRightMargin(0.112)
c1.SetTopMargin(0.05)
c1.SetBottomMargin(0.112)

c1.cd()
s2dr_frame = c1.DrawFrame(x_pars[1], y_pars[1], x_pars[2], y_pars[2])
s2dr_frame.SetTitle(';X [mm];Y [mm]')
s2dr_frame.GetXaxis().CenterTitle()
s2dr_frame.GetYaxis().CenterTitle()
s2dr_frame.GetYaxis().SetDecimals()
s2dr_frame.GetXaxis().SetTitleSize(0.05)
s2dr_frame.GetYaxis().SetTitleSize(0.05)
s2dr_frame.GetXaxis().SetTitleOffset(1.05)
s2dr_frame.GetYaxis().SetTitleOffset(1.15)
s2dr_frame.GetXaxis().SetLabelSize(0.045)
s2dr_frame.GetYaxis().SetLabelSize(0.045)
for s2dr_line in s2dr_lines:
	s2dr_line.Draw('same')
for s2dr_marker in s2dr_markers:
	s2dr_marker.Draw('same')
c1.Update()

c1.SaveAs(results_dir+'/s2dr.png')
