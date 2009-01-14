#ifndef BLOCKCHOOSER_HPP
#define BLOCKCHOOSER_HPP

#include "Block.hpp"
#include <deque>

namespace Bastet{

  class Well;
  //queue of blocks to appear on the screen
  typedef std::deque<BlockType> Queue;

  class BlockChooser{
  public:
    BlockChooser();
    virtual ~BlockChooser();
    virtual Queue GetStartingQueue()=0; //chooses first blocks
    virtual BlockType GetNext(const Well *well, const Queue &q)=0; //chooses next block
  private:
  };

  ///for testing purposes
  class RandomBlockChooser: public BlockChooser{
  public:
    RandomBlockChooser();
    virtual ~RandomBlockChooser();
    virtual Queue GetStartingQueue();
    virtual BlockType GetNext(const Well *well, const Queue &q);
  private:
  };

}

#endif //BLOCKCHOOSER_HPP
