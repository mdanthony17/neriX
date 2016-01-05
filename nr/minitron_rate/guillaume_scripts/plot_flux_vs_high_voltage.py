from ROOT import TCanvas, TGraphErrors, TF1, TGraph, TLatex
import numpy as np
import array
import minitron

high_voltage_range = (30., 100.)
neutron_flux_range = (0.002, 2.e+0)

"""
beam_current = 85. # uA
high_voltage_data = np.array((   40.,  50.,  60.,  70.,  80.,  90.)) # kV
neutron_counts_data = np.array((395., 352., 195., 716., 574., 910.)) # counts
time_interval_data = np.array(( 600., 240.,  60., 120.,  60.,  60.)) # s
"""

beam_current = 65. # uA
high_voltage_data = np.array((45., 50., 55., 60., 65., 70., 75.))
neutron_counts_data = np.array((36.+39.+50., 93.+98.+67., 145.+116.+130., 150.+133.+150., 195.+195.+221., 251.+247.+249., 351.+366., 410.+411.+385., 533.+538.+536.))
time_interval_data = np.array((900, 900, 900, 900, 900, 900, 600, 900, 900))


draft = 0
include_fit = 0

# minitron-snoopy distance
distance = 180 #50. # cm
angle = np.pi/2. - 0.26

# snoopy NP-2 calibration
snoopy_np2_factor = 6000. # counts/mrem

# 2.5 MeV neutron fluence to dose conversion
neutron_fluence_to_dose = 29.e+6 # neutrons/cm^2/rem

flux_conversion = 1./(1.e3*snoopy_np2_factor)*neutron_fluence_to_dose

neutron_flux_data = flux_conversion*neutron_counts_data/time_interval_data/beam_current
neutron_flux_error_data = flux_conversion*np.sqrt(neutron_counts_data)/time_interval_data/beam_current

neutron_flux_graph = TGraphErrors(len(high_voltage_data), array.array('f', high_voltage_data),
array.array('f', neutron_flux_data), array.array('f', len(high_voltage_data)*[0.]),
array.array('f', neutron_flux_error_data))
neutron_flux_graph.SetMarkerStyle(21)

neutron_flux_func = TF1('neutron_flux_func', 'pol3', high_voltage_range[0], high_voltage_range[1])
neutron_flux_func.SetLineWidth(2)
neutron_flux_func.SetLineStyle(4)

if include_fit:
	neutron_flux_graph.Fit(neutron_flux_func, 'q0')

high_voltage_vals = np.linspace(high_voltage_range[0], high_voltage_range[1], 200)
neutron_flux_vals = [minitron.neutron_angular_yield(angle, high_voltage/1000., 1.)/distance**2 for high_voltage in high_voltage_vals]

expected_neutron_flux_graph = TGraph(len(high_voltage_vals), array.array('f', high_voltage_vals), array.array('f', neutron_flux_vals))
expected_neutron_flux_graph.SetLineWidth(2)
expected_neutron_flux_graph.SetLineStyle(1)

minitron_beam_current_text = TLatex(0.2, 0.8, 'Beam Current: %d #muA' % beam_current)
minitron_beam_current_text.SetTextSize(0.05)
minitron_beam_current_text.SetNDC()

minitron_distance_text = TLatex(0.2, 0.72, 'Distance: %d cm' % distance)
minitron_distance_text.SetTextSize(0.05)
minitron_distance_text.SetNDC()

# canvas
c1 = TCanvas('c1', '', 50, 25, 800, 600)
c1.SetLogy()

c1.cd()
neutron_flux_frame = c1.DrawFrame(high_voltage_range[0], neutron_flux_range[0], high_voltage_range[1], neutron_flux_range[1])
if draft:
	neutron_flux_frame.SetTitle('Minitron Neutron Flux;High Voltage [kV];Neutron Flux [n cm^{-2} s^{-1} #muA^{-1}]')
else:
	neutron_flux_frame.SetTitle(';High Voltage [kV];Neutron Flux [n cm^{-2} s^{-1} #muA^{-1}]')
neutron_flux_frame.GetXaxis().CenterTitle()
neutron_flux_frame.GetYaxis().CenterTitle()
neutron_flux_graph.Draw('p')
if include_fit:
	neutron_flux_func.Draw('same')
expected_neutron_flux_graph.Draw('l same')
minitron_beam_current_text.Draw('same')
minitron_distance_text.Draw('same')
c1.Update()

c1.SaveAs('minitron-flux_vs_high_voltage.eps')

