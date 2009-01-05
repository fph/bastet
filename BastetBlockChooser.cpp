#include "BastetBlockChooser.hpp"

#include "Block.hpp"
#include <cstdlib>

namespace Bastet{
  BastetBlockChooser::BastetBlockChooser(){

  }
  
  BastetBlockChooser::~BastetBlockChooser(){

  }

  StartingSet BastetBlockChooser::ChooseStartingSet(){
    return std::make_pair(BlockType(random()%7),BlockType(random()%7));
  }

  BlockType Choose(Well *well, BlockType currentBlock){
    return BlockType(random()%7);
  }

}
