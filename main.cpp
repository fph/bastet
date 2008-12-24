#include "Ui.hpp"

using namespace Bastet;

int main(int argc, char **argv){
  Ui ui(argc,argv);
  ui.ChooseLevel();
  ui.Play();
}
