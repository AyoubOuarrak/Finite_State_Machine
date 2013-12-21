/*
 *	HOW TO COMPILE:
 *  
 *  g++ -c e_NFA.cc
 *  g++ -c main.cc
 *  g++ main.o e_NFA.o -o e_NFA
 *  ./e_NFA
 *
 */

#include "e_NFA.hh"
#include "Menu.hh"
#include <string>
#include <iostream>

int main() {
  Menu menu("e_NFA");
  e_NFA M;
  std::string str;

  menu.addSubMenu(" 1 - Define states ");
  menu.addSubMenu(" 2 - Define final states ");
  menu.addSubMenu(" 3 - Define alphabet ");
  menu.addSubMenu(" 4 - Define transitions ");
  menu.addSubMenu(" 5 - Show transitions ");
  menu.addSubMenu(" 6 - Check string ");
  menu.addSubMenu(" 7 - Save e_NFA configuration");
  menu.addSubMenu(" 8 - Load e_NFA configuration");
  menu.addSubMenu(" 9 - Exit");

  while(true) {
    int index = menu.manageMenu();
    switch(index) {
    case 1 : M.defineStates(); break;
    case 2 : M.defineFinalStates(); break;
    case 3 : M.defineAlphabet(); break;
    case 4 : M.defineTransitions(); getch(); break;
    case 5 : M.showTransitions(); getch(); break;

    case 6 : std::cout << "string to check : ";
      std::cin >> str;
      M.check(str);
      getch();
      break;
			 			
    case 7 : M.saveConfiguration(); getch(); break;
    case 8 : M.loadConfiguration(); getch(); break;
    case 9 : M.halt(); getch(); 
    }
  }
}
