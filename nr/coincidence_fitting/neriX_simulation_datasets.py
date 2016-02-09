run_files = {}

pathToSimulationFiles = './simulated_data/'
pathToReducedSimulationFiles = './reduced_simulation_data/'
pathToData = '../analysis/full_angle_files/'
pathToEfficiencyFiles = 'efficiency_files/'

pathToFakeData = './fake_data/'

default_g1 = 0.095
default_g1_uncertainty = 0.005

default_g2 = 22.5
default_g2_uncertainty = 1.2

default_gas_gain = 31.0
default_gas_gain_uncertainty = 1.0

default_gas_gain_width = 10.0
default_gas_gain_width_uncertainty = 1.5

default_spe_res = 0.7
default_spe_res_uncertainty = 0.2

energyMin = -0.5#-0.05 #-0.5
energyMax = 39.5#39.95 #39.5
energyNumBins = 40#40*10 #40

reducedEnergyMin = -0.125
reducedEnergyMax = 9.875
reducedEnergyNumBins = 40

num_random_trials = int(1e4)

max_photon_yield = 15.
max_charge_yield = 15.

smear_pe_extra_bins = 10
convert_pe_extra_bins = 15

# set runs to use here
runsInUse = [15]

for run in runsInUse:
	run_files[run] = {}

dAngleEnergy = {45.:10.7,
				30.:5.0}

dVoltageField = {0.345:200.,
				 1.054:500.,
				 2.356:1000.}

# files in set should be in the following order
# data_root_file, mc_root_file, tac_efficiency_root_file, peak_finder_efficiency_root_file, g1_root_file, spe_root_file

run_files[(15, 4.5, 1.054, 30.)] = ('results_1.054kV_30deg', 'nerixsim-5keV', 's1_disc_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None)
run_files[(15, 4.5, 1.054, 45.)] = ('results_1.054kV_45deg', 'nerixsim-10keV', 's1_disc_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None)




