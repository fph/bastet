#include "Block.hpp"
#include "Well.hpp"

namespace Bastet{
  
  ///position = well state + current block position & orientation + next block
  class Position{
  public:
    Block *GetNextBlock() const;
    bool RotateCW(); //returns false if it could not
    bool RotateCCW();
    bool MoveLeft();
    bool MoveDown(); //returns false if it could not
    void Drop(); ///move down as much as you can
    Position(Well *well, Block *currentBlock, Block *nextBlock,size_t x, size_t y=0, size_t orientation=0);
    ~Position();
  private:
    Well *_well;
    Block *_currentBlock;
    Block *_nextBlock;
    size_t _x,_y;
    size_t _orientation;
  };

}
