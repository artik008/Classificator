#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include "freeling.h"
#include "features.h"


using namespace std;

freeling::maco_options my_maco_options (const wstring &lang, const wstring &lpath) {
  freeling::maco_options opt(lang);
  opt.UserMapFile = L"";
  opt.LocutionsFile = lpath + L"locucions.dat"; 
  opt.AffixFile = lpath + L"afixos.dat";
  opt.ProbabilityFile = lpath + L"probabilitats.dat"; 
  opt.DictionaryFile = lpath + L"dicc.src";
  opt.NPdataFile = lpath + L"np.dat"; 
  opt.PunctuationFile = lpath + L"../common/punct.dat"; 
  return opt;
}


void ParseText (char* file_name, list<freeling::word>* lw, list<freeling::sentence>* ls ) {

  freeling::util::init_locale(L"default");

  wstring lang = L"ru";
 
  wstring ipath = L"/usr/local";

  wstring lpath = ipath+L"/share/freeling/"+lang+L"/";

  freeling::tokenizer tk(lpath+L"tokenizer.dat"); 
  freeling::splitter sp(lpath+L"splitter.dat");

  freeling::maco_options opt = my_maco_options(lang,lpath);
  freeling::maco morfo(opt);

  morfo.set_active_options (false,  // UserMap
                            true,  // NumbersDetection,
                            true,  // PunctuationDetection,
                            true,  // DatesDetection,
                            true,  // DictionarySearch,
                            true,  // AffixAnalysis,
                            false, // CompoundAnalysis,
                            true,  // RetokContractions,
                            true,  // MultiwordsDetection,
                            true,  // NERecognition,
                            false, // QuantitiesDetection,
                            true); // ProbabilityAssignment

  freeling::hmm_tagger tagger(lpath+L"tagger.dat", true, FORCE_TAGGER); 

  wstring text=L"";
  wstring line;
  string sline;
  ifstream fin;
  fin.open(file_name);
  while ((fin >> sline)) {
    line = freeling::util::string2wstring(sline);
    text = text + line + L" ";
   }

  *lw = tk.tokenize(text);
  *ls = sp.split(*lw);
  morfo.analyze(*ls);
  tagger.analyze(*ls);
  fin.close();
}

vector<float> norm (vector<float> vect) {
    float norm = 0.0;
        for (vector<float>::iterator i = vect.begin(); i < vect.end(); i++) {
            norm += (*i)*(*i);
        }
        norm = sqrt(norm);

        for (vector<float>::iterator i = vect.begin(); i < vect.end(); i++) {
            *i /= norm;
        }
    return vect;
}

vector<float> GetSpeechParts (list<freeling::sentence> ls, list<freeling::word> lw) {
int SpeechParts[5] = {};
		for (list<freeling::sentence>::const_iterator is=ls.begin(); is!=ls.end(); ++is)
    		for (freeling::sentence::const_iterator w=is->begin(); w!=is->end(); ++w)
    			switch (w->get_tag()[0]) {
    				case 'A': SpeechParts[0]++; break; // Adjective
    				case 'E': SpeechParts[1]++; break; // Pronoun
    				case 'V': SpeechParts[2]++; break; // Verb
    				case 'Q': SpeechParts[3]++; break; // Participle
    				case 'J': SpeechParts[4]++; break; // Interjection
    				default: break;  
    			}
    	vector <float> Features1;
    	for (int i = 0; i < 6; i++) {
    		Features1.push_back((float)SpeechParts[i]/(float)lw.size());
    		Features1.push_back((float)SpeechParts[i]/(float)ls.size());
    	}
 	return Features1;
 }

 map<wstring, int> GetGramms (string file_name) {
 	ifstream fin;
 	fin.open(file_name);
 	map<wstring, int> gramms;
 	string line;

 	while (fin.peek() != -1) {
 			fin >> line;
 			gramms.insert(pair<wstring, int> (freeling::util::string2wstring(line), 0));
 			}
 	return gramms; 

 }

 void CheckPunctuation (wstring lemma, map<wstring, int>* punctuation) {
 	for (map<wstring, int>::iterator i = punctuation->begin(); i != punctuation->end(); i++) {
 			if (i->first == lemma) {
 				i->second++;
 				break;
 			}
 	} 
 }

 void CheckGramms (wstring lemma, map<wstring, int>* letters, map<wstring, int>* bigramms, map<wstring, int>* trigramms) {
 	for (int j = 0; j < lemma.length(); j++) {

 		for (map<wstring, int>::iterator i = letters->begin(); i != letters->end(); i++) {
	 		if (i->first[0] == lemma[j]) {
 					i->second++;
 				}
 			}
 		if (j < lemma.length() - 1)
 		for (map<wstring, int>::iterator i = bigramms->begin(); i != bigramms->end(); i++) {
 			if ((i->first[0] == lemma[j])&&(i->first[1] == lemma[j+1])) {
 				i->second++;
 			}
 		}
 		if (j < lemma.length() - 2)
 		for (map<wstring, int>::iterator i = trigramms->begin(); i != trigramms->end(); i++) {
 			if ((i->first[0] == lemma[j])&&
 					(i->first[1] == lemma[j+1])&&
 						(i->first[2] == lemma[j+2])) i->second++;
 		}
 	}

 }

 vector<float> GetGrammsFrequency (list<freeling::sentence>* ls) {
 	
 	int Punct = 0;
 	int Letters = 0;
 	int Bigramms = 0;
 	int Trigramms = 0;
 	
 	map<wstring, int> letters = {{freeling::util::string2wstring("с"), 0}, 
 								 {freeling::util::string2wstring("е"), 0}, 
 								 {freeling::util::string2wstring("н"), 0}, 
 								 {freeling::util::string2wstring("о"), 0}, 
 								 {freeling::util::string2wstring("в"), 0}, 
 								 {freeling::util::string2wstring("а"), 0}, 
 								 {freeling::util::string2wstring("л"), 0}, 
 								 {freeling::util::string2wstring("и"), 0}, 
 								 {freeling::util::string2wstring("т"), 0}, 
 								 {freeling::util::string2wstring("р"), 0}};
 	
 	map<wstring, int> punctuation = {{freeling::util::string2wstring("?"), 0},
 									 {freeling::util::string2wstring("!"), 0},
 									 {freeling::util::string2wstring(";"), 0}};
 	
 	map<wstring, int> Mbigramms = GetGramms("config/bigramms");
 	map<wstring, int> Mtrigramms = GetGramms("config/trigramms");
 	
 	for (list<freeling::sentence>::const_iterator is=ls->begin(); is!=ls->end(); ++is) {
    		for (freeling::sentence::const_iterator w=is->begin(); w!=is->end(); ++w) {
    			if (w->get_tag()[0] == 'F') 
    				{ CheckPunctuation(w->get_lemma(), &punctuation); Punct++;}
    				else { 
    						CheckGramms(w->get_lemma(), &letters, &Mbigramms, &Mtrigramms); 
    						Letters+=w->get_lemma().length(); 
    						Bigramms+=w->get_lemma().length()-1;
    						if (w->get_lemma().length() > 1 ) 
    							Trigramms+=w->get_lemma().length()-2; 
    					}
    			}
    		}
    
    vector <float> VPunct;
    for (map<wstring, int>::iterator i = punctuation.begin(); i != punctuation.end(); i++) 
    	VPunct.push_back((float)i->second/(float)Punct);
    
    vector <float> VLetters;
    for (map<wstring, int>::iterator i = letters.begin(); i != letters.end(); i++) 
    	VLetters.push_back((float)i->second/(float)Letters);
    
    vector <float> VBigramms;
    for (map<wstring, int>::iterator i = Mbigramms.begin(); i != Mbigramms.end(); i++) 
    	VBigramms.push_back((float)i->second/(float)Bigramms);
    
    vector <float> VTrigramms;
    for (map<wstring, int>::iterator i = Mtrigramms.begin(); i != Mtrigramms.end(); i++) 
    	VTrigramms.push_back((float)i->second/(float)Trigramms);

    VPunct = norm(VPunct);
	VLetters = norm(VLetters);
	VBigramms = norm(VBigramms);
	VTrigramms = norm(VTrigramms);
	
	vector <float> VFeatures;
	
	VFeatures.insert(VFeatures.end(),VPunct.begin(), VPunct.end());
	
	VFeatures.insert(VFeatures.end(),VLetters.begin(), VLetters.end());
	
	VFeatures.insert(VFeatures.end(),VBigramms.begin(), VBigramms.end());
	
	VFeatures.insert(VFeatures.end(),VTrigramms.begin(), VTrigramms.end());

	return norm(VFeatures);
 }


vector<float> GetFeatures(char* file_name) {
	
	vector<float> Features;
		
	list<freeling::word> lw;
	list<freeling::sentence> ls;
		
	ParseText(file_name, &lw, &ls);
		
	float feature = (float)lw.size()/(float)ls.size();
		
	Features.push_back(feature);
		
	vector<float> Features0;
    Features0 = norm(GetSpeechParts(ls, lw));
    Features.insert(Features.end(),Features0.begin(), Features0.end());
    	
  Features0 = GetGrammsFrequency(&ls);
	Features.insert(Features.end(),Features0.begin(), Features0.end());
		
	return norm(Features);
}