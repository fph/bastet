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
  int Evaluate(const Well *w, int extralines){
    //high=good for player
    //lines
    int score=100000*extralines;

    //sums "free" height of each column (penalizes "holes")
    std::bitset<WellWidth> occupied;
    occupied.reset();
    BOOST_FOREACH(WellLine l,w->_well){
      occupied = occupied & l;
      score+=(WellWidth-occupied.count());
    }
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

  boost::array<int,7> BastetBlockChooser::ComputeMainScores(const Well *well, BlockType currentBlock){
    RecursiveVisitor visitor;
    Searcher(currentBlock,well,BlockPosition(),&visitor);
    return visitor.GetScores();
  }


  BlockType BastetBlockChooser::GetNext(const Well *well, const Queue &q){
    boost::array<int,7> mainScores=ComputeMainScores(well,q.front());
    boost::array<int,7> finalScores=mainScores;

    //the mainScores alone would give rise to many repeated blocks (e.g., in the case in which only one type of block does not let you clear a line, you keep getting that). This is bad, since it would break the "plausibility" of the sequence you get. We need a correction.

    //old "stupid" algorithm -- TODO: something more elaborate would be advisable

    //perturbes scores to randomize tie handling
    BOOST_FOREACH(int &i, finalScores)
      i+=(random()%32);
    
    //finds which block we want
    static const boost::array<int,7> blockPercentages={{75, 92, 98, 100, 100, 100, 100}};
    int pos=find_if(blockPercentages.begin(),blockPercentages.end(),bind2nd(greater_equal<int>(),random()%100)) - blockPercentages.begin();
    assert(pos>=0 && pos<7);

    //finds index of the pos-th element (in increasing order) of the vector
    //dumb algorithm, always returns the first in case of ties, but we don't care because of the randomization
    boost::array<int,7> temp(finalScores);
    nth_element(temp.begin(),temp.begin()+pos,temp.end());
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
      int thisscore=Evaluate(&w2,linescleared+_bonusLines);
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

}
