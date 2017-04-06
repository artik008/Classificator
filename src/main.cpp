#include <iostream>
#include <cstring>
#include <fstream>
#include "include/classificate.h"
#include <ctime>


using namespace std;

void GetModels (char* file_name) {

	TModelList MList(file_name);
	MList.FillModelList();
	MList.FillModelsFile();
	//vector <float> v1 = GetFeatures(file_name);
	//for (vector <float>::iterator j = v1.begin(); j != v1.end(); j++){
	//  	cout << *j << " ";
	//		}

}

void Classificate (char* file_name) {
	TClassificator Classificator(file_name);
	Classificator.GetModel();
	cout << Classificator.Classificate_Text(file_name) << endl;

}

void Classificateall (char* filelist_name) {
	ifstream fin;
	ofstream resault;
	resault.open("resault");
  	fin.open(filelist_name);
  	char sline[300];
  	int i  = 0;
  	vector<TClassificator> Classificators;
  	while ((fin >> sline)) {
		Classificators.push_back(TClassificator(sline));
		Classificators[i].GetModel();
		resault << sline << "--------" << Classificators[i].Classificate_Text(sline) << endl << endl;
		i++;
	}

}

int main (int argc, char** argv) {
	unsigned int start_time =  clock();

	switch (argc) {
		case 1: cout << "usage: Author_Class [-help] [-train \"train_file_name\" | -classificate \"test_file_name\" | -classificateall \"test_file_list_name\"] " << endl; break;
		case 2: if (!strcmp(argv[1], "-help")) cout << "no help:)" << endl; 
					else if ((!strcmp(argv[1], "-train"))||(!strcmp(argv[1], "-classificate"))||(!strcmp(argv[1], "-classificateall"))) cout << "check file_name" << endl;
						else cout << "Error! Wrong arguments!" << endl;
				break;
		case 3: if (!strcmp(argv[1], "-train")) GetModels(argv[2]);
					else if (!strcmp(argv[1], "-classificate")) Classificate(argv[2]);
					else if (!strcmp(argv[1], "-classificateall")) Classificateall(argv[2]);
					else cout << "Error! Wrong arguments!" << endl; break;
		default: cout << "Error! Wrong arguments!" << endl; break;
	}

	unsigned int end_time = clock();
	cout << "Work time = " << (double)(end_time - start_time)/CLOCKS_PER_SEC << endl;

return 0;
}