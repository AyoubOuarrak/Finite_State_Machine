#include "e_NFA.hh"
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <algorithm>

e_NFA::e_NFA() {
  _q0 = "";
  _transitionsDefined   = false;
  _configurationSaved   = false;
  _configurationLoaded  = false;
  _configurationChanged = false;
}

void e_NFA::defineStates() {
  std::cout << "\t\t(. to terminate)" << std::endl;
  std::string tmp;
  std::cin >> tmp;
  int key = 0;
  _Q.clear();
  _configurationChanged = true;
  // load the states set until tmp is equal to "."
  while(tmp != ".") {
    _Q[tmp] = key;
    ++key;
    std::cin >> tmp;
  }
  // insert the first state
  std::string temp;
  while(_Q.size() != 0) {
    std::cout << "first state : ";
    std::cin >> temp;
    // and control if it's a element of the states set
    if(_Q.find(temp) != _Q.end()) {
      _q0 = temp;
      break;
    }
    else
      std::cout << temp << " is not a element of the states set" << std::endl;
  } 
}

void e_NFA::defineFinalStates() {
  if(_Q.empty())
    std::cout << "set of symbol is empty! " << std::endl;
  else {
    std::cout << "\t\t(. to terminate)" << std::endl;	
    std::string tmp;
    std::cin >> tmp;
    int key = 0;
    _F.clear();
    _finalStateSet.clear();
    _configurationChanged = true;
    while(tmp != ".") {
      // control if tmp is in the states set
      if(_Q.find(tmp) != _Q.end()) {
	_F[tmp] = key;
	_finalStateSet.insert(tmp);
	++key;
	std::cin >> tmp;
      }
      else {
	std::cout << tmp << " is not a element of the states set" << std::endl;
	std::cin >> tmp;
      }
    }
  }
}

void e_NFA::defineAlphabet() {
  std::cout << "\t\t(. to terminate)" << std::endl;
	
  std::string tmp;
  std::cin >> tmp;
  int key = 1;
  _sigma.clear();
  _sigma["$"] = 0;   //epsilon symbol
  _configurationChanged = true;
  while(tmp != ".") {
    _sigma[tmp] = key;
    ++key;
    std::cin >> tmp;
  }
}

bool e_NFA::_notDefinedWell() {
  return _Q.empty() && _sigma.empty();
}

void e_NFA::halt() {
  std::string ans;
  if(_configurationChanged) {
    std::cout << "the configuration has been changed." << std::endl
	      << "Do you really want to exit? (s/n) : ";
    std::cin >> ans;
    if(ans == "s" || ans == "S")
      exit(0);
  }
  else
    exit(0);
}

// input: {q0,q1,...,qn}
// output: a vector that contain the states without separator like: "}" , "{" , "," , " "
std::vector<std::string> 
e_NFA::_tokenizeStates(std::string states) {
  std::vector<std::string> sts;
  char* dup = strdup(states.c_str());
  char* pch;

  pch = strtok (dup," ,{}");
  while (pch != NULL) {
    sts.push_back(pch);
    pch = strtok (NULL, " ,{}");
  }
  free(dup);
  return sts;
}

// return the max size of the vector in the transitions table
unsigned e_NFA::_maxSubSet() {
  unsigned max = 0;
  for(unsigned i = 0; i < _Q.size(); ++i)
    for(unsigned j = 0; j < _sigma.size(); ++j)
      if(_transitions[i][j].size() > max)
	max = _transitions[i][j].size();
  return max;
}

void e_NFA::defineTransitions() {
  // sigma and Q are empty
  if(_notDefinedWell()) {
    std::cout << "e_NFA not defined correctly!" << std::endl;
    std::cout << "please, define states and aphabet set" << std::endl;
    return;
  }
  std::string nextStates;
  std::map<std::string, int>::iterator symbol;
  std::map<std::string, int>::iterator state;
  int s = 0, q = 0;
  // scan the states and sigma sets and load the transistions table
  std::cout << "input format : {q0,q1, ... ,qn} , '$' is epsilon\n\n";

  for(state = _Q.begin(); state != _Q.end(); ++state) {
    for(symbol = _sigma.begin(); symbol != _sigma.end(); ++symbol) {
      std::cout << state->first << " x " << symbol->first << " -> ";
      std::cin >> nextStates;
      // transform the set of states in tokens
      std::vector<std::string> Vstates = _tokenizeStates(nextStates);
      // eg. nextStates :=  {q0,q1,q2}
      // Vstates[0] = "q0", Vstates[1]="q1", Vstates[2]="q2"
      for(unsigned i = 0; i < Vstates.size(); ++i) {
	if(_Q.find(Vstates[i]) == _Q.end()) {
	  std::cout << Vstates[i] << " is not a element of the states set" << std::endl;
	  std::cout << state->first << " x " << symbol->first << " -> ";
	  std::cin >> nextStates;
	  // transform the input states in tokens
	  Vstates.clear();
	  Vstates = _tokenizeStates(nextStates);
	  i = -1; // is necessary to begin the loop from 0
	}
	else 
	  _transitions[q][s].push_back(Vstates[i]);
      }
      ++s;
    }
    s = 0;
    ++q;
  }
  std::cout << "press return to finish";
  _transitionsDefined = true;
  _configurationSaved = false;
  _configurationChanged = true;
}

void e_NFA::showTransitions() {
  if(!_transitionsDefined) {
    std::cout << "Transitions not defined !" << std::endl;
    return;
  }
  std::map<std::string, int>::iterator symbol;
  std::map<std::string, int>::iterator state;
  int s = 0, q = 0;

  std::cout << "|" << std::setw(3) << " State " 
	    << "|" << std::setw(3) << " Input "
	    << "|" << std::setw(3) << " Next States " << std::endl;
	
  for(unsigned i = 0; i < 19 + (_maxSubSet()*2)+(_maxSubSet()-1)+2; ++i)
    std::cout << "-";
  std::cout << std::endl;

  for(state = _Q.begin(); state != _Q.end(); ++state) {
    for(symbol = _sigma.begin(); symbol != _sigma.end(); ++symbol) {
      std::cout << "|" << std::setw(5) << state->first << std::setw(3)
		<< "|" << std::setw(4) << symbol->first << std::setw(4)
		<< "|" << std::setw(3) << "{";
      for(unsigned sts = 0; sts < _transitions[q][s].size(); ++sts) {
	std::cout << _transitions[q][s].at(sts);
	if(sts + 1 < _transitions[q][s].size()) 
	  std::cout << ",";
      }
      std::cout << "}" << std::endl;
      ++s;
      for(unsigned i = 0; i < 19 + (_maxSubSet()*2)+(_maxSubSet()-1)+2; ++i)
	std::cout << "-";
      std::cout << std::endl;
    }
    s = 0;
    ++q;
  }

  std::cout << std::endl;
  std::cout << "first state : " << _q0 << std::endl;

  std::map<std::string, int>::iterator it = _F.begin();
  if(!_F.empty()) {
    std::cout << "final states : { ";
    std::cout << it->first;
    ++it;
    for(; it != _F.end(); ++it) 
      std::cout << " , " << it->first;
    std::cout << " }";	
  }
}

void e_NFA::saveConfiguration() {
  if(_notDefinedWell()) {
    std::cout << "e_NFA not defined correctly!" << std::endl;
    std::cout << "please, define states and aphabet set" << std::endl;
    return;
  }

  if(_configurationSaved) {
    std::cout << "configuration already saved!";
    return;
  }

  std::string fileName;
  std::map<std::string, int>::iterator F_it;
  std::map<std::string, int>::iterator Q_it;
  std::map<std::string, int>::iterator sigma_it;
  int q = 0, s = 0;
  std::cout << "name of configuration file : ";
  std::cin >> fileName;
  // add extension to the configuration file
  fileName.append(".cfg");
  std::ofstream f_e_NFA(fileName.c_str());
	
  f_e_NFA << _q0 << " " << _F.size() << std::endl;
  // write in the file: the final states
  for(F_it = _F.begin(); F_it != _F.end(); ++F_it)
    f_e_NFA << F_it->first << " ";
  f_e_NFA << std::endl;
  // write in the file: current state key, current state, symbol key, symbol, N° nextStates, nextState1, ... nextStaten
  // the file looks like:
  // 0 current_state1 0 a 1 nextState1
  // 0 current_state1 1 b 3 next_state1 next_state2 next_state3
  // 1 current_state2 0 a 0 
  // ....
  for(Q_it = _Q.begin(); Q_it != _Q.end(); ++Q_it) {
    for(sigma_it = _sigma.begin(); sigma_it != _sigma.end(); ++sigma_it) {
      f_e_NFA << Q_it->second << " " << Q_it->first << " " 
	      << sigma_it->second << " " << sigma_it->first << " " 
	      << _transitions[q][s].size() << " ";
      for(unsigned j = 0; j < _transitions[q][s].size(); ++j)
	f_e_NFA << _transitions[q][s].at(j) << " ";
      f_e_NFA << std::endl;
      ++s;
    }
    s = 0;
    ++q;
  }
  f_e_NFA.close();
  std::cout << "configuration saved";
  _configurationSaved = true;
  _configurationChanged = false;
}

void e_NFA::loadConfiguration() { 
  // write in cfg_file how many file with the extension ".cfg"
  // there are in the directory
  system("ls *.cfg | wc -l > cfg_file");

  std::ifstream tmp("cfg_file");
  int nFileCfg;
  // read from the file, the number of file with extension ".cfg"
  tmp >> nFileCfg;
  if(nFileCfg == 0) {
    std::cout << "no configuration file found!";
    tmp.close();
    return;
  }
  // write in the cfg_file, the files with ".cfg" extension
  system("ls *.cfg > cfg_file");

  std::ifstream f_cfg("cfg_file");
  std::vector<std::string> filesCfg;
  std::string temp;
  while(!f_cfg.eof()) {
    f_cfg >> temp;
    // load fileCfg with the name of files
    filesCfg.push_back(temp);
  }
  f_cfg.close();

  unsigned file;
  unsigned i = 0;
  std::cout << "which file you want to load?" << std::endl;
  for(; i < filesCfg.size()-1; ++i)
    std::cout << i << " - " << filesCfg[i] << std::endl;
  std::cout << std::endl;
  std::cin >> file;
	
  if(file > i-1) {
    std::cout << "file does not exist" << std::endl;
    return;
  }
  // loading information to e_NFA
  unsigned  dimFinalStates, dimNextStates;
  std::string keyQState, keySigmaSymbol;
  std::string FState, QState, sigmaSymbol, nextQState;
  std::vector<std::string> VNextStates;
  std::ifstream f_e_NFA(filesCfg[file].c_str());

  f_e_NFA >> _q0 >> dimFinalStates;

  _F.clear();
  _finalStateSet.clear();
  for(unsigned i = 0; i < dimFinalStates; ++i) {
    f_e_NFA >> FState;
    _F[FState] = i;
    _finalStateSet.insert(FState);
  }
  _Q.clear();
  _sigma.clear();
  while(!f_e_NFA.eof()) {
    f_e_NFA >> keyQState >> QState  
	    >> keySigmaSymbol >> sigmaSymbol 
	    >> dimNextStates;
    for(unsigned j = 0; j < dimNextStates; ++j) {
      f_e_NFA >>nextQState;
      VNextStates.push_back(nextQState);
    }
			
    _Q[QState] = atoi(keyQState.c_str());
    _sigma[sigmaSymbol] = atoi(keySigmaSymbol.c_str());
    _transitions[atoi(keyQState.c_str())][atoi(keySigmaSymbol.c_str())] = VNextStates;
    VNextStates.clear();
    _transitionsDefined = true;
    _configurationChanged = false;
  }
  std::cout << "configuration loaded";
}
/********************************************************
 *     deltaCap(q, epsilon) = e_closure(q)     
 *
 *     deltaCap(q, wa) =     UNION   e_closure(delta(p, a)) 
 *						    [p in deltaCap(q, w)]
 ********************************************************/

std::vector<std::string>
e_NFA::_eStep(int q) {
  // return the set of states in the cell [q, epsilon] 
  return _delta(q, 0);
}

std::set<std::string>
e_NFA::_eClosure(std::string q) {
  _epsilonClosed.insert(q);

  std::vector<std::string> eSteps = _eStep(_Q[q]);
  std::vector<std::string>::iterator qn;

  for(qn = eSteps.begin(); qn != eSteps.end(); ++qn) 
    _eClosure(*qn);

  return _epsilonClosed;
}

std::vector<std::string> 
e_NFA::_delta(int q, int w) {
  return _transitions[q][w];
}

void e_NFA::_deltaCap(std::string q, std::string wa) {
  if(wa.size() == 1) {
    std::set<std::string> eClosure = _eClosure(q);
    std::set<std::string>::iterator qn;

    for(qn = eClosure.begin(); qn != eClosure.end(); ++qn) {
      std::vector<std::string> delta = _delta(_Q[*qn], _sigma[wa]);
      if(delta.size() > 0) {
	_deltaCapSet.insert(delta.begin(), delta.end());
	std::vector<std::string>::iterator qDelta;
	for(qDelta = delta.begin(); qDelta != delta.end(); ++qDelta) {
	  std::set<std::string> eClosureDelta = _eClosure(*qDelta);
	  _deltaCapSet.insert(eClosureDelta.begin(), eClosureDelta.end());
	}
      }
    }
  }
  // deltaCap(q, wa) =     UNION   e_closure(delta(p, a)) 
  //	[p in deltaCap(q, w)]
  if(wa.size() > 1) {
    std::string w(wa.begin(), wa.end()-1);
    std::string a(wa.end()-1, wa.end());

    _deltaCap(q, w);

    std::set<std::string>::iterator p; 
    std::set<std::string>::iterator qn;
    std::set<std::string> tmpSet;
		
    for(p = _deltaCapSet.begin(); p != _deltaCapSet.end(); ++p) {
      std::set<std::string> eClosure = _eClosure(*p);
      for(qn = eClosure.begin(); qn != eClosure.end(); ++qn) {
	std::vector<std::string> delta = _delta(_Q[*qn],_sigma[a]);
	tmpSet.insert(delta.begin(), delta.end());
      }
    }
    _deltaCapSet.swap(tmpSet);
  }
}

unsigned 
e_NFA::_intersection(std::set<std::string>& s1, std::set<std::string>& s2) {
  std::set<std::string> intersect;
  std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), 
			std::inserter(intersect,intersect.begin()));
  return intersect.size();
}

void e_NFA::check(std::string str) {
  // a string x is accepted by a e_NFA if:
  // deltaCap(q0, x) ∩ F ≠ ∅
  if(_notDefinedWell()) {
    std::cout << "e_NFA not defined correctly";
    return;
  }

  std::string tmp;
  for(unsigned i = 0; i < str.size(); ++i) {
    tmp = str[i];
    if(_sigma.find(tmp) == _sigma.end()) {
      std::cout << "incorrect string";
      return;
    }
  }
	
  _deltaCapSet.clear();
  _epsilonClosed.clear();
  _deltaCap(_q0, str);
   
  if(_intersection(_deltaCapSet, _finalStateSet) != 0)
    std::cout << "string accepted";
  else
    std::cout << "string rejected";
}
