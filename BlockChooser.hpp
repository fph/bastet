#ifndef BLOCKCHOOSER_HPP
#define BLOCKCHOOSER_HPP

#include "Block.hpp"
#include <utility>

namespace Bastet{

  class Well;
  typedef std::pair<BlockType,BlockType> StartingSet;

  class BlockChooser{
  public:
    BlockChooser();
    virtual ~BlockChooser();
    virtual StartingSet ChooseStartingSet()=0; //chooses first and second block
    virtual BlockType Choose(const Well *well, BlockType current)=0; //chooses next block
  private:
  };

  ///for testing purposes
  class RandomBlockChooser: public BlockChooser{
  public:
    RandomBlockChooser();
    virtual ~RandomBlockChooser();
    virtual StartingSet ChooseStartingSet(); //chooses first and second block
    virtual BlockType Choose(const Well *well, BlockType current); //chooses next block    
  private:
  };

}

#endif //BLOCKCHOOSER_HPP
