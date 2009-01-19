/*
    Bastet - tetris clone with embedded bastard block chooser
    (c) 2005-2009 Federico Poloni <f.polonithirtyseven@sns.it> minus 37

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
