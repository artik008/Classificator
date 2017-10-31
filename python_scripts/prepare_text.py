#!/usr/bin/env python3

from pymystem3 import *

def get_features (file_name):
	fin  = open(file_name, 'r')
	m    = Mystem()
	text = m.analyze (fin.read())
	fout = open('model', 'w')
	print (text)

get_features('test')
