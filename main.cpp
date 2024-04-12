#include "gzdml.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>

#include <stdio.h>
#include <conio.h>
#include <windows.h>   // WinApi header

enum class kb_key {
    enter = 13,
    esc = 27,
    up = 72,
    left = 75,
    right = 77,
    down = 80,

    none = 999,
};

//static inline bool operator==(int num, kb_key key) {
//    return num == static_cast<int>(key);
//}

using namespace std;

const int CONSOLE_COLOR = 2; // - dark green, 10 is bright green
const std::string GREETING_MESSAGE = "Choose Mod with [up/down] & Press [Enter]:"s;
const std::string INFO = "[Esc to exit]"s;


void print_label_on_curr_line(ostream& os, std::string_view label, size_t line_width) {
    cout << "\r" <<setw(line_width) << "[" + std::string(label) + "]" << flush;
}

int main() {
    GZDML modman;
    //set console text color
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, CONSOLE_COLOR);
    
    //TODO: Get max width from gzdml
    const size_t max_label_width = GREETING_MESSAGE.size() - 1;

    kb_key key = kb_key::none;

    cout << GREETING_MESSAGE << endl;
    cout << INFO << endl;

    while (key != kb_key::esc)
    {
        if (_kbhit()) {
            key = static_cast<kb_key>(_getch()); //cast char to key

            switch (key) {
            case kb_key::down:
                print_label_on_curr_line(cout, modman.SetPrevMod(), max_label_width);
                break;

            case kb_key::up:
                print_label_on_curr_line(cout, modman.SetNextMod(), max_label_width);
                break;

            case kb_key::enter:
                modman.LaunchGame();
                break;
            }
        }
    }

	return 0;
}



//clear console output:
/*
* conio.h
* 
* getch() - wait
* clrscr();
* 
* stdlib.h
* 
* system("cls"); - win
* system("clear"); - linux
* 
* 
* 
* 
* 
//CLEAR COUT LINE
#include <iostream>
#include <functional>
#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#define _isatty isatty
#define _fileno fileno
#endif

const std::function<void(const size_t&)> progress_printer(_isatty(_fileno(stdout)) == 1 ?
    [](const size_t& i) {
        std::cout << "\rNumber " << i << std::flush;
    } :
    [](const size_t& i) {
        static std::ios::off_type last(-1);
        if(last != -1)
            std::cout.seekp(last, std::ios::beg);
        last = std::cout.tellp();
        std::cout << "Number " << i << std::endl;
    }
);


int choice;

cout << "\n\t\tMENU\n";
cout << "\n1. Play\n";
cout << "2. Options\n;
cout << "3. Credits\n";
cout << "4. Help\n";
cout << "5. Exit\n";

cout << "\nEnter your choice: ";
cin >> choice;

switch(choice)
{
   case 1:{
              // code goes here
          } break;

   case 2:{
              // code goes here
          } break;

   case 3:{
              // code goes here
          } break;

   case 4:{
              // code goes here
          } break;

   case 5:{
              // code goes here
          } break;

   default : cout << "\nInvalid input!";
}


  HANDLE  hConsole;
    int k;

  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  // you can loop k higher to see more color choices
  for(k = 1; k < 255; k++)
  {
    // pick the colorattribute k you want
    SetConsoleTextAttribute(hConsole, k);
    cout << k << " I want to be nice today!" << endl;
  }

  cin.get(); // wait
  return 0;
*/