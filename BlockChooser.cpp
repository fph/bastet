#include "BlockChooser.hpp"

#include "Block.hpp"
#include <cstdlib> //random

namespace Bastet{

  BlockChooser::BlockChooser(){}
  BlockChooser::~BlockChooser(){}

  RandomBlockChooser::RandomBlockChooser(){}
  RandomBlockChooser::~RandomBlockChooser(){}
  StartingSet RandomBlockChooser::ChooseStartingSet(){
    return std::make_pair(&blocks[random()%7],&blocks[random()%7]);
  }
  Block *RandomBlockChooser::Choose(Well *well, Block *next){
    return &blocks[random()%7];
  }
  
}
