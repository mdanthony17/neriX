#!/usr/bin/python

import sys
import fann2

# create object and then functions are c functions
# without the fann_ before them

if len(sys.argv) != 2:
	print 'Use is python pos_rec_training.py <name of text file>'
	sys.exit()

filename = sys.argv[1]
#assert filename[-4:] == '.txt'
pathToFile = './data/' + filename + '.txt'

ann = fann2.libfann.neural_net()

#connection = 0.63 #0.63 ideal

ann.create_sparse_array(1, (16, 8, 2))
#ann.create_shortcut(2, 16, 2)

print 'Neural Network Created'

ann.print_parameters()

print '\n\n\n'

# consider normalizing to radius of the detector
# so that outputs are from -1 to 1

ann.set_activation_function_hidden(fann2.libfann.SIGMOID_SYMMETRIC)
ann.set_activation_function_output(fann2.libfann.LINEAR) #originally sigmoid
ann.set_training_algorithm(fann2.libfann.TRAIN_RPROP)
#ann.set_bit_fail_limit(0.0241)

#ann.set_activation_steepness_hidden(0.7)
#ann.set_activation_steepness_output(0.45)

print 'Begin Training'
ann.train_on_file(pathToFile, 10000, 100, 0.00001)
#ann.cascadetrain_on_file('./data/subdivision data/subdiv' + str(subdivisionNum) + '.txt', 6, 1, 0.00001)
print 'Training Finished'

# test gave reasonable MSE
"""
print 'Test'
input = [0.0395247127138,0.108094770127,0.0478475181581,0.0994267927609,0.161688708925,0.0564668808404,0.173783811642,0.082584832423,0.0333778879728,0.0404225057335,0.0170471264229,0.0238544378027,0.039971041431,0.0338629909065,0.0235134031841,0.0185326063311]
output = [3.22416782379, 19.1164226532]
ann.test(input, output)
print ann.get_MSE()
"""

ann.save('./nets/pr_' + filename + '.net')

ann.destroy()

