#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include "freeling.h"


using namespace std;


freeling::maco_options my_maco_options (const wstring &lang, const wstring &lpath);


void ParseText (char* file_name, list<freeling::word>* lw, list<freeling::sentence>* ls );

vector<float> norm (vector<float> vect);

vector<float> GetSpeechParts (list<freeling::sentence> ls, list<freeling::word> lw);

 map<wstring, int> GetGramms (string file_name);

 void CheckPunctuation (wstring lemma, map<wstring, int>* punctuation);

 void CheckGramms (wstring lemma, map<wstring, int>* letters, map<wstring, int>* bigramms, map<wstring, int>* trigramms);
 vector<float> GetGrammsFrequency (list<freeling::sentence>* ls);

vector<float> GetFeatures(char* file_name);