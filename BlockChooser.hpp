#ifndef BLOCKCHOOSER_HPP
#define BLOCKCHOOSER_HPP

#include <utility>

namespace Bastet{
  class Block;
  class Well;
  typedef std::pair<Block*,Block*> StartingSet;

  class BlockChooser{
  public:
    BlockChooser();
    virtual ~BlockChooser();
    virtual StartingSet ChooseStartingSet()=0; //chooses first and second block
    virtual Block* Choose(Well *well, Block *current)=0; //chooses next block
  private:
  };

  ///for testing purposes
  class RandomBlockChooser: public BlockChooser{
  public:
    RandomBlockChooser();
    virtual ~RandomBlockChooser();
    virtual StartingSet ChooseStartingSet(); //chooses first and second block
    virtual Block* Choose(Well *well, Block *current); //chooses next block    
  private:
  };

}

#endif //BLOCKCHOOSER_HPP
