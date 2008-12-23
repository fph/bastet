#ifndef BLOCKCHOOSER_HPP
#define BLOCKCHOOSER_HPP

namespace Bastet{

  class BlockChooser{
  public:
    BlockChooser();
    virtual ~BlockChooser();
    Block* Choose(Well *well, Block *next);
  private:
  };

  class RandomBlockChooser{
  public:
    
  private:
  };

}

#endif //BLOCKCHOOSER_HPP
