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

#include "BastetBlockChooser.hpp"
#include "Block.hpp"

#include <boost/functional/hash.hpp>
#include <boost/foreach.hpp>
#include <tr1/unordered_set>
#include <cstdlib>
#include <algorithm>

//debug
#include <fstream>
#include <iostream>
#include <iomanip>
#include <curses.h>

using namespace std;
using namespace boost;

namespace Bastet{
  long Evaluate(const Well *w, int extralines){
    //high=good for the player
    //lines
    long score=100000000*extralines;

    //adds a bonus for each "free" dot above the occupied blocks profile
    std::bitset<WellWidth> occupied;
    occupied.reset();
    BOOST_FOREACH(WellLine l,w->_well){
      occupied = occupied & l;
      score+=10000*(WellWidth-occupied.count());
    }

    //adds a bonus for lower max height of the occupied blocks
    int height=RealWellHeight;
    BOOST_FOREACH(WellLine l, w->_well){
      if(l.any()) break;
      height--;
    }
    score+= 1000 * (RealWellHeight-height);
    return score;
  }

  BastetBlockChooser::BastetBlockChooser(){
  }
  
  BastetBlockChooser::~BastetBlockChooser(){

  }

  Queue BastetBlockChooser::GetStartingQueue(){
    Queue q;
    //The first block is always I,J,L,T (cfr. Tetris guidelines, Bastet is a gentleman).
    BlockType first;
    switch(random()%4){
    case 0:
      first=I;break;
    case 1:
      first=J;break;
    case 2:
      first=L;break;
    case 3:
      first=T;break;
    }
    q.push_back(first);
    q.push_back(BlockType(random()%7));
    return q;
  }

  boost::array<long,7> BastetBlockChooser::ComputeMainScores(const Well *well, BlockType currentBlock){
    RecursiveVisitor visitor;
    Searcher(currentBlock,well,BlockPosition(),&visitor);
    return visitor.GetScores();
  }


  BlockType BastetBlockChooser::GetNext(const Well *well, const Queue &q){
    boost::array<long,7> mainScores=ComputeMainScores(well,q.front());
    boost::array<long,7> finalScores=mainScores;

    //perturbes scores to randomize tie handling
    BOOST_FOREACH(long &i, finalScores)
      i+=(random()%100);

    //prints the final scores, for debugging convenience
    for(int i=0;i<7;++i){
      //mvprintw(i,1,"%c: %d",GetChar(BlockType(i)),finalScores[i]);
    }

    //the mainScores alone would give rise to many repeated blocks (e.g., in the case in which only one type of block does not let you clear a line, you keep getting that). This is bad, since it would break the "plausibility" of the sequence you get. We need a correction.
    
    boost::array<long,7> temp(finalScores);
    sort(temp.begin(),temp.end());

    //always returns the worst block if it's different from the last one
    int worstblock=find(finalScores.begin(),finalScores.end(),temp[0])-finalScores.begin();
    if(BlockType(worstblock) != q.front()) return BlockType(worstblock);
    
    //otherwise, returns the pos-th block, where pos is random
    static const boost::array<int,7> blockPercentages={{80, 92, 98, 100, 100, 100, 100}};
    int pos=find_if(blockPercentages.begin(),blockPercentages.end(),bind2nd(greater_equal<int>(),random()%100)) - blockPercentages.begin();
    assert(pos>=0 && pos<7);

    int chosenBlock=find(finalScores.begin(),finalScores.end(),temp[pos])-finalScores.begin();
    return BlockType(chosenBlock);
    
    //return BlockType(min_element(finalScores.begin(),finalScores.end())-finalScores.begin());
    //return BlockType(random()%7);
  }

  Searcher::Searcher(BlockType b, const Well *well, Vertex v, WellVisitor *visitor):_block(b),_well(well),_visitor(visitor){
    DFSVisit(v);
  }

  void Searcher::DFSVisit(Vertex v){
    if(_visited.insert(v).second==false) return; //already visited

    for(int i=0;i<5;++i){
      Vertex v2(v);
      if(v2.MoveIfPossible(Movement(i),_block,_well))
	DFSVisit(v2);
      else{
	if(Movement(i)==Down) //block may lock here
	  _visitor->Visit(_block,_well,v);
      }
    }
  }

  BestScoreVisitor::BestScoreVisitor(int bonusLines):_score(GameOverScore),_bonusLines(bonusLines){};
  BestScoreVisitor::~BestScoreVisitor(){};
  void BestScoreVisitor::Visit(BlockType b, const Well *w, Vertex v){
    Well w2(*w); //copy
    try{
      int linescleared=w2.LockAndClearLines(b,v);
      long thisscore=Evaluate(&w2,linescleared+_bonusLines);
      _score=max(_score,thisscore);
    }catch(const GameOver &go){}
  }

  RecursiveVisitor::RecursiveVisitor(){_scores.assign(GameOverScore);}
  RecursiveVisitor::~RecursiveVisitor(){}
  void RecursiveVisitor::Visit(BlockType b, const Well *w, Vertex v){
    Well w2(*w); //copy
    try{
      int linescleared=w2.LockAndClearLines(b,v); //may throw GO
      for(int i=0;i<7;++i){
	try{
	  BestScoreVisitor visitor(linescleared);
	  BlockPosition p;
	  if(!p.IsValid(BlockType(i),&w2)) throw(GameOver());
	  Searcher searcher(BlockType(i),&w2,p,&visitor);
	  _scores[i]=max(_scores[i],visitor.GetScore());
	} catch(const GameOver &go){}
      }
    } catch(const GameOver &go){} //catches the exception which might be thrown by LockAndClearLines
  }

  NoPreviewBlockChooser::NoPreviewBlockChooser(){};
  NoPreviewBlockChooser::~NoPreviewBlockChooser(){};
  Queue NoPreviewBlockChooser::GetStartingQueue(){
    Queue q;
    //The first block is always I,J,L,T (cfr. Tetris guidelines, Bastet is a gentleman).
    BlockType first;
    switch(random()%4){
    case 0:
      first=I;break;
    case 1:
      first=J;break;
    case 2:
      first=L;break;
    case 3:
      first=T;break;
    }
    q.push_back(first);
    return q;
  }

  BlockType NoPreviewBlockChooser::GetNext(const Well *well, const Queue &q){
    assert(q.empty());
    boost::array<long,7> finalScores;
    for(int t=0;t<7;++t){
      BestScoreVisitor v;
      Searcher searcher(BlockType(t),well,BlockPosition(),&v);
      finalScores[t]=v.GetScore();
    }

    //perturbes scores to randomize tie handling
    BOOST_FOREACH(long &i, finalScores)
      i+=(random()%100);

    //sorts
    boost::array<long,7> temp(finalScores);
    sort(temp.begin(),temp.end());

    //returns the pos-th block, where pos is random
    static const boost::array<int,7> blockPercentages={{80, 92, 98, 100, 100, 100, 100}};
    int pos=find_if(blockPercentages.begin(),blockPercentages.end(),bind2nd(greater_equal<int>(),random()%100)) - blockPercentages.begin();
    assert(pos>=0 && pos<7);

    int chosenBlock=find(finalScores.begin(),finalScores.end(),temp[pos])-finalScores.begin();
    return BlockType(chosenBlock);

  }

}
