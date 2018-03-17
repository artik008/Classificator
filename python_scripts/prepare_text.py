#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os 

from nltk.tokenize import sent_tokenize, word_tokenize
from collections import OrderedDict
from pymystem3 import *

m = Mystem()

def check_len(l):
	if l == 0:
		l = 1
	return l

class Features:
	def __init__(self, text, id_num):
		self.user = (id_num.split(':')[0]).split('=')[1]
		self.id = (id_num.split(':')[1]).split('=')[1]
		self.analized_text = m.analyze(text)
		self.count_words = check_len(len(self.analized_text))
		self.count_chars = check_len(len(text))
		self.count_sents = check_len(len(sent_tokenize(text)))
		self.features_list = [ self.count_words/self.count_sents, self.count_chars/self.count_words, self.count_chars/self.count_sents]

		fgramms = open('gramms', 'r')
		fvocabulary = open('vocabulary', 'r')

		self.gramms_vok        = OrderedDict.fromkeys((fgramms.read()).split(' '), 0)
		self.speech_parts_vok  = OrderedDict.fromkeys(['A', 'ADV','ADVPRO', 'ANUM', 'APRO', 'COM', 'CONJ', 'INTJ', 'NUM', 'PART', 'PR', 'S', 'SPRO', 'V'], 0)
		self.grammar_forms_vok = OrderedDict.fromkeys(['прев','срав','деепр','прич','изъяв','пов','наст','непрош','прош','действ','страд','имя'], 0)
		self.count_punct       = OrderedDict.fromkeys(['.',',','!','?',':','"',':)','\n'], 0)
		self.main_vocabulary   = OrderedDict.fromkeys((fvocabulary.read()).split('\n'), 0)
		
		fgramms.close()
		fvocabulary.close()

	def count_base_features(self):

		def check_vok(key, vok):
			if vok.get(key) != None:
					vok[key] += 1

		for token in self.analized_text:
			tok_text = del_spaces(token.get('text'))
			check_vok(tok_text, self.count_punct)

			bigramm  = '  '
			trigramm = '   '
			for char in tok_text:
				bigramm  = bigramm[1] + char
				trigramm = trigramm[1] + trigramm[2] + char
				check_vok(char, self.gramms_vok)
				check_vok(bigramm, self.gramms_vok)
				check_vok(trigramm, self.gramms_vok)

			if token.get('analysis') != None:
				word_info_list = (token.get('analysis'))
				if len(word_info_list) > 0:
					word_info = word_info_list[0].get('gr')
					speech_part = ''
					for ch in word_info:
						if ch != ',' and ch != '=':
							speech_part += ch
						else:
							break
					check_vok(speech_part, self.speech_parts_vok)
					for key in self.grammar_forms_vok.keys():
						if word_info.find(key) != -1:
							self.grammar_forms_vok[key] += 1
					check_vok(word_info_list[0].get('lex'), self.main_vocabulary)



		def make_frequencies(count, vok):
			for key in vok.keys():
				vok[key] /= count
		
		make_frequencies(self.count_words, self.count_punct)
		make_frequencies(self.count_chars, self.gramms_vok)
		make_frequencies(self.count_words, self.speech_parts_vok)
		make_frequencies(self.count_words, self.grammar_forms_vok)

		self.features_list += self.count_punct.values() 
		self.features_list += self.gramms_vok.values() 
		self.features_list += self.speech_parts_vok.values()
		self.features_list += self.grammar_forms_vok.values()
		self.features_list += self.main_vocabulary.values()

def del_spaces(text):
	res = ''
	for char in text:
		if char != ' ':
			res += char
	return res

def get_features (text):
	text = text.split('<article_info>\n')
	f = Features(text[1], text[0])
	f.count_base_features()
	return f

def learning (filename):
	with open(filename, 'r') as farticles:
		articles = ((farticles.read()).split('\n\t\n\t\n'))
	articles.pop()
	articles = list(map(get_features,articles))
	learning_claster_list = Clasters_list()
	learning_claster_list.make_learning_clasters(articles)
	for cl in learning_claster_list.claster_list:
		cl.print_clt()


class Claster:
	def __init__(self, id_, first_elem):
		self.claster_id = id_
		self.elem_list  = [first_elem]
		self.mass_elem  = first_elem

	def add_elem(self, new):
		self.elem_list += [new]
		self.mass_elem = self.newmass(new)

	def add_claster(self, new):
		self.elem_list.append(new.elem_list)
		self.mass_elem = self.newmass(new.mass_elem)

	def newmass(self, new):
		return self.mass_elem

	def print_clt(self):
		print('id = ' + str(self.claster_id) + '\n')
		for e in self.elem_list:
			print('elem_id = ' + str(e.id) + '  user_id = ' + str(e.user) + '\n')
		print('\n')

class Clasters_list:
	def __init__ (self):
		self.count_clasters = 0
		self.claster_list = []

	def add_claster(self, new):
		self.count_clasters += 1
		self.claster_list.append(new)

	def del_claster(self, elem):
		self.count_clasters -= 1
		self.claster_list.remove(elem)

	def scan_elem(self, new):
		i = 0
		while i < self.count_clasters and str((self.claster_list[i]).claster_id) != new.user:
			i += 1
		if i < self.count_clasters:
			self.claster_list[i].add_elem(new)
		else:
			self.add_claster(Claster(new.user, new))

	def make_learning_clasters (self, features_list):
		for element in features_list:
			self.scan_elem(element)

def count_quality (cl_list, teach_cl_list, old_quality):
	quality = 0
	for clast in cl_list:
		for tclast in teach_cl_list:
			if len(clast.elem_list) > 1:
				q = same_elems(clast.elem_list, tclast.elem_list)
				if q > 0:
					quality += q
					quality -= (len(clast.elem_list) + len(tclast.elem_list) - 2*q)
	if old_quality > quality:
		return 0
	else:
		return quality

def same_elems (elems, telems):
	count = 0
	for elem in elems:
		if telems.find(elem) != -1:
			count += 1
	return count

def check_quality (cl_list, flag, quality, teach_cl_list):
	if flag:
		if cl_list.list_quality < quality:
			return 0
		else:
			return quality
	else:
		return count_quality (cl_list, teach_cl_list, quality)

def clustering (cl_list, flag, quality, teach_cl_list):
	min = 10000000
	first_claster  = cl_list.claster_list[0]
	second_claster = cl_list.claster_list[0]
	for i in range(0, cl_list.count_clasters-1):
		for j in range(i+1, cl_list.count_clasters):
			if min > dist(cl_list.claster_list[i], cl_list.claster_list[j]):
				min = dist(cl_list.claster_list[i], cl_list.claster_list[j])
				first_claster  = cl_list.claster_list[i]
				second_claster = cl_list.claster_list[j]
	cl_list.del_claster(first_claster)
	cl_list.del_claster(second_claster)
	first_claster.add_claster(second_claster)
	cl_list.add_claster(first_claster)
	if (check_quality(cl_list, flag, quality, teach_cl_list)) != 0:
		return cl_list
	else:
		return clustering(cl_list, flag, quality, teach_cl_list)




learning('articles')