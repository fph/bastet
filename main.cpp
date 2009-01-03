#include "Ui.hpp"
#include "Config.hpp"
#include <boost/assign.hpp>

//DBG
#include <iostream>
#include <boost/format.hpp>

using namespace Bastet;
using namespace std;
using namespace boost;
using namespace boost::assign;


int main(int argc, char **argv){
  Ui ui;
  while(1){
    
    int choice=ui.MenuDialog(list_of("Play!")("View highscores")("Customize keys")("Quit"));
    switch(choice){
    case 0:
      ui.ChooseLevel();
      ui.Play();
      ui.HandleHighScores();
      ui.ShowHighScores();
      break;
    case 1:
      ui.ShowHighScores();
      break;
    case 2:
      ui.CustomizeKeys();
      break;
    case 3:
      exit(0);
      break;
    }
  }
}
