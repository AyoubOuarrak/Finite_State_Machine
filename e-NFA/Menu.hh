/*
 *     Author: Ouarrak Ayoub
 *     Date: 2012/2013
 */

#ifndef GUARD_CLASS_MENU_HH
#define GUARD_CLASS_MENU_HH 1

#include <string>
#include <unistd.h>   
#include <termios.h>  
#include <cstdlib>
#include <iostream>
#include <stdio.h>      

#ifdef WIN32
#   define clearScreen() (system("cls"))
#else                        
#   define clearScreen() (system("clear"))
#endif

#define useGreyColor "\x1b[90m"
#define useWhiteColor "\x1b[0m"

char getch() {
  char buf=0;
  struct termios old={0};
  fflush(stdout);
  if(tcgetattr(0, &old)<0)
    perror("tcsetattr()");
  old.c_lflag&=~ICANON;
  old.c_lflag&=~ECHO;
  old.c_cc[VMIN]=1;
  old.c_cc[VTIME]=0;
  if(tcsetattr(0, TCSANOW, &old)<0)
    perror("tcsetattr ICANON");
  if(read(0,&buf,1)<0)
    perror("read()");
  old.c_lflag|=ICANON;
  old.c_lflag|=ECHO;
  if(tcsetattr(0, TCSADRAIN, &old)<0)
    perror ("tcsetattr ~ICANON");
  printf("%c\n",buf);
  return buf;
}

/***********************************************************/
/******************* CLASS DEFINITION **********************/ 
/***********************************************************/
struct SubMenu {
  int idSubMenu;
  std::string   titleSubMenu;
  SubMenu* nextSubMenu;
  SubMenu* prevSubMenu;
};

class Menu {

private:
  SubMenu* subMenu;
  SubMenu* lastSubMenu();
  int lenghtMenu;
  int indexSubMenu;

public:
  Menu();
  explicit Menu(std::string title);

  void addSubMenu(std::string title);
  void removeSubMenu(std::string title);
  int manageMenu();
  void printMenu(int index);

};

/***********************************************************/
/******************* MEMBER FUNCTIONS **********************/ 
/***********************************************************/

Menu::Menu() {
  lenghtMenu = 0;
  subMenu = new SubMenu;
  subMenu->idSubMenu = lenghtMenu;
  subMenu->titleSubMenu = "";
  subMenu->nextSubMenu = NULL;
  subMenu->prevSubMenu = NULL;
  indexSubMenu = 1;
}

Menu::Menu(std::string title) {
  lenghtMenu = 0;
  subMenu = new SubMenu;

  subMenu->idSubMenu = lenghtMenu;
  subMenu->titleSubMenu = title;
  subMenu->nextSubMenu = NULL;
  subMenu->prevSubMenu = subMenu;
  indexSubMenu = 1;
}

void Menu::addSubMenu(std::string title) {
  SubMenu* tmp = new SubMenu;
  lastSubMenu()->nextSubMenu = tmp;
  tmp->prevSubMenu = lastSubMenu();
  tmp->nextSubMenu = NULL;
  tmp->idSubMenu = ++lenghtMenu;
  tmp->titleSubMenu = title;
}

void Menu::printMenu(int index) {
  SubMenu* tmp = subMenu;
  for(int i = 0; i < 79; ++i)
    std::cout << "_";

  std::cout << std::endl << "\t\t";
  while(tmp != NULL){ 
    std::cout << useGreyColor;
    if(tmp->idSubMenu == index) {
      std::cout << useWhiteColor;
      std::cout << "---> \t" <<" "<< tmp->titleSubMenu <<" "<< "\t <---" << std::endl; 
    }
    else
      std::cout << "\t" << tmp->titleSubMenu << std::endl; 
    tmp = tmp->nextSubMenu; 
  }
  for(int i = 0; i < 79; ++i)
    std::cout << "_";
  std::cout << std::endl;
}

SubMenu* Menu::lastSubMenu() {
  SubMenu* tmp = subMenu;
  while(tmp->nextSubMenu != NULL)
    tmp = tmp->nextSubMenu;
  return tmp;
}

int Menu::manageMenu() {
  char input;
  //int indexSubMenu = 1;
    
  do {
    clearScreen();  
    printMenu(indexSubMenu);
    input = getch();

    switch(input) {
    case 65 : {     //freccia su
      if(indexSubMenu > 1) 
	indexSubMenu--;
      else
	indexSubMenu = lenghtMenu;
    } break;

    case 66 : {     //freccia giù
      if(indexSubMenu < lenghtMenu) 
	indexSubMenu++;
      else
	indexSubMenu = 1;
    } break;
            
    case '\n' : {   //invio
      std::cout << useWhiteColor;
      return indexSubMenu;
    } break;
    } 
  } while(true);  
}

#endif
