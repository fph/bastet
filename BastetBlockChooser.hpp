#ifndef BASTET_BLOCK_CHOOSER_HPP
#define BASTET_BLOCK_CHOOSER_HPP

#include "BlockChooser.hpp"

namespace Bastet{
  class BastetBlockChooser: public BlockChooser{
  public:
    BastetBlockChooser();
    ~BastetBlockChooser();
    virtual StartingSet ChooseStartingSet();
    virtual BlockType Choose(Well *well, BlockType currentBlock);
  private:
  };

}

#endif //BASTET_BLOCK_CHOOSER_HPP
