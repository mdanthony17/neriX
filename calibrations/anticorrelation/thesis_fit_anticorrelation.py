#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys, os

import matplotlib
matplotlib.use('QT4Agg')

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
matplotlib.rcParams['font.size'] = 16

import matplotlib.pyplot as plt

import pandas
import root_pandas

from rootpy.plotting import root2matplotlib as rplt

import emcee, corner, click
import neriX_analysis, neriX_datasets
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
import numpy as np
import tqdm, time, copy_reg, types, pickle
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.stats
import scipy.special
import scipy.misc

import gc
gc.disable()

def neg_ln_likelihood_2d_gaussian(a_parameters, x):
    mean_x, mean_y, var_x, var_y, cov_xy = a_parameters
    ln_likelihood = -np.sum(scipy.stats.multivariate_normal.logpdf(x.T, mean=[mean_x, mean_y], cov=[[var_x, cov_xy], [cov_xy, var_y]]))
    if np.isfinite(ln_likelihood):
        return ln_likelihood
    else:
        return -np.inf

def neg_ln_likelihood_1d_gaussian(a_parameters, x):
    mean_x, stdev_x = a_parameters
    #print a_parameters
    ln_likelihood = -np.sum(scipy.stats.norm.logpdf(x, loc=mean_x, scale=stdev_x))
    if np.isfinite(ln_likelihood):
        return ln_likelihood
    else:
        return -np.inf

def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))

class fit_anticorrelation():


    def __init__(self, d_files):


        l_acceptable_energies = ['cs137', 'na22']
        self.s_dict_filename = 'sampler_dictionary.p'
        self.s_base_save_name = 'ac_'

        # will have to change the setting based on the source otherwise
        # neriX_analysis won't accept the files
        # will do this within the loop
        l_settings = [[-1,0.345,4.5], [-1,0.7,4.5], [-1,1.054,4.5], [-1,1.500,4.5], [-1,2.356,4.5]]
        
        # make class methods pickleable for multithreading process
        copy_reg.pickle(types.MethodType, reduce_method)

        # check that files given are of a supported energy

        for source in d_files:
            if source in l_acceptable_energies:
                continue
            else:
                print 'File of type %s not supported yet!' % (source)
                sys.exit()

        correction_factor = self.get_correction_factor()
        #print correction_factor
        

        s1_par = 'cpS1sTotBottom[0]/%f' % correction_factor
        s2_par = 'cpS2sTotBottom[0]/%f' % correction_factor

        #s1_par = 'cpS1sTotBottom[0]'
        #s2_par = 'cpS2sTotBottom[0]'

        column_names = ('S1', 'S2', 'x', 'y', 'z')

        bounds_width_inclusion = 2.5
        orth_bins, orth_min, orth_max = 100, -500, 600


        # use dictionary to store all arrays, histograms,
        # fits, etc.

        self.d_energy_information = {}
        self.d_source_functions = {}
        self.d_s1_s2_arrays = {}

        self.base_save_name = 'ac_'
        
        self.d_files = d_files
        
        # save the yield information for the source
        d_yield_information = {}

        for dummy_index, source in enumerate(d_files):
            self.base_save_name += '%s_' % source

            # create entry in dictionary
            self.d_energy_information[source] = {}
            


            if source == 'cs137':
                self.d_energy_information[source]['num_bins'] = 250
                self.d_energy_information[source]['ub_ces'] = 1200
            elif source == 'na22':
                self.d_energy_information[source]['num_bins'] = 250
                self.d_energy_information[source]['ub_ces'] = 800
            elif source == 'bkg':
                self.d_energy_information[source]['num_bins'] = 50
                self.d_energy_information[source]['ub_ces'] = 450
            elif source == 'co57':
                self.d_energy_information[source]['num_bins'] = 50
                self.d_energy_information[source]['ub_ces'] = 250


            a_combined_tree = -1

            column_dtype = -1

            l_columns = [np.empty(0) for i in xrange(len(column_names))]

            l_files = d_files[source]

            # must change l_settings based on current source
            if source == 'cs137':
                correct_degree_setting = -1
                self.d_energy_information[source]['peaks'] = 662.
                self.d_energy_information[source]['color'] = root.kBlue
            elif source == 'na22':
                correct_degree_setting = -5
                self.d_energy_information[source]['peaks'] = 511.
                self.d_energy_information[source]['color'] = root.kRed
            elif source == 'bkg':
                correct_degree_setting = -6
                self.d_energy_information[source]['peaks'] = 164.
                self.d_energy_information[source]['color'] = root.kGreen+1
            elif source == 'co57':
                correct_degree_setting = -2
                self.d_energy_information[source]['peaks'] = 122.
                self.d_energy_information[source]['color'] = root.kMagenta+1

            for i in xrange(len(l_settings)):
                l_settings[i][0] = correct_degree_setting



            # transpose list of files such that
            # files of the same cathode voltage are together
            l_files = map(list, zip(*l_files))


            for cur_index, files_at_setting in enumerate(l_files):
                # if fitting a full run, "file" is actually a list of
                # files for a given cathode setting
                # name is from older version

                current_analysis = neriX_analysis.neriX_analysis(list(files_at_setting), *l_settings[cur_index])

                current_analysis.add_z_cut()
                #current_analysis.add_z_cut(0, -12) # top only
                current_analysis.add_single_scatter_cut()
                current_analysis.add_radius_cut(0, 0.85)
                current_analysis.add_cut('(s1asym > 0)')

                run_number = current_analysis.get_run()

                current_analysis.set_event_list()
                
                """
                c_test = Canvas()
                h_test = Hist2D(40, 0, 5000, 40, 0, 8e5)
                current_analysis.Draw('%s:%s' % (s1_par, s2_par), hist=h_test)
                h_test.Draw('colz')
                c_test.Update()
                raw_input()
                """
                

                if cur_index == 0:
                    if len(files_at_setting) == 1:
                        self.s_directory_save_name = './results/run_%d/%s/' % (run_number, files_at_setting[0][:12])
                    else:
                        self.s_directory_save_name = './results/run_%d_combination/' % (run_number)
            
                    if not os.path.exists(self.s_directory_save_name):
                        os.makedirs(self.s_directory_save_name)
            
                    self.d_energy_information[source]['d_arrays_difft_voltages'] = {}


                for j in xrange(len(list(files_at_setting))):
                    #print s1_par, s2_par
                    current_tree = tree2array(current_analysis.get_T1(j), [s1_par, s2_par, 'X[0]', 'Y[0]', 'Z'], selection=current_analysis.get_cuts())
                    #print current_tree.dtype.names
                    current_tree.dtype.names = column_names
                    
                    # add to dictionary of trees to look at individual spectra
                    self.d_energy_information[source]['d_arrays_difft_voltages'][current_analysis.get_cathode_setting()] = current_tree
                    
                    #print current_tree.dtype.names
                    column_dtype = current_tree.dtype
                    for i, column_name in enumerate(current_tree.dtype.names):
                        l_columns[i] = np.append(l_columns[i], current_tree[column_name])

                del current_analysis

            a_combined_tree = np.empty(len(l_columns[0]), dtype=column_dtype)
            for i, column_name in enumerate(column_names):
                a_combined_tree[column_name] = l_columns[i]

            self.d_energy_information[source]['combined_tree'] = array2tree(a_combined_tree, name='combined_tree_%s' % (source))


            # ---------- MAKE CES CUT ----------

            # no time correction: 0.08, 14.
            print 'Corrected for new gain (3/17/17)'
            g1_test = 0.121#0.118/9.35*8.68 #0.1308 #0.12
            g2_test = 22.9*0.88#0.9*21.29/9.35*8.68 #28.

            self.d_energy_information[source]['canvas'] = Canvas(width=900, height=700, name='cCES')
            self.d_energy_information[source]['canvas'].SetGridx()
            self.d_energy_information[source]['canvas'].SetGridy()

            self.d_energy_information[source]['hist'] = Hist(self.d_energy_information[source]['num_bins'], 0, self.d_energy_information[source]['ub_ces'], name='hCES_%s' % (source), title='hCES_%s' % (source), drawstyle='hist')
            self.d_energy_information[source]['combined_tree'].Draw('0.0137*(S1/%f + S2/%f)>>hCES_%s' % (g1_test, g2_test, source))

            self.d_energy_information[source]['hist'].Draw()
            self.d_energy_information[source]['canvas'].Update()

            print self.d_energy_information[source]['hist']

            #self.d_energy_information[source]['lbCES'] = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
            #self.d_energy_information[source]['ubCES'] = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))

            #print '\n\nDEBUG MODE SKIPPING LIMIT PICKS\n\n'
            #self.d_energy_information[source]['lbCES'] = 400
            #self.d_energy_information[source]['ubCES'] = 700

            if source == 'cs137':

                self.d_energy_information[source]['lbCES'] = 550
                self.d_energy_information[source]['ubCES'] = 800

                self.d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2])/(exp(-%f/[2])-exp(-%f/[2])) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )' % (self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES']), self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParameters(1000, 0.5, 150, 500, 662, 50)
                self.d_energy_information[source]['func'].SetParLimits(0, 1, 1e8)
                self.d_energy_information[source]['func'].SetParLimits(1, 0, 1)
                self.d_energy_information[source]['func'].SetParLimits(2, 50, 1000)
                self.d_energy_information[source]['func'].SetParLimits(3, 600, 700)
                #self.d_energy_information[source]['func'].SetParLimits(3, self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParLimits(4, 10, 100)


            elif source == 'na22':
            
                self.d_energy_information[source]['lbCES'] = 420
                self.d_energy_information[source]['ubCES'] = 620

                self.d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2])/(exp(-%f/[2])-exp(-%f/[2])) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )' % (self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES']), self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParameters(1000, 0.7, 170, 511, 20)
                self.d_energy_information[source]['func'].SetParLimits(0, 1, 1e8)
                self.d_energy_information[source]['func'].SetParLimits(1, 0, 1)
                self.d_energy_information[source]['func'].SetParLimits(2, 10, 500)
                self.d_energy_information[source]['func'].SetParLimits(3, 450, 560)
                #self.d_energy_information[source]['func'].SetParLimits(3, self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParLimits(4, 10, 100)


            elif source == 'bkg':

                self.d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1] + (1.-[1])/(2*3.141592)^0.5/[3]*exp(-0.5*pow((x-[2])/[3], 2)) )', self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParameters(1000, 0.5, 164, 50)
                self.d_energy_information[source]['func'].SetParLimits(0, 1, 1e8)
                self.d_energy_information[source]['func'].SetParLimits(1, 0, 1)
                self.d_energy_information[source]['func'].SetParLimits(2, self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParLimits(3, 1, 50)

            elif source == 'co57':

                self.d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1] + (1.-[1])/(2*3.141592)^0.5/[3]*exp(-0.5*pow((x-[2])/[3], 2)) )', self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParameters(1000, 0.5, 164, 50)
                self.d_energy_information[source]['func'].SetParLimits(0, 1, 1e8)
                self.d_energy_information[source]['func'].SetParLimits(1, 0, 1)
                self.d_energy_information[source]['func'].SetParLimits(2, self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES'])
                self.d_energy_information[source]['func'].SetParLimits(3, 1, 50)



            self.d_energy_information[source]['hist'].Fit(self.d_energy_information[source]['func'], 'LR')
            self.d_energy_information[source]['func'].Draw('same')
            self.d_energy_information[source]['canvas'].Update()



            #raw_input('\nFit okay?\n')


            # set lower and upper bounds such that guassian is mostly included
            # but minimal bkg is included
            self.d_energy_information[source]['lbCES_gaus_only'] = self.d_energy_information[source]['func'].GetParameter(3) - bounds_width_inclusion*self.d_energy_information[source]['func'].GetParameter(4)
            self.d_energy_information[source]['ubCES_gaus_only'] = self.d_energy_information[source]['func'].GetParameter(3) + bounds_width_inclusion*self.d_energy_information[source]['func'].GetParameter(4)

            #print self.d_energy_information[source]['lbCES'], self.d_energy_information[source]['ubCES']

            trial_width = 2.

            # will fit background with gaussian
            # background can be either exponential or constant
            # exponential: Cs137, Na22
            # constant: Co57, bkg peaks
            sCES_arr = '(0.0137*(S1/%f + S2/%f)) > %f && (0.0137*(S1/%f + S2/%f)) < %f' % (g1_test, g2_test, self.d_energy_information[source]['lbCES_gaus_only'], g1_test, g2_test, self.d_energy_information[source]['ubCES_gaus_only'])
            
            
            
            
            # now deal with axis orthogonal to energy to deal
            # with S1 scatters in dead volume
            
            self.d_energy_information[source]['c_orth'] = Canvas(width=900, height=700, name='cCES')

            self.d_energy_information[source]['h_orth'] = Hist(100, -500, 600, name='h_orth_%s' % (source), title='h_orth_%s' % (source), drawstyle='hist')
            self.d_energy_information[source]['combined_tree'].Draw('0.0137*(-S1/%f + S2/%f)>>h_orth_%s' % (g1_test, g2_test, source), sCES_arr, '')
            
            self.d_energy_information[source]['f_orth_trial'] = root.TF1('f_gaus_trial_%s' % (source), 'gaus', 50, 400)
            self.d_energy_information[source]['h_orth'].Fit('f_gaus_trial_%s' % (source))
            
            self.d_energy_information[source]['f_orth'] = root.TF1('f_gaus_%s' % (source), 'gaus', self.d_energy_information[source]['f_orth_trial'].GetParameter(1) - trial_width*self.d_energy_information[source]['f_orth_trial'].GetParameter(2), self.d_energy_information[source]['f_orth_trial'].GetParameter(1) + trial_width*self.d_energy_information[source]['f_orth_trial'].GetParameter(2))
            self.d_energy_information[source]['h_orth'].Fit('f_gaus_%s' % (source))
            
            self.d_energy_information[source]['h_orth'].Draw()
            self.d_energy_information[source]['f_orth'].Draw('same')
            self.d_energy_information[source]['c_orth'].Update()
            
            orth_lb_cut = self.d_energy_information[source]['f_orth'].GetParameter(1) - bounds_width_inclusion*self.d_energy_information[source]['f_orth'].GetParameter(2)
            orth_ub_cut = self.d_energy_information[source]['f_orth'].GetParameter(1) + bounds_width_inclusion*self.d_energy_information[source]['f_orth'].GetParameter(2)
            
            #raw_input('press enter to continue')
            
            
            
            # make ces and orthogonal figures to show cuts and save
            
            self.d_energy_information[source]['fig_ces'], self.d_energy_information[source]['ax_ces'] = plt.subplots(1)
            
            self.d_energy_information[source]['ax_ces'].hist(0.0137 * (a_combined_tree['S1']/g1_test + a_combined_tree['S2']/g2_test), bins=self.d_energy_information[source]['num_bins'], range=[0, self.d_energy_information[source]['ub_ces']])
            self.d_energy_information[source]['ax_ces'].axvline(self.d_energy_information[source]['lbCES_gaus_only'], color='r', linestyle='--')
            self.d_energy_information[source]['ax_ces'].axvline(self.d_energy_information[source]['ubCES_gaus_only'], color='r', linestyle='--')
            
            self.d_energy_information[source]['ax_ces'].set_title(r'Full Energy Cut')
            self.d_energy_information[source]['ax_ces'].set_xlabel(r'$w(\frac{S1}{g_1} + \frac{S2}{\eta g_{gas}}) [keV]$')
            self.d_energy_information[source]['ax_ces'].set_ylabel('Counts')
            
            self.d_energy_information[source]['fig_ces'].savefig(self.s_directory_save_name + self.s_base_save_name + '%s_ces_cut.png' % (source))
            
            
            
            a_ces_cut_only = tree2array(self.d_energy_information[source]['combined_tree'], ['S1', 'S2'], selection='%s' % (sCES_arr))
            self.d_energy_information[source]['fig_orth'], self.d_energy_information[source]['ax_orth'] = plt.subplots(1)
            
            self.d_energy_information[source]['ax_orth'].hist(0.0137 * (-a_ces_cut_only['S1']/g1_test + a_ces_cut_only['S2']/g2_test), bins=orth_bins, range=[orth_min, orth_max])
            self.d_energy_information[source]['ax_orth'].axvline(orth_lb_cut, color='r', linestyle='--')
            self.d_energy_information[source]['ax_orth'].axvline(orth_ub_cut, color='r', linestyle='--')
            
            self.d_energy_information[source]['ax_orth'].set_title(r'Dead Volume Scatter Removal')
            self.d_energy_information[source]['ax_orth'].set_xlabel(r'$w(-\frac{S1}{g_1} + \frac{S2}{\eta g_{gas}}) [keV]$')
            self.d_energy_information[source]['ax_orth'].set_ylabel('Counts')
            
            self.d_energy_information[source]['fig_orth'].savefig(self.s_directory_save_name + self.s_base_save_name + '%s_orth_cut.png' % (source))
            
            
            
            
            s_orth_cut = '(0.0137*(-S1/%f + S2/%f)) > %f && (0.0137*(-S1/%f + S2/%f)) < %f' % (g1_test, g2_test, orth_lb_cut, g1_test, g2_test, orth_ub_cut)


            self.d_s1_s2_arrays[source] = tree2array(self.d_energy_information[source]['combined_tree'], ['S1', 'S2', 'x', 'y', 'z'], selection='(%s) && (%s)' % (sCES_arr, s_orth_cut))
            
            fig_pos, (ax_xy, ax_r2z) = plt.subplots(1, 2, figsize=(10,5))

            print self.d_s1_s2_arrays[source]['x']**2 + self.d_s1_s2_arrays[source]['y']**2
            
            self.d_s1_s2_arrays[source] = self.d_s1_s2_arrays[source][self.d_s1_s2_arrays[source]['z'] < -4]
            ax_xy.hist2d(self.d_s1_s2_arrays[source]['x'], self.d_s1_s2_arrays[source]['y'], bins=30)
            ax_r2z.hist2d(self.d_s1_s2_arrays[source]['x']**2 + self.d_s1_s2_arrays[source]['y']**2, self.d_s1_s2_arrays[source]['z'], bins=30)
            #plt.show()
            
            s1_min = min(self.d_s1_s2_arrays[source]['S1'])
            s1_max = max(self.d_s1_s2_arrays[source]['S1'])
            s1_mean = np.mean(self.d_s1_s2_arrays[source]['S1'])
        
            s2_min = min(self.d_s1_s2_arrays[source]['S2'])
            s2_max = max(self.d_s1_s2_arrays[source]['S2'])
            s2_mean = np.mean(self.d_s1_s2_arrays[source]['S2'])
            
            # ellipse for 2d fit
            s_ellipse = '[0]*exp( -0.5 * (pow( (( (x-[1])*cos( [2] ) - (y-[3])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[1])*sin( [2] ) + (y-[3])*cos( [2] )) /([5]) , 2.)) ) '
            
            # dictionary to pickle
            d_yield_information[source] = {}
            
            
            for cathode_setting in [0.345, 1.054, 2.356]:
                d_yield_information[source][cathode_setting] = {}
                
                
                
                current_tree = array2tree(self.d_energy_information[source]['d_arrays_difft_voltages'][cathode_setting])
                
                #h_current_s1 = Hist(40, s1_min, s1_max, name='h_current_s1')
                #current_tree.Draw('S1>>h_current_s1', '(%s) && (%s)' % (sCES_arr, s_orth_cut), '')
                
                if source == 'cs137':
                    if cathode_setting == 0.345:
                        trial_s1_mean = 2500
                        trial_s2_mean = 550e3
                    elif cathode_setting == 1.054:
                        trial_s1_mean = 2200
                        trial_s2_mean = 590e3
                    else:
                        trial_s1_mean = 1850
                        trial_s2_mean = 620e3
                elif source == 'na22':
                    if cathode_setting == 0.345:
                        trial_s1_mean = 1900
                        trial_s2_mean = 410e3
                    elif cathode_setting == 1.054:
                        trial_s1_mean = 1650
                        trial_s2_mean = 440e3
                    else:
                        trial_s1_mean = 1450
                        trial_s2_mean = 460e3
                
            
            
                h_current = Hist2D(40, s1_min, s1_max, 40, s2_min, s2_max, name='h_current')
                current_tree.Draw('S2:S1>>h_current', '(%s) && (%s)' % (sCES_arr, s_orth_cut), '')
                
                f_current = root.TF2('f_current', s_ellipse, s1_min, s1_max, s2_min, s2_max)
                #print trial_s1_mean, trial_s2_mean
                #print s1_mean, s2_mean
                
                f_current.SetParameters(30, trial_s1_mean, -3e-3, trial_s2_mean, trial_s1_mean*0.1, trial_s2_mean*0.1)
                f_current.SetParLimits(1, 0.8*trial_s1_mean, 1.2*trial_s1_mean)
                f_current.SetParLimits(3, 0.8*trial_s2_mean, 1.2*trial_s2_mean)
                
                h_current.Fit('f_current', 'MELL')
                
                d_yield_information[source][cathode_setting]['s1_mean_value'] = f_current.GetParameter(1)
                d_yield_information[source][cathode_setting]['s1_mean_unc'] = f_current.GetParError(1)
                
                d_yield_information[source][cathode_setting]['s2_mean_value'] = f_current.GetParameter(3)
                d_yield_information[source][cathode_setting]['s2_mean_unc'] = f_current.GetParError(3)
                
                
                """
                c_test = Canvas()
                h_current.SetStats(0)
                h_current.Draw('colz')
                c_test.Update()
                raw_input()
                """
        
        
                
                
    
    
            if 'cs137' in d_files:
                self.d_source_functions['cs137'] = {}
                self.d_source_functions['cs137']['lbCES'] = self.d_energy_information[source]['lbCES']
                self.d_source_functions['cs137']['ubCES'] = self.d_energy_information[source]['ubCES']
                self.d_source_functions['cs137']['ln_prior'] = self.ln_prior_greater_than_zero
                self.d_source_functions['cs137']['ln_likelihood'] = self.ln_likelihood_cs137
    
            if 'na22' in d_files:
                self.d_source_functions['na22'] = {}
                self.d_source_functions['na22']['lbCES'] = self.d_energy_information[source]['lbCES']
                self.d_source_functions['na22']['ubCES'] = self.d_energy_information[source]['ubCES']
                self.d_source_functions['na22']['ln_prior'] = self.ln_prior_greater_than_zero
                self.d_source_functions['na22']['ln_likelihood'] = self.ln_likelihood_na22
                
                
        pickle.dump(d_yield_information, open('%syield_info.pkl' % (self.s_directory_save_name), 'w'))



        
    def get_correction_factor(self):
        # should eventually read from file Zach gives
        # or better yet just be a constant
        #correction_factor = 1.07
        #neriX_analysis.warning_message('Using hard coded correction: %.2e' % correction_factor)
        
        # correction factor = 1. / (gain_in_processing / (800V_gain / rel_gain_530V_800V))
        new_gain_at_voltage = 8.68e5 * 0.00953
        new_gain_at_voltage *= (1.48e6 / 8.68e5) # because cpS1/S2 has correction already
        correction_factor = 1. / (1.18e4 / new_gain_at_voltage)
        
        return correction_factor
    
    
    
    
    
    def ln_prior_greater_than_zero(self, par):
        if par > 0.:
            return 0.
        else:
            return -np.inf



    def ln_prior_between_zero_and_one(self, par):
        if 0. < par < 1.:
            return 0.
        else:
            return -np.inf



    def ln_prior_gas_gain(self, gas_gain):
        # corrected 3/17/17
        measured_value = 22.93
        measured_uncertainty = 0.55
        return np.log(1./(2*np.pi)**0.5/measured_uncertainty) - 0.5*(gas_gain-measured_value)**2/measured_uncertainty**2



    def ln_prior_w_normal(self, w_value):
        measured_value = 13.7
        measured_uncertainty = 0.2
        return np.log(1./(2*np.pi)**0.5/measured_uncertainty) - 0.5*(w_value-measured_value)**2/measured_uncertainty**2



    def ln_prior_w_uniform(w_value):
        if 8. < w_value < 19.:
            return 0.
        else:
            return -np.inf




    def ln_likelihood_cs137(self, a_s1, a_s2, g1, eta, gas_gain, w_value, s2_width_cs137):
        g2 = eta*gas_gain
        """
        a_model_s2 = -(g2/g1)*a_s1 + 662.*g2/(w_value/1000.)
        #print g1, g2, w_value, s2_width_cs137
        return np.sum(np.log(1./(2*np.pi)**0.5/s2_width_cs137) - 0.5*(a_s2-a_model_s2)**2/s2_width_cs137**2)
        """
        #a_energy = 0.0137 * (a_s1/g1 + a_s2/g2)
        a_energy = (w_value/1000.) * (a_s1/g1 + a_s2/g2)
        return np.sum(np.log(1./(2*np.pi)**0.5/s2_width_cs137) - 0.5*(a_energy-662.)**2/s2_width_cs137**2)
        

    
    def ln_likelihood_na22(self, a_s1, a_s2, g1, eta, gas_gain, w_value, s2_width_na22):
        g2 = eta*gas_gain
        """
        a_model_s2 = -(g2/g1)*a_s1 + 511.*g2/(w_value/1000.)
        #print g1, g2, w_value, s2_width_na22
        return np.sum(np.log(1./(2*np.pi)**0.5/s2_width_na22) - 0.5*(a_s2-a_model_s2)**2/s2_width_na22**2)
        """
        #a_energy = 0.0137 * (a_s1/g1 + a_s2/g2)
        a_energy = (w_value/1000.) * (a_s1/g1 + a_s2/g2)
        return np.sum(np.log(1./(2*np.pi)**0.5/s2_width_na22) - 0.5*(a_energy-511.)**2/s2_width_na22**2)
    
    
    
    
    def ln_posterior(self, a_parameters, d_s1_s2_arrays, d_source_functions):
        prev_index = 0
        cur_index = 4
        g1, eta, gas_gain, w_value = a_parameters[prev_index:cur_index]
        prev_index = cur_index


        # since keys have no particular order have to explicitly
        # give options and add the arguments in the correct order

        if 'cs137' in d_source_functions:
            # need -1 since have to feed in the estimate
            cur_index += 1
            d_source_functions['cs137']['pars'] = a_parameters[prev_index:cur_index]
            prev_index = cur_index

        if 'na22' in d_source_functions:
            # need -1 since have to feed in the estimate
            cur_index += 1
            d_source_functions['na22']['pars'] = a_parameters[prev_index:cur_index]
            prev_index = cur_index



        #print a_parameters

        tot_ln_likelihood = 0
        tot_ln_likelihood += self.ln_prior_between_zero_and_one(g1)
        tot_ln_likelihood += self.ln_prior_between_zero_and_one(eta)
        tot_ln_likelihood += self.ln_prior_gas_gain(gas_gain)
        tot_ln_likelihood += self.ln_prior_w_normal(w_value)

        if not np.isfinite(tot_ln_likelihood):
            return -np.inf

        try:
            for source in d_source_functions:
                tot_ln_likelihood += d_source_functions[source]['ln_prior'](*d_source_functions[source]['pars'])
                #print source
                #print d_source_functions[source]['pars']
                #print 'after priors'
                #print 'new iteration'
                #print tot_ln_likelihood
                #print d_source_functions[source]['ln_likelihood'](d_s1_s2_arrays[source]['S1'], d_s1_s2_arrays[source]['S2'], g1, g2, w_value, *d_source_functions[source]['pars'])
                tot_ln_likelihood += d_source_functions[source]['ln_likelihood'](d_s1_s2_arrays[source]['S1'], d_s1_s2_arrays[source]['S2'], g1, eta, gas_gain, w_value, *d_source_functions[source]['pars'])
                #print tot_ln_likelihood
            #print 'end call'
            if np.isfinite(tot_ln_likelihood):
                return tot_ln_likelihood
            else:
                return -np.inf
        except:
            return -np.inf





    def run_mcmc(self, num_walkers, num_steps, threads=1):

        l_value_guesses = [0.12, 0.90, 21.5, 13.7, 33, 25]
        l_std_guesses = [0.01, 0.01, 0.5, 0.2, 3, 3]
        l_par_names = ['g1', 'eta', 'gas_gain', 'w_value', 'cs137_stdev', 'na22_stdev']

        if not os.path.exists(self.s_directory_save_name):
            os.makedirs(self.s_directory_save_name)
        
        num_dim = len(l_value_guesses)
        
        
        loaded_prev_sampler = False
        try:
            # two things will fail potentially
            # 1. open if file doesn't exist
            # 2. posWalkers load since initialized to None

            with open(self.s_directory_save_name + self.s_dict_filename, 'r') as f_prev_sampler:

                d_sampler = pickle.load(f_prev_sampler)
                prevSampler = d_sampler[num_walkers][-1]


                # need to load in weird way bc can't pickle
                # ensembler object
                a_starting_pos = prevSampler['_chain'][:,-1,:]
                random_state = prevSampler['_random']
            loaded_prev_sampler = True
            print '\nSuccessfully loaded previous chain!\n'
        except:
            print '\nCould not load previous sampler or none existed - starting new sampler.\n'

        if not loaded_prev_sampler:

            a_starting_pos = emcee.utils.sample_ball(l_value_guesses, l_std_guesses, size=num_walkers)

            random_state = None

            # create file if it doesn't exist
            if not os.path.exists(self.s_directory_save_name + self.s_dict_filename):
                with open(self.s_directory_save_name + self.s_dict_filename, 'w') as f_prev_sampler:
                    d_sampler = {}

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
            else:
                with open(self.s_directory_save_name + self.s_dict_filename, 'r') as f_prev_sampler:
                    d_sampler = pickle.load(f_prev_sampler)
                with open(self.s_directory_save_name + self.s_dict_filename, 'w') as f_prev_sampler:

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
        
        
        
        

        #print a_starting_pos

        #sampler = emcee.EnsembleSampler(num_walkers, num_dim, ln_posterior, threads=threads)
        sampler = emcee.DESampler(num_walkers, num_dim, self.ln_posterior, threads=threads, autoscale_gamma=True, args=[self.d_s1_s2_arrays, self.d_source_functions])
        
        print '\n\nBeginning MCMC sampler\n\n'
        print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)
        start_time_mcmc = time.time()

        with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
            for pos, lnprob, state in mcmc_sampler:
                pass

        total_time_mcmc = (time.time() - start_time_mcmc) / 3600.
        print '\n\n%d function calls took %.2f hours.\n\n' % (num_walkers*num_steps, total_time_mcmc)


        dictionary_for_sampler = sampler.__dict__
        dictionary_for_sampler = {'_chain':dictionary_for_sampler['_chain'], '_random':dictionary_for_sampler['_random']}
        if 'lnprobfn' in dictionary_for_sampler:
            del dictionary_for_sampler['lnprobfn']
        if 'pool' in dictionary_for_sampler:
            del dictionary_for_sampler['pool']


        with open(self.s_directory_save_name + self.s_dict_filename, 'r') as f_prev_sampler:
            d_sampler = pickle.load(f_prev_sampler)
        #f_prev_sampler.close()

        f_prev_sampler = open(self.s_directory_save_name + self.s_dict_filename, 'w')

        d_sampler[num_walkers].append(dictionary_for_sampler)

        pickle.dump(d_sampler, f_prev_sampler)
        f_prev_sampler.close()






    def draw_anticorrelation_plots(self, num_walkers, num_steps_to_include):
        
        l_par_names = ['g1', 'eta', 'gas_gain', 'w_value', 'cs137_stdev', 'na22_stdev']
        num_dim = len(l_par_names)
        
        sPathToFile = self.s_directory_save_name + self.s_dict_filename
        
        if os.path.exists(sPathToFile):
            dSampler = pickle.load(open(sPathToFile, 'r'))
            l_chains = []
            for sampler in dSampler[num_walkers]:
                l_chains.append(sampler['_chain'])

            a_sampler = np.concatenate(l_chains, axis=1)

            print 'Successfully loaded sampler!'
        else:
            print sPathToFile
            print 'Could not find file!'
            sys.exit()
        
        a_sampler = a_sampler[:, -num_steps_to_include:, :num_dim].reshape((-1, num_dim))
        
        print 'Starting corner plot...\n'
        start_time = time.time()
        fig = corner.corner(a_sampler, labels=l_par_names, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
        print 'Corner plot took %.3f minutes.\n\n' % ((time.time()-start_time)/60.)
        
        if not os.path.exists(self.s_directory_save_name):
            os.makedirs(self.s_directory_save_name)

        fig.savefig(self.s_directory_save_name + self.s_base_save_name + 'corner.png')
        
        try:
            print emcee.autocorr.integrated_time(np.mean(a_sampler, axis=0), axis=0,
                                        low=10, high=None, step=1, c=2,
                                        fast=False)
        except:
            print 'Chain too short to find autocorrelation time!'


        distance_from_median = 34.

        g1_low, g1_median, g1_high = np.percentile(a_sampler[:, 0], [50.-distance_from_median, 50., 50.+distance_from_median])
        g1_low = g1_median - g1_low
        g1_high = g1_high - g1_median

        eta_low, eta_median, eta_high = np.percentile(a_sampler[:, 1], [50.-distance_from_median, 50., 50.+distance_from_median])
        eta_low = eta_median - eta_low
        eta_high = eta_high - eta_median
        
        gas_gain_low, gas_gain_median, gas_gain_high = np.percentile(a_sampler[:, 2], [50.-distance_from_median, 50., 50.+distance_from_median])
        gas_gain_low = gas_gain_median - gas_gain_low
        gas_gain_high = gas_gain_high - gas_gain_median

        w_low, w_median, w_high = np.percentile(a_sampler[:, 3], [50.-distance_from_median, 50., 50.+distance_from_median])
        w_low = w_median - w_low
        w_high = w_high - w_median
        
        cs_res_low, cs_res_median, cs_res_high = np.percentile(a_sampler[:, 3], [50.-distance_from_median, 50., 50.+distance_from_median])
        cs_res_low = cs_res_median - cs_res_low
        cs_res_high = cs_res_high - cs_res_median
        
        na_res_low, na_res_median, na_res_high = np.percentile(a_sampler[:, 3], [50.-distance_from_median, 50., 50.+distance_from_median])
        na_res_low = na_res_median - na_res_low
        na_res_high = na_res_high - na_res_median


        lb_s1, ub_s1 = 1000, 3500
        lb_s2, ub_s2 = 250e3, 900e3

        x_line_s1 = np.linspace(lb_s1, ub_s1)
        y_line_cs137 = (self.d_energy_information['cs137']['peaks'] * eta_median*gas_gain_median / (w_median/1000.)) - ((eta_median*gas_gain_median)/g1_median)*x_line_s1
        y_line_na22 = (self.d_energy_information['na22']['peaks'] * eta_median*gas_gain_median / (w_median/1000.)) - ((eta_median*gas_gain_median)/g1_median)*x_line_s1

        f1, ax1 = plt.subplots(1)
        ax1.hist2d(np.concatenate((self.d_s1_s2_arrays['cs137']['S1'], self.d_s1_s2_arrays['na22']['S1'])), np.concatenate((self.d_s1_s2_arrays['cs137']['S2'], self.d_s1_s2_arrays['na22']['S2'])), bins=(80, 80), range=[[lb_s1, ub_s1], [lb_s2, ub_s2]])
        ax1.plot(x_line_s1, y_line_cs137, color='r', linestyle='--')
        ax1.plot(x_line_s1, y_line_na22, color='magenta', linestyle='--')

        #ax1.set_title(r'Anticorrelation of S1 and S2')
        ax1.set_xlabel('S1 [PE]')
        ax1.set_ylabel('S2 [PE]')

        f1.savefig(self.s_directory_save_name + self.s_base_save_name + 's1_s2.png')

        d_results = {'g1':{'median':g1_median, 'p_one_sig':g1_high, 'm_one_sig':g1_low},
                     'eta':{'median':eta_median, 'p_one_sig':eta_high, 'm_one_sig':eta_low},
                     'gas_gain':{'median':gas_gain_median, 'p_one_sig':gas_gain_high, 'm_one_sig':gas_gain_low},
                     'w':{'median':w_median, 'p_one_sig':w_high, 'm_one_sig':w_low},
                     'cs_res':{'median':cs_res_median, 'p_one_sig':cs_res_high, 'm_one_sig':cs_res_low},
                     'na_res':{'median':na_res_median, 'p_one_sig':na_res_high, 'm_one_sig':na_res_low}}

        pickle.dump(d_results, open(self.s_directory_save_name + self.s_base_save_name + 'results.p', 'w'))




if __name__ == '__main__':
    d_files = {}
    
    #root.SetBatch(1)


    d_files['cs137'] = []
    #d_files['cs137'].append(['nerix_160404_1204', 'nerix_160404_1232', 'nerix_160404_1259', 'nerix_160404_1325', 'nerix_160404_1350'])
    #d_files['cs137'].append(['nerix_160627_1156', 'nerix_160627_1224', 'nerix_160627_1253', 'nerix_160627_1321', 'nerix_160627_1349'])
    d_files['cs137'].append(['nerix_160606_1155', 'nerix_160606_1225', 'nerix_160606_1254', 'nerix_160606_1347', 'nerix_160606_1421'])
    d_files['na22'] = []
    #d_files['na22'].append(['nerix_160404_1421', 'nerix_160404_1447', 'nerix_160404_1530', 'nerix_160404_1555', 'nerix_160404_1621'])
    #d_files['na22'].append(['nerix_160627_1417', 'nerix_160627_1448', 'nerix_160627_1531', 'nerix_160627_1601', 'nerix_160627_1630'])
    d_files['na22'].append(['nerix_160606_1451', 'nerix_160606_1521', 'nerix_160606_1549', 'nerix_160606_1617', 'nerix_160606_1646'])

    test = fit_anticorrelation(d_files)
    test.run_mcmc(128, 500)
    test.draw_anticorrelation_plots(128, 100)



