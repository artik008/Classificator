#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>
#include "model.h"



using namespace std;

TModelList::TModelList (char* file_name) {
		models_fin.open(file_name);
		models_fout.open("config/models");
	}

TModelList::~TModelList () {
		models_fin.close();
		models_fout.close();
	}

void TModelList::FillModelList () {
		
		while (models_fin.peek()!= -1) {
			char buffer1[100];
			char buffer2[100];
			char buffer[100];
			buffer1[0] = '\0';
			buffer2[0] = '\0';
			int i = 0;
			while (models_fin.peek()!='\n')	buffer1[i++] = models_fin.get();
			buffer1[i] = '\0';
			models_fin.get();
			while (models_fin.peek()!='\n')	models_fin.get();
			models_fin.get();
			i = 0;
			while (models_fin.peek()!='\n')	buffer2[i++] = models_fin.get();
			buffer2[i] = '\0';
			models_fin.get();

			if (FindLabels(buffer1) == Labels.end()) {
				Labels.insert(pair<char*,int> (buffer1, Labels.size()));
			}
			
		Models.insert(TModel(FindLabels(buffer1)->second, GetFeatures(buffer2)));
		}
		
		
	}

map<string,int>::iterator TModelList::FindLabels(string find_string) {
		for (map<string,int>::iterator i = Labels.begin(); i != Labels.end(); i++){
			if (i->first == find_string) return i;
		}
		return Labels.end();
	}

void TModelList::FillModelsFile () {
		for (map<string,int>::iterator i = Labels.begin(); i != Labels.end(); i++){
			models_fout << i->first << endl << i->second << endl;
		}
		models_fout << '$'<< endl;
		for (multimap<int, vector <float> >::iterator i = Models.begin(); i != Models.end(); i++){
			models_fout << i->first << endl;
			for (vector <float>::iterator j = i->second.begin(); j != i->second.end(); j++){
			models_fout << *j << " ";
			}
			models_fout << endl;
		}

}


