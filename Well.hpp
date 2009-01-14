#ifndef WELL_HPP
#define WELL_HPP

#include "Block.hpp" //for Color
#include "BlockPosition.hpp"
#include <cstddef> //size_t
#include <vector>
#include <bitset>
#include <boost/array.hpp>

namespace Bastet{
  
  class GameOver{}; //used as an exception

  class WellLine: public boost::array<bool,WellWidth>{

  };

  /*
   * the real height of the well is _height+2, with the top two rows( -1 and -2) hidden (see guidelines)
   */
  class Well{
  private:
    boost::array<WellLine,RealWellHeight> _well;
  public:
    Well();
    ~Well();
    void Clear();
    bool Accomodates(const DotMatrix &d) const; //true if the given tetromino fits into the well
    bool IsLineComplete(int y) const;
    std::vector<int> Lock(BlockType t, const BlockPosition &p); //permanently adds a tetromino to the well; returns a bitset of 4 bits where return[i]==1 iff line (start of fb)+i is complete
    void ClearLines(const std::vector<int> &completed); //removes the given lines from the well (whether they are completed or not)
    int LockAndClearLines(BlockType t, const BlockPosition &p); //locks, clear lines, returns number of lines cleared
  };

}

#endif //WELL_HPP
