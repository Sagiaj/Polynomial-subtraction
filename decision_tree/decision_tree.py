import numpy as np
from sklearn.cluster import KMeans
import helper_functions as hf
import helper_classes as hc
import random

# column is attr (for example height is an attr)
# row is an instance(for example a person is an instance)
# steps:
# calculate information gain recursively
# export the results to an external file in any format
# create a decision tree class to support the gain


predefined_props = ['height', 'age', 'weight', 'strength', 'speed']
instance_num = 1000

target_attr_list = [random.randint(0,1) for i in range(1, instance_num)]
random_list = hf.randomMatrixGenerator(instance_num, len(predefined_props))

data_set = hc.DataSet(random_list, predefined_props, target_attr_list)

my_tree = hf.recurseDecisionTree(data_set, 'successful', predefined_props)
