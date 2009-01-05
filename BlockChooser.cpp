#include "BlockChooser.hpp"

#include <cstdlib> //random

namespace Bastet{

  BlockChooser::BlockChooser(){}
  BlockChooser::~BlockChooser(){}

  RandomBlockChooser::RandomBlockChooser(){}
  RandomBlockChooser::~RandomBlockChooser(){}
  StartingSet RandomBlockChooser::ChooseStartingSet(){
    return std::make_pair(BlockType(random()%7),BlockType(random()%7));
  }
  BlockType RandomBlockChooser::Choose(const Well *well, BlockType next){
    return BlockType(random()%7);
  }
  
}
