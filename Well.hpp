#ifndef WELL_HPP
#define WELL_HPP

#include "Block.hpp" //for Color
#include "FallingBlock.hpp"
#include <cstddef> //size_t
#include <vector>
#include <bitset>

namespace Bastet{

  class FallingBlock; //forward
  
  class GameOver{}; //used as an exception

  class WellLine: public boost::array<Color,WellWidth>{

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
    const Color &operator()(const Dot &p) const;
    Color &operator()(const Dot &p);
    bool Accomodates(const DotMatrix &d) const; //true if the given tetromino fits into the well
    bool IsLineComplete(int y) const;
    std::vector<int> Lock(const FallingBlock &fb); //permanently adds a tetromino to the well; returns a bitset of 4 bits where return[i]==1 iff line (start of fb)+i is complete
    void ClearLines(const std::vector<int> &completed); //removes the given lines from the well (whether they are completed or not)
    int LockAndClearLines(const FallingBlock &fb); //locks, clear lines, returns number of lines cleared
  };

}

#endif //WELL_HPP
