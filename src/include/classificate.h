#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "model.h"



using namespace std;

typedef pair<int, vector <float> > TModel;

class TClassificator {

	map<string, int> Labels;
	multimap<int, vector <float> > Models;
	ifstream models_fin;
	ifstream class_file;
	char in_file_name[300];

public:
	TClassificator (char* file_name);

	TClassificator (const TClassificator& Cl);

	~TClassificator ();

	void GetModel ();

	string Classificate_Text(char* file_name);

	float GetPrecision (vector<float> v1, vector<float> v2);

	string ResultBook (int label);
};

