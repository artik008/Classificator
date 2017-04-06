#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>
#include "features.h"



using namespace std;

typedef pair<int, vector <float> > TModel;

class TModelList {

	map<string, int> Labels;
	multimap<int, vector <float> > Models;
	ifstream models_fin;
	ofstream models_fout;

public:
	TModelList (char* file_name);

	~TModelList ();

	void FillModelList ();

	map<string,int>::iterator FindLabels(string find_string) ;

	void FillModelsFile ();
};

