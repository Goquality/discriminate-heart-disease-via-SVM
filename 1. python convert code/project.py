from micromlgen import port
from sklearn.svm import SVC
from sklearn.datasets import load_svmlight_file as LD
import numpy as np

#kernel tag : 'linear', 'poly', 'rbf', 'sigmoid'
classifier = SVC(kernel = 'sigmoid', gamma = 0.001)
#input data values
#training_points = [[1,1], [1,2], [2,3], [3,1], [3,2], [4,3]]
#input data labels
#labels = [0,0,0,1,1,1]

##use heart.csv dataset file name
filename = 'heart.csv'
##load dataset
datafile = np.loadtxt(filename, delimiter=',', dtype=np.float32)
##delete first column : first col is label.(not data)
training_points = datafile[:, 1:]
##load only first col data: label
labels = np.loadtxt(filename, delimiter=',', usecols=0 ,dtype=np.int32)

classmap = {}
classifier.fit(training_points, labels)
classmap[0] = 'class1'
classmap[1] = 'class2'

##you can skip under three line : just view in python.

c_code = port(classifier, classmap=classmap)
print(c_code)
with open("model_sigmoid.h", "w") as f:
    f.write(c_code)


