#include "DFA.hh"
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <vector>

DFA::DFA() {
  _q0 = "";
  _transitionsDefined  = false;
  _configurationSaved  = false;
  _configurationLoaded = false;
  _configurationDfaChanged = false;
}

void DFA::saveConfiguration() {
  if(_notDefinedWell()) {
    std::cout << "DFA not defined correctly!" << std::endl;
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
  int i = 0, j = 0;
  std::cout << "name of configuration file : ";
  std::cin >> fileName;
  // add extension to the configuration file
  fileName.append(".cfg");
  std::ofstream f_DFA(fileName.c_str());
	
  f_DFA << _q0 << " " << _F.size() << std::endl;
  // write in the file: the final states
  for(F_it = _F.begin(); F_it != _F.end(); ++F_it)
    f_DFA << F_it->first << " ";
  f_DFA << std::endl;
  // write in the file: current state key, current state, symbol key, symbol and the nextState
  // the file looks like:
  // 0 current_state1 0 a next_state
  // 0 current_state1 1 b next_state
  // 1 current_state2 0 a next_state
  // ....
  for(Q_it = _Q.begin(); Q_it != _Q.end(); ++Q_it) {
    for(sigma_it = _sigma.begin(); sigma_it != _sigma.end(); ++sigma_it) {
      f_DFA << Q_it->second << " " << Q_it->first << " " 
	    << sigma_it->second << " " << sigma_it->first << " "
	    << _transitions[j][i] << std::endl;
      ++i;
    }
    i = 0;
    ++j;
  }
  f_DFA.close();
  std::cout << "configuration saved";
  _configurationSaved = true;
  _configurationDfaChanged = false;
}

void DFA::loadConfiguration() {
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
  // loading information to DFA
  int  dimFinalStates;
  std::string keyQState, keySigmaSymbol;
  std::string FState, QState, sigmaSymbol, nextQState;

  std::ifstream f_dfa(filesCfg[file].c_str());

  f_dfa >> _q0 >> dimFinalStates;

  _F.clear();
  for(int i = 0; i < dimFinalStates; ++i) {
    f_dfa >> FState;
    _F[FState] = i;
  }
  _Q.clear();
  _sigma.clear();
  while(!f_dfa.eof()) {
    f_dfa >> keyQState >> QState  
	  >> keySigmaSymbol >> sigmaSymbol 
	  >> nextQState;

    _Q[QState] = atoi(keyQState.c_str());
    _sigma[sigmaSymbol] = atoi(keySigmaSymbol.c_str());
    _transitions[atoi(keyQState.c_str())][atoi(keySigmaSymbol.c_str())] = nextQState;
    _transitionsDefined = true;
    _configurationDfaChanged = false;
  }
  std::cout << "configuration loaded";
}

void DFA::defineStates() {
  std::cout << "\t\t(. to terminate)" << std::endl;
  std::string tmp;
  std::cin >> tmp;
  int key = 0;
  _Q.clear();
  _configurationDfaChanged = true;
  // load the states set until tmp is equal to "."
  while(tmp != ".") {
    _Q[tmp] = key;
    ++key;
    std::cin >> tmp;
  }
  // insert the first state
  std::string temp;
  do {
    std::cout << "first state : ";
    std::cin >> temp;
    // and control if is a element of the states set
    if(_Q.find(temp) != _Q.end()) {
      _q0 = temp;
      break;
    }
    else
      std::cout << temp << " is not a element of the states set" << std::endl;
  } while(true);
}

void DFA::defineFinalStates() {
  if(_Q.empty())
    std::cout << "set of symbol is empty! " << std::endl;
  else {
    std::cout << "\t\t(. to terminate)" << std::endl;	
    std::string tmp;
    std::cin >> tmp;
    int key = 0;
    _F.clear();
    _configurationDfaChanged = true;
    while(tmp != ".") {
      // control if tmp is in the states set
      if(_Q.find(tmp) != _Q.end()) {
	_F[tmp] = key;
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

void DFA::defineAlphabet() {
  std::cout << "\t\t(. to terminate)" << std::endl;
	
  std::string tmp;
  std::cin >> tmp;
  int key = 0;
  _sigma.clear();
  _configurationDfaChanged = true;
  while(tmp != ".") {
    _sigma[tmp] = key;
    ++key;
    std::cin >> tmp;
  }
}

void DFA::defineTransitions() {
  // sigma and Q are empty
  if(_notDefinedWell()) {
    std::cout << "DFA not defined correctly!" << std::endl;
    std::cout << "please, define states and aphabet set" << std::endl;
    return;
  }
  std::string nextState;
  std::map<std::string, int>::iterator string;
  std::map<std::string, int>::iterator state;
  int i = 0, j = 0;
  // scan the states and sigma sets and load the transistions table
  for(state = _Q.begin(); state != _Q.end(); ++state) {
    for(string = _sigma.begin(); string != _sigma.end(); ++string) {
      std::cout << state->first << " x " << string->first << " -> ";
      std::cin >> nextState;
      // the next state is in the states set
      do {
	if(_Q.find(nextState) != _Q.end()) {
	  _transitions[j][i] = nextState;
	  ++i;
	  break;
	}
	else {
	  std::cout << nextState << " is not a element of the states set" << std::endl;
	  std::cout << state->first << " x " << string->first << " -> ";
	  std::cin >> nextState;
	}
      } while(true);
    }
    i = 0;
    ++j;
  }
  std::cout << "press return to finish";
  _transitionsDefined = true;
  _configurationSaved = false;
  _configurationDfaChanged = true;
}

void DFA::showTransitions() {
  if(!_transitionsDefined) {
    std::cout << "Transitions not defined !" << std::endl;
    return;
  }
  std::map<std::string, int>::iterator string;
  std::map<std::string, int>::iterator state;
  int i = 0, j = 0;

  std::cout << std::setw(8);
  for(string = _sigma.begin(); string != _sigma.end(); ++string)
    std::cout <<  " | " << std::setw(4) << string->first << std::setw(4);
  std::cout << " | " <<std::endl;
	

  for(unsigned i = 0; i < 7 + (8*_sigma.size()); ++i)
    std::cout << "-";
  std::cout << std::endl;


  for(state = _Q.begin(); state != _Q.end(); ++state) {
    std::cout << std::setw(4) << state->first << std::setw(4) << " | ";
    for(string = _sigma.begin(); string != _sigma.end(); ++string) {
      std::cout << std::setw(4) << _transitions[j][i] << std::setw(4) << " | ";
      ++i;
    }
    i = 0;
    ++j;
    std::cout << std::endl;
    for(unsigned i = 0; i < 7 + (8*_sigma.size()); ++i)
      std::cout << "-";
    std::cout << std::endl;
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

//**********************************************//
// DELTA_CAP(q, wa) = DELTA( DELTA_CAP(q, w), a)//
//**********************************************//

std::string DFA::_delta(int q, int w) {
  return _transitions[q][w];
}

std::string DFA::_deltaCap(std::string q, std::string wa) {
  // delta(q, epsilon) = q
  if(wa.size() == 0)
    return q;

  // delta(q, wa) = delta( delta_cap(q, w), a)
  if (wa.size() >= 1) {
    std::string w(wa.begin(), wa.end()-1);
    std::string a(wa.end()-1, wa.end());

    return _delta(_Q[_deltaCap(q, w)], _sigma[a]);	
  }
}

void DFA::check(std::string str) {
  if(_notDefinedWell()) {
    std::cout << "DFA not defined correctly";
    return;
  }

  std::string tmp;
  for(int i = 0; i < str.size(); ++i) {
    tmp = str[i];
    if(_sigma.find(tmp) == _sigma.end()) {
      std::cout << "incorrect string";
      return;
    }
  }
	
  if(_F.find(_deltaCap(_q0, str)) != _F.end())
    std::cout << "string accepted";
  else
    std::cout << "string rejected";
}

bool DFA::_notDefinedWell() {
  return _Q.empty() && _sigma.empty();
}

void DFA::halt() {
  std::string ans;
  if(_configurationDfaChanged) {
    std::cout << "the configuration of the DFA has been changed." << std::endl
	      << "Do you really want to exit? (s/n) : ";
    std::cin >> ans;
    if(ans == "s" || ans == "S")
      exit(0);
  }
  else
    exit(0);
}
