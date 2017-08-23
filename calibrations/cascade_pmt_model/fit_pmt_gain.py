#!/usr/bin/python
#import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys, os

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt

import emcee, corner, click
import neriX_analysis, neriX_datasets, neriX_config
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
from rootpy.io import File
from rootpy import stl
import numpy as np
import tqdm, time, copy_reg, types, pickle
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
from scipy.stats import norm, poisson
from scipy.special import erf
from math import floor

import astroML.density_estimation

import cuda_pmt_mc
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray
import pycuda.autoinit

gpu_cascade_model = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('cascade_pmt_model')
gpu_pure_cascade_spectrum = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('pure_cascade_spectrum')
gpu_fixed_pe_cascade_spectrum = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('fixed_pe_cascade_spectrum')
setup_kernel = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('setup_kernel')



def weighted_avg_and_std(values, weights):
    """
    Return the weighted average and standard deviation.

    values, weights -- Numpy ndarrays with the same shape.
    """
    average = np.average(values, weights=weights)
    variance = np.average((values-average)**2, weights=weights)  # Fast and numerically precise
    return (average, (variance)**0.5)


def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))



def poisson_binned_likelihood(a_model, a_data):
    #print a_data*np.log(a_model) - a_model
    return np.sum(a_data*np.log(a_model) - a_model - a_data*np.log(a_data) + a_data)



class fit_pmt_gain(object):
    def __init__(self, d_fit_filenames, run=16, channel_number=17, num_mc_events=1e6, b_making_comparison_plots=False):
    
        # d_fit_files should be of the form
        # d_fit_files['mpe1'] = <bkg file name>
        # d_fit_files['mpe2'] = <spe file name>

        self.run = run

        # make class methods pickleable for multithreading process
        copy_reg.pickle(types.MethodType, reduce_method)

        self.channel_number = channel_number
        self.parameter_to_examine = 'SingleIntegral[%d]' % (self.channel_number - 1)
        
        
        self.num_mc_events = int(num_mc_events)
        self.d_gpu_scale = {}
        block_dim = 1024
        self.d_gpu_scale['block'] = (block_dim,1,1)
        numBlocks = floor(num_mc_events / float(block_dim))
        self.d_gpu_scale['grid'] = (int(numBlocks), 1)
        self.num_mc_events = int(numBlocks*block_dim)
        
        
        self.b_making_comparison_plots = b_making_comparison_plots
        
        
        seed = int(time.time())
        self.rng_states = drv.mem_alloc(self.num_mc_events*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
        setup_kernel(np.int32(self.num_mc_events), self.rng_states, np.uint64(seed), np.uint64(0), **self.d_gpu_scale)
        print 'Cuda random states setup...\n'
        

        self.d_fit_filenames = d_fit_filenames
        self.d_fit_files = {}
        self.d_fit_files['mpe1'] = {}
        self.d_fit_files['mpe2'] = {}
        self.d_fit_files['mpe1']['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_filenames['mpe1'] + '.root')
        self.d_fit_files['mpe2']['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_filenames['mpe2'] + '.root')

        self.d_fit_files['mpe1']['tree'] = self.d_fit_files['mpe1']['file'].T0
        self.d_fit_files['mpe2']['tree'] = self.d_fit_files['mpe2']['file'].T0

        self.d_fit_files['mpe1']['settings'] = [50, -6e5, 3e6]
        self.d_fit_files['mpe2']['settings'] = [50, -6e5, 4e6]
        
        self.d_fit_files['mpe1']['bin_edges'] = np.linspace(self.d_fit_files['mpe1']['settings'][1], self.d_fit_files['mpe1']['settings'][2], self.d_fit_files['mpe1']['settings'][0]+1) # need +1 for bin edges
        self.d_fit_files['mpe1']['bin_width'] = self.d_fit_files['mpe1']['bin_edges'][1] - self.d_fit_files['mpe1']['bin_edges'][0]
        self.d_fit_files['mpe1']['bin_centers'] = np.linspace(self.d_fit_files['mpe1']['settings'][1]+self.d_fit_files['mpe1']['bin_width']/2., self.d_fit_files['mpe1']['settings'][2]-self.d_fit_files['mpe1']['bin_width']/2., self.d_fit_files['mpe1']['settings'][0])
        if self.b_making_comparison_plots:
            num_bins_plots = 5*self.d_fit_files['mpe1']['settings'][0]
            self.d_fit_files['mpe1']['bin_edges_plots'] = np.linspace(self.d_fit_files['mpe1']['settings'][1], self.d_fit_files['mpe1']['settings'][2], num_bins_plots+1) # need +1 for bin edges
            self.d_fit_files['mpe1']['bin_width_plots'] = self.d_fit_files['mpe1']['bin_edges_plots'][1] - self.d_fit_files['mpe1']['bin_edges_plots'][0]
            self.d_fit_files['mpe1']['bin_centers_plots'] = np.linspace(self.d_fit_files['mpe1']['settings'][1]+self.d_fit_files['mpe1']['bin_width_plots']/2., self.d_fit_files['mpe1']['settings'][2]-self.d_fit_files['mpe1']['bin_width_plots']/2., num_bins_plots)
        
        
        self.d_fit_files['mpe2']['bin_edges'] = np.linspace(self.d_fit_files['mpe2']['settings'][1], self.d_fit_files['mpe2']['settings'][2], self.d_fit_files['mpe2']['settings'][0]+1) # need +1 for bin edges
        self.d_fit_files['mpe2']['bin_width'] = self.d_fit_files['mpe2']['bin_edges'][1] - self.d_fit_files['mpe2']['bin_edges'][0]
        self.d_fit_files['mpe2']['bin_centers'] = np.linspace(self.d_fit_files['mpe2']['settings'][1]+self.d_fit_files['mpe2']['bin_width']/2., self.d_fit_files['mpe2']['settings'][2]-self.d_fit_files['mpe2']['bin_width']/2., self.d_fit_files['mpe2']['settings'][0])
        if self.b_making_comparison_plots:
            num_bins_plots = 5*self.d_fit_files['mpe2']['settings'][0]
            self.d_fit_files['mpe2']['bin_edges_plots'] = np.linspace(self.d_fit_files['mpe2']['settings'][1], self.d_fit_files['mpe2']['settings'][2], num_bins_plots+1) # need +1 for bin edges
            self.d_fit_files['mpe2']['bin_width_plots'] = self.d_fit_files['mpe2']['bin_edges_plots'][1] - self.d_fit_files['mpe2']['bin_edges_plots'][0]
            self.d_fit_files['mpe2']['bin_centers_plots'] = np.linspace(self.d_fit_files['mpe2']['settings'][1]+self.d_fit_files['mpe2']['bin_width_plots']/2., self.d_fit_files['mpe2']['settings'][2]-self.d_fit_files['mpe2']['bin_width_plots']/2., num_bins_plots)
        
        
        
        
        self.d_fit_files['mpe1']['array'] = tree2array(self.d_fit_files['mpe1']['tree'], [self.parameter_to_examine])
        self.d_fit_files['mpe1']['array'] = np.array(filter(lambda x: x < self.d_fit_files['mpe1']['settings'][2] and x > self.d_fit_files['mpe1']['settings'][1], self.d_fit_files['mpe1']['array'][self.parameter_to_examine]))
        
        self.d_fit_files['mpe2']['array'] = tree2array(self.d_fit_files['mpe2']['tree'], [self.parameter_to_examine])
        self.d_fit_files['mpe2']['array'] = np.array(filter(lambda x: x < self.d_fit_files['mpe2']['settings'][2] and x > self.d_fit_files['mpe2']['settings'][1], self.d_fit_files['mpe2']['array'][self.parameter_to_examine]))
        
       
        
        #self.d_fit_files['mpe1']['bin_edges'] = astroML.density_estimation.bayesian_blocks(self.d_fit_files['mpe1']['array'])
        self.d_fit_files['mpe1']['hist'], dummy = np.histogram(self.d_fit_files['mpe1']['array'], bins=self.d_fit_files['mpe1']['bin_edges'])
       
        
        #self.d_fit_files['mpe2']['bin_edges'] = astroML.density_estimation.bayesian_blocks(self.d_fit_files['mpe2']['array'])
        self.d_fit_files['mpe2']['hist'], dummy = np.histogram(self.d_fit_files['mpe2']['array'], bins=self.d_fit_files['mpe2']['bin_edges'])
        
        
        
        # set save and load paths
        self.timestamp = self.d_fit_filenames['mpe1'][6:12]
        self.s_base_save_name = 'cascade_model_fit'
        self.dict_filename = 'sampler_dictionary.p'
        self.acceptance_filename = 'acceptance_fraction.p'
        self.s_directory_save_name = 'results/%s/' % (self.timestamp)
        self.s_directory_save_plots_name = 'plots/%s/' % (self.timestamp)
        
        if self.d_fit_filenames['mpe1'] == 'nerix_160407_1533':
            self.a_free_par_guesses = [0.8881, 25.106, 0.1251, 4.55e4, 2.129e5, 9.24e5, 0.074, 1.04, 2.104]
            # var: ~2
        elif self.d_fit_filenames['mpe1'] == 'nerix_160411_1118':
            self.a_free_par_guesses = [0.892, 26.039, 0.12067, 4.84e4, 2.076e5, 8.91e5, 0.038, 1.007, 2.007]
            # var: 11.7 -> 3.3
        elif self.d_fit_filenames['mpe1'] == 'nerix_160414_1006':
            self.a_free_par_guesses = [0.911, 25.848, 0.1212, 2.59e4, 1.99e5, 8.06e5, 0.2134, 0.957, 1.962]
            # var: 7.5 -> 6.19
        elif self.d_fit_filenames['mpe1'] == 'nerix_160418_1523':
            self.a_free_par_guesses = [0.85803094415816827, 25.048046526578027, 0.12554215939235089, 15667.182858295622, 213376.65164450661, 69027.698399416462, 0.46112882231559366, 1.2840331328330929, 2.5354224394859259]
            # var: 42.3 -> 4.2
        elif self.d_fit_filenames['mpe1'] == 'nerix_160421_1521':
            self.a_free_par_guesses = [0.85803094415816827, 25.048046526578027, 0.12554215939235089, 15667.182858295622, 213376.65164450661, 69027.698399416462, 0.46112882231559366, 1.2840331328330929, 2.5354224394859259]
            # var: 5.25 (used above)
        elif self.d_fit_filenames['mpe1'] == 'nerix_160425_1529':
            self.a_free_par_guesses = [0.85803094415816827, 25.048046526578027, 0.12554215939235089, 15667.182858295622, 213376.65164450661, 69027.698399416462, 0.46112882231559366, 1.2840331328330929, 2.5354224394859259]
            # var: 3.45 (used above)
        elif self.d_fit_filenames['mpe1'] == 'nerix_160428_1008':
            self.a_free_par_guesses = [0.8505217801594902, 24.49996402551448, 0.12870859771595666, 13686.021073150921, 208465.58989236719, 276633.02893147222, 0.18354508957348148, 1.1671134580693008, 2.2919801718926083]
            # var: 33.35 -> 4.2
        elif self.d_fit_filenames['mpe1'] == 'nerix_160502_1650':
            self.a_free_par_guesses = [0.8505217801594902, 24.49996402551448, 0.12870859771595666, 13686.021073150921, 208465.58989236719, 276633.02893147222, 0.18354508957348148, 1.1671134580693008, 2.2919801718926083]
            # var: 4.7 (used above)
        elif self.d_fit_filenames['mpe1'] == 'nerix_160505_1120':
            self.a_free_par_guesses = [0.8505217801594902, 24.49996402551448, 0.12870859771595666, 13686.021073150921, 208465.58989236719, 276633.02893147222, 0.18354508957348148, 1.1671134580693008, 2.2919801718926083]
            # var: 5.1 (used above)
        elif self.d_fit_filenames['mpe1'] == 'nerix_160509_0946':
            self.a_free_par_guesses = [0.8505217801594902, 24.49996402551448, 0.12870859771595666, 13686.021073150921, 208465.58989236719, 276633.02893147222, 0.18354508957348148, 1.1671134580693008, 2.2919801718926083]
            # var: 4.5 (used above)
        elif self.d_fit_filenames['mpe1'] == 'nerix_160512_1155':
            self.a_free_par_guesses = [0.8505217801594902, 24.49996402551448, 0.12870859771595666, 13686.021073150921, 208465.58989236719, 276633.02893147222, 0.18354508957348148, 1.1671134580693008, 2.2919801718926083]
            # var: 7.59 (used above)
        elif self.d_fit_filenames['mpe1'] == 'nerix_160516_1126':
            self.a_free_par_guesses = [0.87937563599990476, 24.614689965982425, 0.12759090961320521, 38408.573975041843, 193349.18264355112, 707545.9726513573, 0.011687173143101748, 1.1324119100150347, 2.1865690908670965]
            # var:  8.72 -> 2.4
        elif self.d_fit_filenames['mpe1'] == 'nerix_160519_0952':
            self.a_free_par_guesses = [0.90663748720160187, 24.450913224066017, 0.12838606502464542, 16252.328235095738, 190671.22878131439, 47436.145097399771, 0.44327666048600939, 1.2116209625543881, 2.3588847038514675]
            # var: 22.07 -> 3.26
        elif self.d_fit_filenames['mpe1'] == 'nerix_160523_0956':
            self.a_free_par_guesses = [0.91162996161643284, 25.24416324141076, 0.12429895367421455, 16329.965673254199, 202501.83802560001, 954537.08119224641, 0.22321206793447884, 1.0855022187626575, 2.2469262556137175]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160525_1217':
            self.a_free_par_guesses = [0.86364679027814251, 24.357309647432874, 0.12959363094038975, 31851.257215202957, 193612.0482863076, 610251.36843417725, 0.065569029781821442, 1.2823867308934653, 2.5048915161056611]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160531_0936':
            self.a_free_par_guesses = [0.92489039642676674, 25.655568582066252, 0.12251197086374592, 32451.812754898358, 191914.81418980355, 686562.67913495493, 0.042095535725049316, 1.4561450033010956, 2.8125619299708902]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160604_1241':
            self.a_free_par_guesses = [0.90640449021254121, 25.981141090083494, 0.1211292990178742, 41064.217916015456, 194144.4565849966, 948342.64068968548, 0.21233518970466636, 1.2213711001112231, 2.5181396006954984]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160606_0941':
            self.a_free_par_guesses = [0.85896266408554955, 25.016104572757417, 0.12626322584168298, 42826.956321356767, 196815.66154942638, 421960.95709681005, 0.018021876232128087, 1.3466639251311627, 2.5463400540305741]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160609_0956':
            self.a_free_par_guesses = [0.86855456745454029, 24.379453535346777, 0.12938309284523042, 45262.073824882049, 197688.36985806137, 860295.93235139083, 0.029041486987830156, 1.3031248676655498, 2.5121268031634436]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160614_0956':
            self.a_free_par_guesses = [0.89021358228044423, 24.384882074039496, 0.12911609889807807, 33059.631103190099, 189975.47895769111, 59010.447016695223, 0.19156201110266036, 1.4226175745385823, 2.7353160085611599]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160617_0946':
            self.a_free_par_guesses = [0.90718652482678674, 25.327103314394421, 0.12422932988041005, 52314.317976454047, 198079.51902007853, 406518.45920144016, 0.026735216758420888, 1.4518536004429088, 2.8067367416623035]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160620_0945':
            self.a_free_par_guesses = [0.87297577072001098, 24.490021346283971, 0.12881048875805015, 81929.912356170476, 184799.34485288223, 963428.67696334294, 0.091103966175220596, 1.4076348679324153, 2.7728569517166206]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160623_1044':
            self.a_free_par_guesses = [0.89199924225432048, 24.267970500587769, 0.12971850806690502, 51617.352449284466, 197308.24665506167, 748351.43843974546, 0.10306699205852488, 1.4196732658010978, 2.8111280721387311]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160627_0944':
            self.a_free_par_guesses = [0.86926500304742704, 25.552397489220397, 0.12323658933936876, 44539.59487673665, 189175.16352115796, 47013.165172002628, 0.092420560609708158, 1.5764832933075055, 2.9709283140381197]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160630_0934':
            self.a_free_par_guesses = [0.87981132722499633, 25.881544316500776, 0.12180559291231839, 41488.458699311908, 191769.85899359034, 628547.96798815543, 0.12038570811881916, 1.4929309104505193, 2.8957097723521295]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160705_0945':
            self.a_free_par_guesses = [0.85925723545889554, 25.3727705181518, 0.12433964027462679, 56351.943099630589, 196103.61766911327, 155981.38412956905, 0.013576154270197427, 1.552976982164227, 2.9700619862754092]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160711_0808':
            self.a_free_par_guesses = [0.91818943940860331, 25.371189686785119, 0.12391092174198602, 54104.743509833417, 185853.1861528547, 17197.346350771841, 0.2203183705304067, 1.4867584458942837, 2.9744464543658298]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160715_0939':
            self.a_free_par_guesses = [0.89557496289857486, 25.287215107838385, 0.12429186861176918, 63968.766176303776, 182117.87864689133, 934466.03879330202, 0.27313941773547823, 1.4303993867405345, 2.9512503289663536]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160718_0929':
            self.a_free_par_guesses = [0.87592749965502326, 25.061544644733164, 0.12549557002763312, 58975.050754927339, 180985.70532316848, 951865.76683863637, 0.36795663746483759, 1.3473743970769445, 2.9218966521869834]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160722_0933':
            self.a_free_par_guesses = [0.93064810485595439, 25.853941630328592, 0.12136613661654715, 68134.832549495535, 185001.7678662994, 993689.02079514507, 0.30398252020649125, 1.4005826037461089, 2.9296676925756997]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160725_0936':
            self.a_free_par_guesses = [0.93419808677831695, 25.52283144090563, 0.12304829843209471, 88755.37634075337, 207930.80542890803, 941725.61535245099, 0.30040722012511584, 1.4081007625166002, 2.9374721352522299]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160728_0938':
            self.a_free_par_guesses = [0.89424681086665569, 25.14310426385002, 0.1253378858162382, 82423.476943185015, 194103.04948995364, 931037.53607224382, 0.28749722600788186, 1.3671546927002554, 2.8330315835591353]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160801_0933':
            self.a_free_par_guesses = [0.85485602926648152, 25.048816999469011, 0.12594303999336035, 80837.45370155634, 193761.44614757266, 102065.30529169837, 0.052963564611036962, 1.5402731230711131, 2.9484646548909721]
        elif self.d_fit_filenames['mpe1'] == 'nerix_160808_1038':
            self.a_free_par_guesses = [0.85835404851708486, 25.138058838308922, 0.125607865877159, 87328.208366320847, 202444.92391247553, 917369.76174181071, 0.2151003740228703, 1.4059030225197111, 2.8348766870548694]
        
        else:
            print '\n\n\nInitialization not found for %s - using earlier file\'s best fit' % (self.d_fit_filenames['mpe1'])
            print 'Please consider running minimizer before fitting with MCMC.\n\n'
            self.a_free_par_guesses = [0.85803094415816827, 25.048046526578027, 0.12554215939235089, 15667.182858295622, 213376.65164450661, 69027.698399416462, 0.46112882231559366, 1.2840331328330929, 2.5354224394859259]
        
    
    
        self.b_suppress_likelihood = False
    
    
        #print self.d_fit_files['spe']['hist']
        
        
        
    def prior_between_0_and_1(self, parameter_to_examine):
        if 0 < parameter_to_examine < 1:
            return 0
        else:
            return -np.inf



    def prior_greater_than_0(self, parameter_to_examine):
        if parameter_to_examine > 0:
            return 0
        else:
            return -np.inf



            
    def cascade_model_ln_likelihood(self, a_parameters):
        prob_hit_first, mean_e_from_dynode, probability_electron_ionized, bkg_mean, bkg_std, bkg_exp, prob_exp_bkg, mean_num_pe_mpe1, mean_num_pe_mpe2 = a_parameters

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.prior_between_0_and_1(prob_hit_first)
        ln_prior += self.prior_greater_than_0(mean_e_from_dynode)
        ln_prior += self.prior_greater_than_0(bkg_std)
        ln_prior += self.prior_greater_than_0(bkg_exp)
        ln_prior += self.prior_between_0_and_1(prob_exp_bkg)
        ln_prior += self.prior_greater_than_0(mean_num_pe_mpe1) # just require > 0
        ln_prior += self.prior_greater_than_0(mean_num_pe_mpe2) # just require > 0

        if not np.isfinite(ln_prior):
            return -np.inf

        a_hist_mpe1 = np.zeros(len(self.d_fit_files['mpe1']['hist']), dtype=np.float32)
        a_hist_mpe2 = np.zeros(len(self.d_fit_files['mpe2']['hist']), dtype=np.float32)
        
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
        prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
        
        num_bins_mpe1 = np.asarray(len(self.d_fit_files['mpe1']['hist']), dtype=np.int32)
        mpe1_bin_edges = np.asarray(self.d_fit_files['mpe1']['bin_edges'], dtype=np.float32)
        num_bins_mpe2 = np.asarray(len(self.d_fit_files['mpe2']['hist']), dtype=np.int32)
        mpe2_bin_edges = np.asarray(self.d_fit_files['mpe2']['bin_edges'], dtype=np.float32)
        
        
        l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
        l_mpe2_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe2), drv.In(mean_num_pe_mpe2), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(mpe2_bin_edges)]
    
    
        #start_time_mpe1 = time.time()
        gpu_cascade_model(*l_mpe1_args_gpu, **self.d_gpu_scale)
        #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)
        
        #start_time_mpe2 = time.time()
        gpu_cascade_model(*l_mpe2_args_gpu, **self.d_gpu_scale)
        #print 'Time for mpe2 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe2 = np.asarray(a_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/np.sum(a_hist_mpe2)
        
        
        
        ln_likelihood += poisson_binned_likelihood(a_model_mpe1, self.d_fit_files['mpe1']['hist'])
        ln_likelihood += poisson_binned_likelihood(a_model_mpe2, self.d_fit_files['mpe2']['hist'])

        total_ln_likelihood = ln_prior + ln_likelihood

        if np.isnan(total_ln_likelihood):
            return -np.inf
            
        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor
        
        return total_ln_likelihood
    
    
    
    def run_cascade_model_mcmc(self, num_walkers=32, num_steps=2000, threads=1):
        
        l_value_guesses = self.a_free_par_guesses
        l_std_guesses = [0.005, 0.5, 0.01, 1e4, 5e4, 8e4, 0.04, 0.03, 0.06]
        l_par_names = ['p_hit_first_dynode', 'electrons_per_dynode', 'p_e_freed', 'bkg_mean', 'bkg_std', 'bkg_std', 'bkg_exp', 'mean_num_pe_mpe1', 'mean_num_pe_mpe2']

        
        
        
        if not os.path.exists(self.s_directory_save_name):
            os.makedirs(self.s_directory_save_name)
        num_dim = len(l_value_guesses)
        
        
        loaded_prev_sampler = False
        try:
            # two things will fail potentially
            # 1. open if file doesn't exist
            # 2. posWalkers load since initialized to None

            with open(self.s_directory_save_name + self.dict_filename, 'r') as f_prev_sampler:

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
            if not os.path.exists(self.s_directory_save_name + self.dict_filename):
                with open(self.s_directory_save_name + self.dict_filename, 'w') as f_prev_sampler:
                    d_sampler = {}

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
            else:
                with open(self.s_directory_save_name + self.dict_filename, 'r') as f_prev_sampler:
                    d_sampler = pickle.load(f_prev_sampler)
                with open(self.s_directory_save_name + self.dict_filename, 'w') as f_prev_sampler:

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
        
        



        #sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.cascade_model_ln_likelihood, threads=threads)
        sampler = emcee.DESampler(num_walkers, num_dim, self.cascade_model_ln_likelihood, threads=threads, autoscale_gamma=True)
        
        print '\n\nBeginning MCMC sampler\n\n'
        print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)
        start_time_mcmc = time.time()

        with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
            for i, dummy in enumerate(mcmc_sampler):
                if (i != 0 and (i % 25) == 0) or (i == 3):
                    index_max_flattened = np.argmax(sampler.lnprobability[:, :i].flatten())
                    flat_chain = sampler.chain[:, :i, :].reshape(-1, num_dim)
                    self.suppress_likelihood(iterations=200, a_passed_free_par_guesses=flat_chain[index_max_flattened, :])
                

        total_time_mcmc = (time.time() - start_time_mcmc) / 3600.
        print '\n\n%d function calls took %.2f hours.\n\n' % (num_walkers*num_steps, total_time_mcmc)


        dictionary_for_sampler = sampler.__dict__
        if 'lnprobfn' in dictionary_for_sampler:
            del dictionary_for_sampler['lnprobfn']
        if 'pool' in dictionary_for_sampler:
            del dictionary_for_sampler['pool']

        with open(self.s_directory_save_name + self.dict_filename, 'r') as f_prev_sampler:
            d_sampler = pickle.load(f_prev_sampler)
        #f_prev_sampler.close()

        f_prev_sampler = open(self.s_directory_save_name + self.dict_filename, 'w')

        d_sampler[num_walkers].append(sampler.__dict__)

        pickle.dump(d_sampler, f_prev_sampler)
        f_prev_sampler.close()



        #sampler.run_mcmc(posWalkers, numSteps) # shortcut of above method
        pickle.dump(sampler.acceptance_fraction, open(self.s_directory_save_name + self.acceptance_filename, 'w'))
    
    
    
    def draw_mc_model_fit(self, a_parameters):
        prob_hit_first, mean_e_from_dynode, probability_electron_ionized, bkg_mean, bkg_std, bkg_exp, prob_exp_bkg, mean_num_pe_mpe1, mean_num_pe_mpe2 = a_parameters
        
        
        a_hist_mpe1 = np.zeros(len(self.d_fit_files['mpe1']['hist']), dtype=np.float32)
        a_hist_mpe2 = np.zeros(len(self.d_fit_files['mpe2']['hist']), dtype=np.float32)
        
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
        prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
        
        num_bins_mpe1 = np.asarray(len(self.d_fit_files['mpe1']['hist']), dtype=np.int32)
        mpe1_bin_edges = np.asarray(self.d_fit_files['mpe1']['bin_edges'], dtype=np.float32)
        num_bins_mpe2 = np.asarray(len(self.d_fit_files['mpe2']['hist']), dtype=np.int32)
        mpe2_bin_edges = np.asarray(self.d_fit_files['mpe2']['bin_edges'], dtype=np.float32)
        
        
        l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
        l_mpe2_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe2), drv.In(mean_num_pe_mpe2), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(mpe2_bin_edges)]
    
    
        #start_time_mpe1 = time.time()
        gpu_cascade_model(*l_mpe1_args_gpu, **self.d_gpu_scale)
        #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)
        
        #start_time_mpe2 = time.time()
        gpu_cascade_model(*l_mpe2_args_gpu, **self.d_gpu_scale)
        #print 'Time for mpe2 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe2 = np.asarray(a_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/np.sum(a_hist_mpe2)
        
    
        f1, (ax1) = plt.subplots(1)
        #ax1.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe1']['hist'], self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_model_mpe1, self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='r', fmt='.')
        
        
        f2, (ax2) = plt.subplots(1)
        #ax2.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe2']['hist'], self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_model_mpe2, self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='r', fmt='.')
    
        plt.show()
    
    
    
    def draw_cascade_model_with_error_bands(self, num_walkers, num_steps_to_include=20):
        if not self.b_making_comparison_plots:
            print 'Must instantiate class such that it is ready for making comparison plots\n.'
            sys.exit()
        
        print '\n\nAdded single PE spectrum with mean and variance output in this function too\n\n'
        
        num_dim = len(self.a_free_par_guesses)
        
        sPathToFile = self.s_directory_save_name + self.dict_filename
        
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
        
        num_independent_samples = num_walkers*num_steps_to_include
        
        num_bins_plots_mpe1 = len(self.d_fit_files['mpe1']['bin_centers_plots'])
        num_bins_plots_mpe2 = len(self.d_fit_files['mpe2']['bin_centers_plots'])
        a_histograms_mpe1 = np.zeros((num_independent_samples, num_bins_plots_mpe1), dtype=np.float32)
        a_histograms_mpe2 = np.zeros((num_independent_samples, num_bins_plots_mpe2), dtype=np.float32)
        a_histograms_pure = np.zeros((num_independent_samples, num_bins_plots_mpe1), dtype=np.float32)
        
        a_means = np.zeros(num_independent_samples)
        a_stds = np.zeros(num_independent_samples)
        
        
        print 'Generating spectra from draws from the posterior'
        for i in tqdm.tqdm(xrange(num_independent_samples)):
            prob_hit_first = a_sampler[i][0]
            mean_e_from_dynode = a_sampler[i][1]
            probability_electron_ionized = a_sampler[i][2]
            bkg_mean = a_sampler[i][3]
            bkg_std = a_sampler[i][4]
            bkg_exp = a_sampler[i][5]
            prob_exp_bkg = a_sampler[i][6]
            mean_num_pe_mpe1 = a_sampler[i][7]
            mean_num_pe_mpe2 = a_sampler[i][8]
        
        
            a_hist_mpe1 = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
            a_hist_mpe2 = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
            
            a_hist_pure = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
            
            mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
            mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
            
            num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
            prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
            mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
            probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
            bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
            bkg_std = np.asarray(bkg_std, dtype=np.float32)
            bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
            prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
            
            num_bins_mpe1 = np.asarray(num_bins_plots_mpe1, dtype=np.int32)
            mpe1_bin_edges = np.asarray(self.d_fit_files['mpe1']['bin_edges_plots'], dtype=np.float32)
            num_bins_mpe2 = np.asarray(num_bins_plots_mpe2, dtype=np.int32)
            mpe2_bin_edges = np.asarray(self.d_fit_files['mpe2']['bin_edges_plots'], dtype=np.float32)
            
            
            l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
            l_mpe2_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe2), drv.In(mean_num_pe_mpe2), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(mpe2_bin_edges)]
        
        
            #start_time_mpe1 = time.time()
            gpu_cascade_model(*l_mpe1_args_gpu, **self.d_gpu_scale)
            #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
            a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)*self.d_fit_files['mpe1']['bin_width']/self.d_fit_files['mpe1']['bin_width_plots']
            
            #start_time_mpe2 = time.time()
            gpu_cascade_model(*l_mpe2_args_gpu, **self.d_gpu_scale)
            #print 'Time for mpe2 call: %f s' % (time.time() - start_time_spe)
            a_model_mpe2 = np.asarray(a_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/np.sum(a_hist_mpe2)*self.d_fit_files['mpe2']['bin_width']/self.d_fit_files['mpe2']['bin_width_plots']
            
            a_histograms_mpe1[i] = a_model_mpe1
            a_histograms_mpe2[i] = a_model_mpe2
        
        
        
            # gather inputs for pure spec
            l_pure_spec = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_pure), drv.In(np.asarray(1, dtype=np.int32)), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
            
            gpu_pure_cascade_spectrum(*l_pure_spec, **self.d_gpu_scale)
            
            sum_pure_spec = np.sum(a_hist_pure)
            
            if sum_pure_spec != 0:
                a_means[i], a_stds[i] = weighted_avg_and_std(self.d_fit_files['mpe1']['bin_centers_plots'], a_hist_pure)
                a_pure_single_spec = np.asarray(a_hist_pure, dtype=np.float32)/np.sum(a_hist_pure)
                a_histograms_pure[i] = a_pure_single_spec
            else:
                print 'Bad pure spectrum giving, removing sample'
                a_means[i], a_stds[i] = -1, -1
                # remove the -1's later
        
        
        
        # remove all bad pure spectras
        a_means = filter(lambda a: a!=-1, a_means)
        a_stds = filter(lambda a: a!=-1, a_stds)
        
        
        a_one_sigma_below_mpe1 = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        a_one_sigma_above_mpe1 = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        for bin in xrange(num_bins_mpe1):
            a_one_sigma_below_mpe1[bin], a_one_sigma_above_mpe1[bin] = np.percentile(a_histograms_mpe1[:, bin], [16, 84])
        
        a_one_sigma_below_mpe2 = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
        a_one_sigma_above_mpe2 = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
        for bin in xrange(num_bins_mpe2):
            a_one_sigma_below_mpe2[bin], a_one_sigma_above_mpe2[bin] = np.percentile(a_histograms_mpe2[:, bin], [16, 84])
            
        a_one_sigma_below_pure = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        a_one_sigma_above_pure = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        for bin in xrange(num_bins_mpe1):
            a_one_sigma_below_pure[bin], a_one_sigma_above_pure[bin] = np.percentile(a_histograms_pure[:, bin], [16, 84])
            
            
            
        # get the median from a_sampler for each parameter
        
        
    
        f1, (ax1) = plt.subplots(1)
        #ax1.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe1']['hist'], self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        ax1.fill_between(self.d_fit_files['mpe1']['bin_centers_plots'], a_one_sigma_below_mpe1, a_one_sigma_above_mpe1, facecolor='red', alpha=0.1, interpolate=True)
        ax1.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe1']))
        ax1.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax1.set_ylabel('Counts')
        
        
        
        f2, (ax2) = plt.subplots(1)
        #ax2.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe2']['hist'], self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        ax2.fill_between(self.d_fit_files['mpe2']['bin_centers_plots'], a_one_sigma_below_mpe2, a_one_sigma_above_mpe2, facecolor='red', alpha=0.1, interpolate=True)
        ax2.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe2']))
        ax2.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax2.set_ylabel('Counts')
        
        
        s_mean_gain = 'Mean = %.2e +/- %.2e' % (np.average(a_means), np.std(a_means))
        s_rms_gain = 'RMS = %.2e +/- %.2e' % (np.average(a_stds), np.std(a_stds))
        
        f3, (ax3) = plt.subplots(1)
        ax3.fill_between(self.d_fit_files['mpe1']['bin_centers_plots'], a_one_sigma_below_pure, a_one_sigma_above_pure, facecolor='blue', alpha=0.3, interpolate=True)
        ax3.set_title('Single PE Spectrum - Best Fit')
        ax3.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax3.set_ylabel('Normalized Counts')


        ax1.text(0.7, 0.9, '%s\n%s' % (s_mean_gain, s_rms_gain), ha='center', va='center', transform=ax1.transAxes)
        ax2.text(0.7, 0.9, '%s\n%s' % (s_mean_gain, s_rms_gain), ha='center', va='center', transform=ax2.transAxes)
        ax3.text(0.7, 0.8, '%s\n%s' % (s_mean_gain, s_rms_gain), ha='center', va='center', transform=ax3.transAxes)
        
        if not os.path.exists(self.s_directory_save_plots_name):
            os.makedirs(self.s_directory_save_plots_name)

        f1.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_mpe1.png')
        f2.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_mpe2.png')
        f3.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_pure.png')

    
        #plt.show()



    def draw_cascade_model_corner_plot(self, num_walkers, num_steps_to_include):
        
        l_labels_for_corner_plot = ['p_hit_first_dynode', 'electrons_per_dynode', 'p_e_freed', 'bkg_mean', 'bkg_std', 'bkg_exp', 'p_exp_bkg', 'mean_num_pe_mpe1', 'mean_num_pe_mpe2']
        num_dim = len(l_labels_for_corner_plot)
        
        sPathToFile = self.s_directory_save_name + self.dict_filename
        
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
        fig = corner.corner(a_sampler, labels=l_labels_for_corner_plot, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 12})
        print 'Corner plot took %.3f minutes.\n\n' % ((time.time()-start_time)/60.)
        
        if not os.path.exists(self.s_directory_save_plots_name):
            os.makedirs(self.s_directory_save_plots_name)

        fig.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_corner.png')
        
        try:
            print emcee.autocorr.integrated_time(np.mean(a_sampler, axis=0), axis=0,
                                        low=10, high=None, step=1, c=2,
                                        fast=False)
        except:
            print 'Chain too short to find autocorrelation time!'




    def draw_fit_with_peaks(self, num_walkers, num_steps_to_include):
        
        num_dim =len(self.a_free_par_guesses)
        
        sPathToFile = self.s_directory_save_name + self.dict_filename
        
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
        a_medians = np.median(a_sampler, axis=0)


        l_num_pe = [0, 1, 2, 3, 4, 5, 6]
        l_colors = ['r', 'b', 'g', 'c', 'y', 'saddlebrown', 'orchid']
        prob_hit_first, mean_e_from_dynode, probability_electron_ionized, bkg_mean, bkg_std, bkg_exp, prob_exp_bkg, mean_num_pe_mpe1, mean_num_pe_mpe2 = a_medians

        d_hists = {}
        d_hists['mpe1'] = [np.zeros(len(self.d_fit_files['mpe1']['bin_centers_plots']), dtype=np.float32) for i in xrange(len(l_num_pe))]
        d_hists['mpe2'] = [np.zeros(len(self.d_fit_files['mpe2']['bin_centers_plots']), dtype=np.float32) for i in xrange(len(l_num_pe))]

        d_sum_hists = {}
        d_sum_hists['mpe1'] = np.zeros(len(self.d_fit_files['mpe1']['bin_centers_plots']), dtype=np.float32)
        d_sum_hists['mpe2'] = np.zeros(len(self.d_fit_files['mpe2']['bin_centers_plots']), dtype=np.float32)
        #sum_hist = np.zeros(len(self.d_fit_files['bin_centers_plots']), dtype=np.float32)
        
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)



        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
        prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
        
        bin_edges_mpe1 = np.asarray(self.d_fit_files['mpe1']['bin_edges_plots'], dtype=np.float32)
        bin_edges_mpe2 = np.asarray(self.d_fit_files['mpe2']['bin_edges_plots'], dtype=np.float32)
        
        num_bins_mpe1 = np.asarray(len(bin_edges_mpe1)-1, dtype=np.int32)
        num_bins_mpe2 = np.asarray(len(bin_edges_mpe2)-1, dtype=np.int32)
        
        sum_of_hists_mpe1 = 0
        sum_of_hists_mpe2 = 0

        for i, num_pe in enumerate(l_num_pe):
            current_hist_mpe1 = d_hists['mpe1'][i]
            num_trials_mpe1 = np.asarray(int(self.num_mc_events*scipy.stats.poisson.pmf(num_pe, mean_num_pe_mpe1)), dtype=np.int32)
            
            current_hist_mpe2 = d_hists['mpe2'][i]
            num_trials_mpe2 = np.asarray(int(self.num_mc_events*scipy.stats.poisson.pmf(num_pe, mean_num_pe_mpe2)), dtype=np.int32)
            
            num_pe = np.asarray(num_pe, dtype=np.int32)
        
        
            l_args_gpu_mpe1 = [self.rng_states, drv.In(num_trials_mpe1), drv.InOut(current_hist_mpe1), drv.In(num_pe), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(bin_edges_mpe1)]
            l_args_gpu_mpe2 = [self.rng_states, drv.In(num_trials_mpe2), drv.InOut(current_hist_mpe2), drv.In(num_pe), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(bin_edges_mpe2)]
            
            gpu_fixed_pe_cascade_spectrum(*l_args_gpu_mpe1, **self.d_gpu_scale)
            sum_of_hists_mpe1 += np.sum(current_hist_mpe1)
            
            gpu_fixed_pe_cascade_spectrum(*l_args_gpu_mpe2, **self.d_gpu_scale)
            sum_of_hists_mpe2 += np.sum(current_hist_mpe2)
            
            d_hists['mpe1'][i] = current_hist_mpe1
            d_hists['mpe2'][i] = current_hist_mpe2





        for i, num_pe in enumerate(l_num_pe):
            current_hist_mpe1 = d_hists['mpe1'][i]
            current_hist_mpe2 = d_hists['mpe2'][i]
            
            current_hist_mpe1 = np.asarray(current_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/sum_of_hists_mpe1*self.d_fit_files['mpe1']['bin_width']/self.d_fit_files['mpe1']['bin_width_plots']
            current_hist_mpe2 = np.asarray(current_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/sum_of_hists_mpe2*self.d_fit_files['mpe2']['bin_width']/self.d_fit_files['mpe2']['bin_width_plots']
            
            d_sum_hists['mpe1'] += current_hist_mpe1
            d_sum_hists['mpe2'] += current_hist_mpe2
            
            d_hists['mpe1'][i] = current_hist_mpe1
            d_hists['mpe2'][i] = current_hist_mpe2
        




        f1, (ax1) = plt.subplots(1)
        ax1.set_yscale('log', nonposx='clip')
        
        f2, (ax2) = plt.subplots(1)
        ax2.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe1']['hist'], self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='k', fmt='.')
        ax1.plot(self.d_fit_files['mpe1']['bin_centers_plots'], d_sum_hists['mpe1'], color='darkorange', linestyle='-')
        
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe2']['hist'], self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='k', fmt='.')
        ax2.plot(self.d_fit_files['mpe2']['bin_centers_plots'], d_sum_hists['mpe2'], color='darkorange', linestyle='-')
        

        for i in xrange(len(l_num_pe)):
            ax1.plot(self.d_fit_files['mpe1']['bin_centers_plots'], d_hists['mpe1'][i], color=l_colors[i])
            ax2.plot(self.d_fit_files['mpe2']['bin_centers_plots'], d_hists['mpe2'][i], color=l_colors[i])
        
        
        ax1.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe1']))
        ax1.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax1.set_ylabel('Counts')

        ax2.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe2']))
        ax2.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax2.set_ylabel('Counts')
        
        f1.savefig('%s%s_pe_specs_%s.png' % (self.s_directory_save_plots_name, self.s_base_save_name, self.d_fit_filenames['mpe1']))
        f2.savefig('%s%s_pe_specs_%s.png' % (self.s_directory_save_plots_name, self.s_base_save_name, self.d_fit_filenames['mpe2']))


    



    def differential_evolution_minimizer(self, a_bounds, maxiter=250, tol=0.05, popsize=15, polish=False):
        def neg_log_likelihood_diff_ev(a_guesses):
            return -self.cascade_model_ln_likelihood(a_guesses)
        print '\n\nStarting differential evolution minimizer...\n\n'
        result = op.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize, polish=polish)
        print result
        s_diff_ev_info = '\n\n%s, %s: %s\n' % (self.d_fit_filenames['mpe1'], self.d_fit_filenames['mpe2'], repr(list(result.x)))
        print s_diff_ev_info
        
        with open('diff_ev_results.txt', 'a') as f_diff_ev_result:
            f_diff_ev_result.write(s_diff_ev_info)



    def suppress_likelihood(self, iterations=200, a_passed_free_par_guesses=None):
    
        # reset variables in case this is not the first time run
        self.b_suppress_likelihood = False
        self.ll_suppression_factor = 1.

        #a_free_par_guesses = [0.9807, 3.1225, 4.5875e4, 2.1723e5, 1.104, 2.1]
        if a_passed_free_par_guesses == None:
            a_free_par_guesses = self.a_free_par_guesses
        else:
            a_free_par_guesses = a_passed_free_par_guesses
        
        l_parameters = [a_free_par_guesses for i in xrange(iterations)]
        l_log_likelihoods = [0. for i in xrange(iterations)]
        for i in tqdm.tqdm(xrange(iterations)):
            l_log_likelihoods[i] = self.cascade_model_ln_likelihood(a_free_par_guesses)

        #print l_log_likelihoods
        var_ll = np.std(l_log_likelihoods)

        print 'Standard deviation for %.3e MC iterations is %f' % (self.num_mc_events, var_ll)
        print 'Will scale LL such that std is 0.1'

        if var_ll < 0.1:
            self.b_suppress_likelihood = True
            self.ll_suppression_factor = 1.
            print 'Standard deviation already small so not supressing\n\n'
        else:
            self.b_suppress_likelihood = True
            self.ll_suppression_factor = var_ll / 0.1
            print 'LL suppression factor: %f\n' % self.ll_suppression_factor




if __name__ == '__main__':
    d_fit_filenames = {}
    d_fit_filenames['mpe1'] = 'nerix_160519_0952'
    d_fit_filenames['mpe2'] = 'nerix_160519_1007'

    test = fit_pmt_gain(d_fit_filenames, num_mc_events=1e7, b_making_comparison_plots=True)
    
    #test.bkg_ln_likelihood([9.42947869e-01, 7.56473634e+04, 1.12592575e+05, 4.61294214e-03])
    #print test.full_model_ln_likelihood([0.005, 1.19e4, 2.e5, -14, 0.05, 8e5, 6e5])
    #for i in xrange(10):
    #    print test.gpu_mc_model_ln_likelihood([0.05, 0.95, 3.15, 1e4, 2e5, 1.2])

    #test.draw_cascade_model_with_error_bands(num_walkers=128, num_steps_to_include=20)
    #test.draw_cascade_model_corner_plot(num_walkers=128, num_steps_to_include=20)
    #test.draw_fit_with_peaks(num_walkers=128, num_steps_to_include=150)

    test.suppress_likelihood()
    a_bounds = [(0.75, 1), (24, 26), (0.12, 0.13), (1e3, 1e5), (5e4, 5e5), (1e4, 1e6), (0, 1), (0.6, 2), (1.5, 3.0)]
    test.differential_evolution_minimizer(a_bounds, maxiter=50, tol=0.01, popsize=10, polish=False)

    #test.run_cascade_model_mcmc(num_walkers=128, num_steps=50, threads=1)



