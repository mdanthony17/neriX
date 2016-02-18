import os.path

run_files = {}

pathToThisModule = os.path.dirname(__file__)

pathToSimulationFiles = pathToThisModule + '/simulated_data/'
pathToReducedSimulationFiles = pathToThisModule + '/reduced_simulation_data/'
pathToData = pathToThisModule + '/../analysis/full_angle_files/'
pathToEfficiencyFiles = pathToThisModule + '/efficiency_files/'

pathToFakeData = pathToThisModule + '/mcmc_analysis/fake_data/'

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

dAngleEnergy = {62.:17.83,
				45.:10.62,
				35.:6.60,
				30.:4.95}

dVoltageField = {0.345:200.,
				 1.054:500.,
				 2.356:1000.}

# files in set should be in the following order
# data_root_file, mc_root_file, tac_efficiency_root_file, peak_finder_efficiency_root_file, g1_root_file, spe_root_file


# 30 deg
run_files[(15, 4.5, 0.345, 30.)] = ('results_0.345kV_30deg', 'nerixsim-5keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 1.054, 30.)] = ('results_1.054kV_30deg', 'nerixsim-5keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 2.356, 30.)] = ('results_2.356kV_30deg', 'nerixsim-5keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)


# 35 deg
run_files[(15, 4.5, 0.345, 35.)] = ('results_0.345kV_35deg', 'nerixsim-7keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 1.054, 35.)] = ('results_1.054kV_35deg', 'nerixsim-7keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 2.356, 35.)] = ('results_2.356kV_35deg', 'nerixsim-7keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)


# 45 deg
run_files[(15, 4.5, 0.345, 45.)] = ('results_0.345kV_45deg', 'nerixsim-10keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 1.054, 45.)] = ('results_1.054kV_45deg', 'nerixsim-10keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 2.356, 45.)] = ('results_2.356kV_45deg', 'nerixsim-10keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)


# 62 deg
run_files[(15, 4.5, 0.345, 62.)] = ('results_0.345kV_62deg', 'nerixsim-20keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 1.054, 62.)] = ('results_1.054kV_62deg', 'nerixsim-20keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)
run_files[(15, 4.5, 2.356, 62.)] = ('results_2.356kV_62deg', 'nerixsim-20keV', 'nerixsim-pure_nr_spec', 'tof_efficiency', 'PeakFindEfficiency', 'TrigEfficiency', None, None, None, None, True)




