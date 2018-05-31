/*
    Bastet - tetris clone with embedded bastard block chooser
    (c) 2005-2009 Federico Poloni <f.polonithirtyseven@sns.it> minus 37

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Ui.hpp"
#include "Config.hpp"
#include "BastetBlockChooser.hpp"
#include <boost/assign.hpp>

//DBG
#include <iostream>
#include <boost/format.hpp>

#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#define _(STRING) gettext(STRING)

using namespace Bastet;
using namespace std;
using namespace boost;
using namespace boost::assign;


int main(int argc, char **argv){

setlocale(LC_ALL, "");
bindtextdomain("main", "/usr/share/locale");
textdomain("main");

  Ui ui;
  while(1){
    
    int choice=ui.MenuDialog(list_of(_("Play! (normal version)"))(_("Play! (harder version)"))(_("View highscores"))(_("Customize keys"))(_("Quit")));  
    
    switch(choice){
    case 0:{
      //ui.ChooseLevel();
      BastetBlockChooser bc;
      ui.Play(&bc);
      ui.HandleHighScores(difficulty_normal);
      ui.ShowHighScores(difficulty_normal);
    }
      break;
    case 1:{
      //ui.ChooseLevel();
      NoPreviewBlockChooser bc;
      ui.Play(&bc);
      ui.HandleHighScores(difficulty_hard);
      ui.ShowHighScores(difficulty_hard);
    }
      break;
    case 2:
      ui.ShowHighScores(difficulty_normal);
      ui.ShowHighScores(difficulty_hard);
      break;
    case 3:
      ui.CustomizeKeys();
      break;
    case 4:
      exit(0);
      break;
    }
  }
}
