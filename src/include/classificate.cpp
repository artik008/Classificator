#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "classificate.h"



using namespace std;

TClassificator::TClassificator (char* file_name) {
		models_fin.open("config/models");
		class_file.open(file_name);
		strcpy(in_file_name, file_name);
	}

TClassificator::TClassificator (const TClassificator& Cl) {
		Labels = Cl.Labels;
		Models = Cl.Models;
		models_fin.open("config/models");
		class_file.open(Cl.in_file_name);
	}

TClassificator::~TClassificator () {
		models_fin.close();
		class_file.close();
	}

void TClassificator::GetModel () {
		
		while (models_fin.peek()!= '$') {
			char buffer1[100];
			char buffer2[100];
			buffer1[0] = '\0';
			buffer2[0] = '\0';
			int i = 0;
			while (models_fin.peek()!='\n')	buffer1[i++] = models_fin.get();
			buffer1[i] = '\0';
			models_fin.get();
			i = 0;
			while (models_fin.peek()!='\n')	buffer2[i++] = models_fin.get();
			buffer2[i] = '\0';
			models_fin.get();

			Labels.insert(pair<char*,int> (buffer1, atoi(buffer2)));
			
		}
		models_fin.get();
		models_fin.get();
		while (models_fin.peek()!= -1) {
			char buffer1[100];
			char buffer2[100];
			buffer1[0] = '\0';
			buffer2[0] = '\0';

			int i = 0;
			while (models_fin.peek()!='\n')	 buffer1[i++] = models_fin.get();
			buffer1[i] = '\0';
			models_fin.get();
			vector<float> v1;
			while (models_fin.peek()!='\n') {
				i = 0;
				while (models_fin.peek()!=' ') buffer2[i++] = models_fin.get();
				buffer2[i] = '\0';
				models_fin.get();
				v1.push_back(atof(buffer2));

			}
			models_fin.get();
			Models.insert(TModel(atoi(buffer1), v1));
		}
	}

string TClassificator::Classificate_Text(char* file_name) {
		vector<float> cl_features = GetFeatures(file_name);

		float precision = -1;
		float new_precision = 0;
		int label = -1;

		for (multimap<int, vector <float> >::iterator i = Models.begin(); i != Models.end(); i++){
			new_precision = GetPrecision(cl_features, i->second);
			if (new_precision > precision) {
				precision = new_precision;
				label = i->first;
			}
		}

		return ResultBook(label);
	}

float TClassificator::GetPrecision (vector<float> v1, vector<float> v2) {
		float summ = 0;
		for (vector <float>::iterator i = v1.begin(), j = v2.begin(); i != v1.end() && j != v2.end();i++, j++){
				summ+= (*i - *j) * (*i - *j);
				//cout << *i - *j << endl;
			}

		//cout << endl << 100*(1-sqrt(summ)) << endl << endl;
		return 100*(1-sqrt(summ));

	}

string TClassificator::ResultBook (int label) {
		for (map<string,int>::iterator i = Labels.begin(); i != Labels.end(); i++){
			if (label == i->second) return i->first;
		}
		return "Can't cassificate";

	}

