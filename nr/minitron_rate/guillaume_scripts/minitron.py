import numpy as np
from scipy import integrate
from scipy import interpolate
import sys
#sys.path.append('/home/fox/projects/research/xenon/xecube2/analysis/endf/')
import endf

target_thickness = 10.e-4 # cm
target_area = 1. # cm^2

energy_cutoff = 0.002 # MeV

reaction_q_value = 3.269 # MeV
atomic_mass_unit = 931.494028 # MeV/c^2
neutron_mass = 939.565 # MeV/c^2
deuteron_mass = 2.01410178*atomic_mass_unit
helium3_mass = 3.0160293*atomic_mass_unit

pathToData = '/Users/Matt/Desktop/Xenon/neriX/nr/minitron_rate/guillaume_scripts/data/'

reaction_neutron_energy_table = {}

_initialized = 0

def read_stopping_power_data(filename):
	"""
	"""
	stopping_power_data = np.loadtxt(filename, comments='%').transpose()

	return stopping_power_data

def initialize():
	global _initialized
	global titanium_proton_stopping_power_interpolation
	global hydrogen_proton_stopping_power_interpolation
	global dd_neutron_production_cross_section_interpolation
	global dd_neutron_production_differential_cross_section_interpolation

	titanium_proton_stopping_power_data = read_stopping_power_data(pathToData + 'dedx-p-ti.dat')
#    titanium_proton_stopping_power_interpolation = interpolate.interp1d(*titanium_proton_stopping_power_data, bounds_error=False, fill_value=titanium_proton_stopping_power_data[1][0])
	titanium_proton_stopping_power_interpolation = interpolate.interp1d(*titanium_proton_stopping_power_data)
	hydrogen_proton_stopping_power_data = read_stopping_power_data(pathToData + 'dedx-p-h.dat')
#    hydrogen_proton_stopping_power_interpolation =interpolate.interp1d(*hydrogen_proton_stopping_power_data, bounds_error=False, fill_value=hydrogen_proton_stopping_power_data[1][0])
	hydrogen_proton_stopping_power_interpolation =interpolate.interp1d(*hydrogen_proton_stopping_power_data)

	deuteron = endf.Particle('d')
	deuterium = endf.Isotope(atomic_number=1, mass_number=2)
	reaction = endf.NeutronProduction(projectile=deuteron, target=deuterium)
	dd_neutron_production_cross_section_interpolation = endf.Sigma(reaction)
	dd_neutron_production_differential_cross_section_interpolation = endf.DSigmaDThetaForProduct(reaction, endf.Particle('n'))

	_initialized = 1

def titanium_proton_stopping_power(energy):
	"""
	Attributes:
	   energy -- proton energy in MeV
	"""
	global titanium_proton_stopping_power_interpolation

	if not _initialized:
		initialize()
	
	return titanium_proton_stopping_power_interpolation(energy)

def hydrogen_proton_stopping_power(energy):
	"""
	Attributes:
	   energy -- proton energy in MeV
	"""
	global hydrogen_proton_stopping_power_interpolation

	if not _initialized:
		initialize()
	
	return hydrogen_proton_stopping_power_interpolation(energy)

def dd_neutron_production_cross_section(deuteron_energy):
	"""
	Attributes:
	   deuteron_energy -- deuteron energy in MeV
	"""
	global dd_neutron_production_cross_section_interpolation

	if not _initialized:
		initialize()
	
	return dd_neutron_production_cross_section_interpolation(1.e+6*deuteron_energy)

def dd_neutron_production_differential_cross_section(angle, deuteron_energy):
	"""
	Attributes:
	   angle  -- neutron emission angle in the laboratory frame
	   deuteron_energy -- deuteron energy in MeV
	"""
	global dd_neutron_production_differential_cross_section_interpolation

	if not _initialized:
		initialize()
	
	xi = np.sqrt(deuteron_energy*neutron_mass/(deuteron_energy*helium3_mass+2*helium3_mass*reaction_q_value))
	cos_theta_cm = np.cos(angle)*(xi*np.cos(angle)+np.sqrt(1-xi**2*np.sin(angle)**2))-xi
	domega_cm_domega = np.sqrt((1+2*xi*cos_theta_cm+xi**2)**3)/np.fabs(1+xi*cos_theta_cm)
	
	return domega_cm_domega*dd_neutron_production_differential_cross_section_interpolation(cos_theta_cm, 1.e+6*deuteron_energy)

def target_density(dti_ratio=2):
	"""
	"""
	titanium_density = 4.506 # g/cm^3
	titanium_lattice_expansion = 0.18

	density = (48.+2.*dti_ratio)/48.*titanium_density*(1.-titanium_lattice_expansion)

	return density

def target_deuterium_number_density(dti_ratio=2):
	"""
	"""
	deuterium_number_density = (dti_ratio*target_density(dti_ratio)*6.022e23)/(48.+2.*dti_ratio)

	return deuterium_number_density

def target_deuteron_stopping_power(energy, dti_ratio=2):
	"""Stopping power of deuterons in TiD_N target

	Attributes:
	   energy    -- deuteron energy in MeV
	   dti_ratio -- deuterium titanium ratio in target
	"""

	titanium_stopping_power = 48./(48.+2.*dti_ratio)*titanium_proton_stopping_power(0.5*energy)
	hydrogen_stopping_power = (2.*dti_ratio)/(48.+2.*dti_ratio)*hydrogen_proton_stopping_power(0.5*energy)

	titanium_deuteride_stopping_power = titanium_stopping_power + hydrogen_stopping_power

	return titanium_deuteride_stopping_power

def target_deuteron_range(energy, dti_ratio=2):
	"""Range of deuterons in TiD_N target

	Attributes:
	   energy    -- deuteron energy in MeV
	   dti_ratio -- deuterium titanium ratio in target
	"""
	deuteron_range = integrate.quad(lambda x: 1./target_deuteron_stopping_power(x, dti_ratio), energy_cutoff, energy, epsrel=0.01)[0]

	return deuteron_range

def deuteron_beam_flux(beam_current):
	"""
	Attributes:
	   beam_current -- deuteron beam current in uA
	"""
	electric_charge = 1.602176487e-19 # C/e^-

	deuteron_flux = beam_current*1.e-6/electric_charge/target_area

	return deuteron_flux

def neutron_yield(high_voltage, beam_current, monoatomic_fraction=0.05, diatomic_fraction=0.95, dti_ratio=2):
	"""
	Attributes:
	   high_voltage -- deuteron accelerating high voltage in MV
	   beam_current -- deuteron beam current in uA
	"""
	deuteron_energy = high_voltage

	prefactor = deuteron_beam_flux(beam_current)*target_deuterium_number_density(dti_ratio)/target_density(dti_ratio)*target_area
	integrand = lambda x: 1.e-24*dd_neutron_production_cross_section(x)/target_deuteron_stopping_power(x, dti_ratio)

	monoatomic_term = monoatomic_fraction*integrate.quad(integrand, energy_cutoff, deuteron_energy, epsrel=0.01)[0]
	diatomic_term = 2.*diatomic_fraction*integrate.quad(integrand, energy_cutoff, max(energy_cutoff, 0.5*deuteron_energy), epsrel=0.01)[0]

	total_neutron_yield = prefactor*(monoatomic_term+diatomic_term)

	return total_neutron_yield

def neutron_angular_yield(angle, high_voltage, beam_current, monoatomic_fraction=0.05, diatomic_fraction=0.95, dti_ratio=2):
	"""
	Attributes:
	   angle        -- neutron emission angle in the lab frame
	   high_voltage -- deuteron accelerating high voltage in MV
	   beam_current -- deuteron beam current in uA
	"""
	deuteron_energy = high_voltage

	prefactor = deuteron_beam_flux(beam_current)*target_deuterium_number_density(dti_ratio)/target_density(dti_ratio)*target_area
	integrand = lambda x: 1.e-24*dd_neutron_production_differential_cross_section(angle, x)/target_deuteron_stopping_power(x, dti_ratio)

	monoatomic_term = monoatomic_fraction*integrate.quad(integrand, energy_cutoff, deuteron_energy, epsrel=0.01)[0]
	diatomic_term = 2.*diatomic_fraction*integrate.quad(integrand, energy_cutoff, max(energy_cutoff, 0.5*deuteron_energy), epsrel=0.01)[0]

	total_neutron_yield = prefactor*(monoatomic_term+diatomic_term)

	return total_neutron_yield

def reaction_neutron_energy(deuteron_energy, angle):
	"""Neutron energy in the 2H(d,n)3He fusion reaction

	Attributes:
	   deuteron_energy -- deuteron energy in MeV
	   angle           -- neutron emission angle in the laboratory frame
	"""
	sqrt1 = np.sqrt(deuteron_mass*neutron_mass*deuteron_energy)*np.cos(angle)
	sqrt2 = np.sqrt(deuteron_mass*neutron_mass*deuteron_energy*(np.cos(angle)**2) + (helium3_mass+neutron_mass)*(helium3_mass*reaction_q_value + (helium3_mass-deuteron_mass)*deuteron_energy))

	neutron_energy = ((sqrt1+sqrt2)/(helium3_mass+neutron_mass))**2

	return neutron_energy

def reaction_deuteron_energy(neutron_energy, angle, branch=0):
	"""Deuteron energy in the 2H(d,n)3He fusion reaction that creates a neutron of the specified energy

	Attributes:
	   neutron_energy -- neutron energy in MeV
	   angle          -- neutron emission angle in the laboratory frame
	"""
	term1 = neutron_energy*helium3_mass*(helium3_mass-deuteron_mass+neutron_mass) + helium3_mass*(deuteron_mass-helium3_mass)*reaction_q_value
	term2 = -2*np.sqrt(neutron_energy*deuteron_mass*neutron_mass*np.cos(angle)**2*((helium3_mass-deuteron_mass)*(neutron_energy*(helium3_mass+neutron_mass) - helium3_mass*reaction_q_value)+neutron_energy*deuteron_mass*neutron_mass*np.cos(angle)**2))
	term3 = neutron_energy*deuteron_mass*neutron_mass*np.cos(2*angle)

	if branch == 1:
		term2 = -term2
	deuteron_energy = (term1+term2+term3)/(helium3_mass-deuteron_mass)**2

	return deuteron_energy

def reaction_deuteron_energy_variation(neutron_energy, angle, branch=0):
	"""Variation of the deuteron energy in the 2H(d,n)3He fusion reaction with respect to neutron energy that
	   creates a neutron of the specified energy

	Attributes:
	   neutron_energy -- neutron energy in MeV
	   angle          -- neutron emission angle in the laboratory frame
	"""
	term1 = helium3_mass*(helium3_mass-deuteron_mass+neutron_mass) + deuteron_mass*neutron_mass*np.cos(2*angle)
	term2_numerator = deuteron_mass*neutron_mass*np.cos(angle)**2*(neutron_energy*(-2*helium3_mass**2 + 2*helium3_mass*(deuteron_mass-neutron_mass) + deuteron_mass*neutron_mass) + helium3_mass*(helium3_mass-deuteron_mass)*reaction_q_value - neutron_energy*deuteron_mass*neutron_mass*np.cos(2*angle))
	term2_denominator = np.sqrt(neutron_energy*deuteron_mass*neutron_mass*np.cos(angle)**2*((helium3_mass-deuteron_mass)*(neutron_energy*(helium3_mass+neutron_mass) - helium3_mass*reaction_q_value) + neutron_energy*deuteron_mass*neutron_mass*np.cos(angle)**2))

	if branch == 1:
		term2_numerator = -term2_numerator
	deuteron_energy_variation = (term1+term2_numerator/term2_denominator)/(helium3_mass-deuteron_mass)**2

	return deuteron_energy_variation

def neutron_energy_angle_yield(angle, neutron_energy, high_voltage, beam_current, monoatomic_fraction=0.05, diatomic_fraction=0.95, dti_ratio=2):
	"""
	Attributes:
	   angle        -- neutron emission angle in the laboratory frame
	   high_voltage -- deuteron accelerating high voltage in MV
	   beam_current -- deuteron beam current in uA
	"""
	global reaction_neutron_energy_table

	if angle in reaction_neutron_energy_table:
		neutron_energy_vals = reaction_neutron_energy_table[angle]
	else:
		neutron_energy_vals = [reaction_neutron_energy(energy, angle) for energy in np.linspace(0., high_voltage, 1000.)]
		reaction_neutron_energy_table[angle] = neutron_energy_vals
	min_neutron_energy = min(neutron_energy_vals)
	max_neutron_energy = max(neutron_energy_vals)
	if neutron_energy < min_neutron_energy or neutron_energy > max_neutron_energy:
		total_neutron_yield = 0.
	else:
		# bug bug bug, find condition for which we need to sum both branches (because of the second order solution)
#        if angle > np.pi/2:
#            branches = [0, 1]
#        else:
#            branches = [0]
		branches = [0]
		total_neutron_yield = 0.
		for branch in branches:
			deuteron_energy = reaction_deuteron_energy(neutron_energy, angle, branch)
			#print deuteron_energy, energy_cutoff, high_voltage
			if np.isnan(deuteron_energy) or deuteron_energy < 2.*energy_cutoff or deuteron_energy > high_voltage:
				neutron_yield = 0
			else:
				prefactor = deuteron_beam_flux(beam_current)*target_deuterium_number_density(dti_ratio)/target_density(dti_ratio)*target_area
				energy_dependent_factor = lambda x: 1.e-24*dd_neutron_production_differential_cross_section(angle, x)/target_deuteron_stopping_power(x, dti_ratio)*reaction_deuteron_energy_variation(neutron_energy, angle, branch)

				monoatomic_term = monoatomic_fraction*energy_dependent_factor(deuteron_energy)
				diatomic_term = 2.*diatomic_fraction*energy_dependent_factor(0.5*deuteron_energy)

				neutron_yield = np.fabs(prefactor*(monoatomic_term+diatomic_term))
			total_neutron_yield += neutron_yield

	return total_neutron_yield


if __name__ == '__main__':
	print neutron_angular_yield(np.pi/2 - 0.26, .08, 65)
