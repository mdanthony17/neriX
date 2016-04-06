from ROOT import *
from nerixcore import *
import numpy as np
import array, math
import os

# parameters
x_pars = [30, -40., 40.]
y_pars = [30, -40., 40.]
s2w_range = [0., 1.2]

selection_radius = 20.
min_nb_events_per_bin = 10

runNb = 15

execfile('datasets.py')
#set_root_files_directory('/data2/tmp/nerix/run_' + str(runNb) + '/')
set_root_files_directory('/data2/tmp/nerix/')
set_run('run_' + str(runNb))
set_current_file(root_file)

define_aliases()

from nerixcore import _current_tree as tree

dataset = root_file[:-5]
results_dir = 'results/' + dataset

if not os.path.exists(results_dir):
	os.mkdir(results_dir)

# cuts
#Xdt0 = TCut('(-dt_us > 2) && (-dt_us < 15)')
Xdt0 = TCut('(-dt_us < 10)')
Xrp0 = TCut('(rp0_mm < 30)')
Xs1range0 = TCut('(s1stot[0] > 1000) && (s1stot[0] < 2000)')
#Xs2peak0 = TCut('(Alt$(s2speak[1],100000)-s2speak[0] > 200.)')
Xs2peak0 = TCut('nbs2peaks == 1')
Xs2stot = TCut('S2sTot[0] > 1000')

basic_cuts = TCut('')
#basic_cuts += Xdt0
basic_cuts += Xrp0
#basic_cuts += Xs1range0
basic_cuts += Xs2peak0
basic_cuts += Xs2stot

s2wxy_hist = TH2F('s2wxy_hist', ';X [mm]; Y [mm]', *tuple(x_pars+y_pars))
s2wxy_hist.SetStats(0)

x_axis = s2wxy_hist.GetXaxis()
y_axis = s2wxy_hist.GetYaxis()

tree.Draw('s2slowwidth[0]/100.:yp0_mm:xp0_mm', basic_cuts, 'goff')

xp_vals = np.copy(np.ndarray(tree.GetSelectedRows(), dtype=np.double, buffer=tree.GetV3()))
yp_vals = np.copy(np.ndarray(tree.GetSelectedRows(), dtype=np.double, buffer=tree.GetV2()))
s2w_vals = np.copy(np.ndarray(tree.GetSelectedRows(), dtype=np.double, buffer=tree.GetV1()))

s2wm_vals = []
for x_bin in range(1, x_pars[0]+1):
    for y_bin in range(1, y_pars[0]+1):

		xp_selection = (xp_vals > x_axis.GetBinLowEdge(x_bin)) & (xp_vals < x_axis.GetBinUpEdge(x_bin))
		yp_selection = (yp_vals > y_axis.GetBinLowEdge(y_bin)) & (yp_vals < y_axis.GetBinUpEdge(y_bin))

		s2w_vals2 = s2w_vals[xp_selection & yp_selection]
		if s2w_vals2.size > min_nb_events_per_bin:
			s2w = np.median(s2w_vals2)
			s2wm_vals.append(s2w)
			s2wxy_hist.SetBinContent(x_bin, y_bin, s2w)

s2width_mean = np.array(s2wm_vals).mean()
s2width_sigma = np.array(s2wm_vals).std()

# get the x,y,z coordinates of the s2width xy distribution
half_width = 30.

x_bin, y_bin, z_bin = Long(0), Long(0), Long(0)

s2wxy_hist.GetBinXYZ(s2wxy_hist.FindBin(half_width, half_width), x_bin, y_bin, z_bin)
tr_bin = [int(x_bin), int(y_bin)]

s2wxy_hist.GetBinXYZ(s2wxy_hist.FindBin(-half_width, -half_width), x_bin, y_bin, z_bin)
bl_bin = [int(x_bin), int(y_bin)]

# create the linear fitter to fit a 2d plane to the s2width xy distribution
linear_fitter = TLinearFitter(2)
linear_fitter.SetFormula('hyp2')

for x_bin in range(bl_bin[0], tr_bin[0]+1):
	for y_bin in range(bl_bin[1], tr_bin[1]+1):
		x_val = s2wxy_hist.GetXaxis().GetBinCenter(x_bin)
		y_val = s2wxy_hist.GetYaxis().GetBinCenter(y_bin)
		s2width_val = s2wxy_hist.GetBinContent(x_bin, y_bin)
		
#        if s2wxy_hist.GetBinEntries(s2wxy_hist.GetBin(x_bin, y_bin, 0)) > 10:
		if math.sqrt(x_val**2+y_val**2) < selection_radius:
			linear_fitter.AddPoint(array.array('d', [x_val, y_val]), s2width_val)

linear_fitter.Eval()
params = TVectorD()
linear_fitter.GetParameters(params)
par0 = params[0]
par1 = params[1]
par2 = params[2]

norm = math.sqrt(par1**2+par2**2)
arrow_x = -15.*par1/norm
arrow_y = -15.*par2/norm

arrow = TArrow(-arrow_x, -arrow_y, arrow_x, arrow_y, 0.03, '|>')
arrow.SetAngle(40.)
arrow.SetLineWidth(2)

selection_circle = TEllipse(0., 0., selection_radius)
selection_circle.SetLineColor(kGray+1)
selection_circle.SetLineWidth(2)
selection_circle.SetLineStyle(2)
selection_circle.SetFillStyle(0)

min_s2width = par0 + par1*(-21.5*par1/norm) + par2*(-21.5*par2/norm)
max_s2width = par0 + par1*(21.5*par1/norm) + par2*(21.5*par2/norm)

s2width_plane_phi = math.pi + math.atan2(par2, par1)
s2width_plane_slope = (max_s2width-min_s2width)/43.

# text
s2widthm_text = TLatex(0.2, 0.8, '%0.2f #mus' % s2width_mean)
s2widthm_text.SetNDC(1)
s2widths_text = TLatex(0.2, 0.75, '%0.2f #mus' % s2width_sigma)
s2widths_text.SetNDC(1)
s2widthr_text = TLatex(0.2, 0.7, '%0.2f %%' % (100.*s2width_sigma/s2width_mean))
s2widthr_text.SetNDC(1)

s2widtha_text = TLatex(0.16, 0.84, '#phi = %0.2f' % (180./math.pi*s2width_plane_phi))
s2widtha_text.SetNDC(1)
s2widthb_text = TLatex(0.58, 0.84, '#alpha = %0.2f ns/mm' % (s2width_plane_slope*1000.))
s2widthb_text.SetNDC(1)

s2width_min_text = TLatex(0.16, 0.16, 'w_{-} = %0.3f #mus' % (min_s2width))
s2width_min_text.SetNDC(1)
s2width_max_text = TLatex(0.65, 0.16, 'w_{+} = %0.3f #mus' % (max_s2width))
s2width_max_text.SetNDC(1)

# canvas
c1 = TCanvas('c1', '', 50, 20, 550, 500)
c1.SetLeftMargin(0.112)
c1.SetRightMargin(0.112)
c1.SetTopMargin(0.05)
c1.SetBottomMargin(0.112)

# drawing canvas
c1.cd()
s2wxy_hist.Draw('colz')
arrow.Draw()
s2wxy_hist.GetZaxis().SetRangeUser(*s2w_range)
s2wxy_hist.GetXaxis().CenterTitle()
s2wxy_hist.GetYaxis().CenterTitle()
s2widtha_text.Draw()
s2widthb_text.Draw()
s2width_min_text.Draw()
s2width_max_text.Draw()
selection_circle.Draw()
c1.Update()
c1.SaveAs(results_dir+'/s2wxy.png')

raw_input("Please press enter to continue...")
