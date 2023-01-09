import numpy
import random

f1 = open("marks1.csv", "r")
f2 = open("marks2.csv", "r")

l1 = f1.readlines()
l2 = f2.readlines()
f1.close()
f2.close()

random.shuffle(l1)
random.shuffle(l2)

f1 = open("marks1.csv", "w")
f2 = open("marks2.csv", "w")

f1.writelines(l1)
f2.writelines(l2)
