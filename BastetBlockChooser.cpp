#include "BastetBlockChooser.hpp"

#include "Block.hpp"
#include <cstdlib>

namespace Bastet{
  BastetBlockChooser::BastetBlockChooser(){

  }
  
  BastetBlockChooser::~BastetBlockChooser(){

  }

  StartingSet BastetBlockChooser::ChooseStartingSet(){
    return std::make_pair(&blocks[random()%7],&blocks[random()%7]);
  }

  Block *Choose(Well *well, Block* currentBlock){
    return &blocks[random()%7];
  }

}
