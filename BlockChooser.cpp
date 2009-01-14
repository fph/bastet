#include "BlockChooser.hpp"

#include <cstdlib> //random

namespace Bastet{

  BlockChooser::BlockChooser(){}
  BlockChooser::~BlockChooser(){}

  RandomBlockChooser::RandomBlockChooser(){}
  RandomBlockChooser::~RandomBlockChooser(){}
  Queue RandomBlockChooser::GetStartingQueue(){
    Queue q;
    q.push_back(BlockType(random()%7));
    q.push_back(BlockType(random()%7));
    return q;
  }
  BlockType RandomBlockChooser::GetNext(const Well *well, const Queue &q){
    return BlockType(random()%7);
  }
  
}
