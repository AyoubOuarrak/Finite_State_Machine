/*
 *		Author  : Ouarrak Ayoub
 *		Date    : 04/12/2013
 *		version : v1.5
 *
 *		DFA = (Q, sigma, delta, q0, F)
 *
 *		Q     : finite set of states
 *		sigma : alphabet
 *		delta : transition function  delta : (Q x sigma) --> Q 
 *		q0    : first state
 *		F     : set of final states  F (subset of) Q	
 */
#ifndef DETERMINISTIC_FINITE_AUTOMATA_HH
#define DETERMINISTIC_FINITE_AUTOMATA_HH 1

#include <iostream>
#include <string>
#include <map>

class DFA {

public:
  DFA();
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
  // The key is a string, in this case the key rappresent
  // the symbol or the single state, the integer rappresent
  // a number that we use later to access the transistions table
  std::map<std::string, int> _Q;      
  std::map<std::string, int> _sigma;	 
  std::map<std::string, int> _F;      

  bool _transitionsDefined;
  bool _configurationSaved;
  bool _configurationLoaded;
  bool _configurationDfaChanged;

  std::string _q0;  
  std::string _transitions[100][100];

  // delta 
  // input:  current state, a   (a := symbol) 
  // output: next state 

  // deltaCap 
  // input:  current state, wa   (wa := string)
  // output: delta(deltaCap(current, w), a)

  std::string _delta(int q, int w);
  std::string _deltaCap(std::string q, std::string wa);
  bool _notDefinedWell();
};

#endif /* DETERMINISTIC_FINITE_AUTOMATA_HH */
