import numpy as np
import csv
import helper_classes as hc


def randomMatrixGenerator(list_size = 2, range_num = 2):
    import random
    return [[random.randint(0, 1) for j in range(range_num)] for i in range(list_size)]


def calculateEntropy(data_set):
    attrs = data_set.getAttrs()
    set_len = len(data_set.getUnfilteredList())
    if set_len <= 0:
        return 0
    attr_list = data_set.getAttrList()
    entropy = 0
    for x in attrs:
        x_len = len((data_set.getTargetedByAttr(x)))
        p_x = float(x_len/set_len)
        n_x = float((set_len - x_len)/set_len)
        if p_x > 0 and n_x > 0:
            entropy += -p_x*np.log2(p_x) -n_x*np.log2(n_x)
    return entropy


def calculateInfoGain(attr, data_set):
    original_entropy = calculateEntropy(data_set)
    attrs = data_set.getAttrs()
    subset_entropy = 0.0

    for possibility in [0, 1]:  # all possibilities of 'attr' instead of range
        possibility_subset = data_set.getListByAttrVal(attr, value=possibility)
        p_t = len(possibility_subset) / len(data_set.getUnfilteredList())

        data_subset = [list(instance.getAttributeList().values()) for instance in possibility_subset if instance.getAttributeList()[attr] == possibility]
        subset_entropy += p_t * calculateEntropy(
            hc.DataSet(data_subset, attrs, data_set.getTargetedListByAttr(attr, value=[possibility]))
        )

    return original_entropy - subset_entropy

result_set = []
def recurseDecisionTree(data_set, target_attr, attrs):
    print("current set: ", data_set)
    optimal_subset = []
    current_subset = []
    data_subset = []
    max_info_gain = 0.0
    for attr in data_set.getAttrs():
        data_subset.clear()
        del current_subset
        cur_attr_list = data_set.getListByAttr(attr)
        data_subset = [list(instance.getAttributeList().values()) for instance in cur_attr_list]
        target_attr_array = [instance.getTargetAttr() for instance in cur_attr_list]
        current_subset = hc.DataSet(data_subset, attrs, target_attr_array)
        current_info_gain = calculateInfoGain(attr, current_subset)
        if current_info_gain > max_info_gain:
            optimal_subset = current_subset
            max_info_gain = current_info_gain
    if max_info_gain != 0:
        result_set.append(max_info_gain)
        print('maximal info gain: ', max_info_gain)
        return recurseDecisionTree(optimal_subset, target_attr, attrs)
    else:
        np.savetxt("test.csv", result_set, fmt="%f", delimiter=",")
        return optimal_subset
