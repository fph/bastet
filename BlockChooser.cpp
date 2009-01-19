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
