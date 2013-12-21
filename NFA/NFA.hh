/*
 *		Author  : Ouarrak Ayoub
 *		Date    : 14/12/2013
 *		version : v1.0
 *
 *		NFA = (Q, sigma, delta, q0, F)
 *
 *		Q     : finite set of states
 *		sigma : alphabet
 *		delta : transition function  delta : (Q x sigma) --> P(Q) 
 *		q0    : first state
 *		F     : set of final states  F (subset of) Q	
 */
#ifndef NON_DETERMINISTIC_FINITE_AUTOMATA_HH
#define NON_DETERMINISTIC_FINITE_AUTOMATA_HH 1

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

class NFA {

public:
  NFA();
  void defineStates();
  void defineFinalStates();
  void defineAlphabet();
  void defineTransitions();
  void showTransitions();
  void saveConfiguration();
  void loadConfiguration();
  void check(std::string str);
  void halt();

private:
  std::map<std::string, int>  _Q;      
  std::map<std::string, int>  _sigma;	 
  std::map<std::string, int>  _F;    
  std::string _q0;  

  bool _transitionsDefined;
  bool _configurationSaved;
  bool _configurationLoaded;
  bool _configurationChanged;
  bool _notDefinedWell();

  std::vector<std::string> _transitions[100][100];
  std::set<std::string> _finalStateSet;
  std::set<std::string> _deltaCapSet;
	
  unsigned _maxSubSet();
  unsigned _intersection(std::set<std::string>& s1, std::set<std::string>& s2);
  void     _deltaCap(std::string q, std::string wa);
  std::vector<std::string> _delta(int q, int w);
  std::vector<std::string> _tokenizeStates(std::string states);
};

#endif
