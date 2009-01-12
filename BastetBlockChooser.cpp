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
    for(int i=0;i<WellWidth;++i){
      Dot d;
      for(d=(Dot){i,-2}; d.y<WellHeight;d.y++){
	if((*w)(d)!=0) break;
      }
      score+=100*d.y;
    }

    return score;
  }

  BastetBlockChooser::BastetBlockChooser(){
  }
  
  BastetBlockChooser::~BastetBlockChooser(){

  }

  StartingSet BastetBlockChooser::ChooseStartingSet(){
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
    return make_pair(first,BlockType(random()%7));
  }

  boost::array<int,7> BastetBlockChooser::ComputeMainScores(const Well *well, BlockType currentBlock){
    RecursiveVisitor visitor;
    Searcher(well,FallingBlock(currentBlock,*well),&visitor);
    return visitor.GetScores();
  }


  BlockType BastetBlockChooser::Choose(const Well *well, BlockType currentBlock){
    boost::array<int,7> mainScores=ComputeMainScores(well,currentBlock);
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

  Searcher::Searcher(const Well *well, Vertex v, WellVisitor *visitor):_well(well),_visitor(visitor){
    DFSVisit(v);
  }

  void Searcher::DFSVisit(Vertex v){
    if(_visited.insert(v).second==false) return; //already visited
    
    //if(_visited.size()>2) return;

    //there is some code replication here, I don't know if it is worth refactoring
    {
      Vertex v2(v);
      if(v2.MoveDown(*_well))
	DFSVisit(v2);
      else{ //block may lock here
	_visitor->Visit(_well,v);
      }
    }
    {
      Vertex v2(v);
      if(v2.MoveLeft(*_well))
	DFSVisit(v2);
    }
    {
      Vertex v2(v);
      if(v2.MoveRight(*_well))
	DFSVisit(v2);
    }
    {
      Vertex v2(v);
      if(v2.RotateCW(*_well))
	DFSVisit(v2);
    }
    {
      Vertex v2(v);
      if(v2.RotateCCW(*_well))
	DFSVisit(v2);
    }
  }

  BestScoreVisitor::BestScoreVisitor(int bonusLines):_score(GameOverScore),_bonusLines(bonusLines){};
  BestScoreVisitor::~BestScoreVisitor(){};
  void BestScoreVisitor::Visit(const Well *w, Vertex v){
    Well w2(*w); //copy
    try{
      int linescleared=w2.LockAndClearLines(v);
      int thisscore=Evaluate(&w2,linescleared+_bonusLines);
      _score=max(_score,thisscore);
    }catch(const GameOver &go){}
  }

  RecursiveVisitor::RecursiveVisitor(){_scores.assign(GameOverScore);}
  RecursiveVisitor::~RecursiveVisitor(){}
  void RecursiveVisitor::Visit(const Well *w, Vertex v){
    Well w2(*w); //copy
    try{
      int linescleared=w2.LockAndClearLines(v);
      for(int i=0;i<7;++i){
	try{
	  FallingBlock fb(BlockType(i),w2);
	  BestScoreVisitor visitor(linescleared);
	  Searcher searcher(&w2,fb,&visitor);
	  _scores[i]=max(_scores[i],visitor.GetScore());
	} catch(const GameOver &go){} //catches the one thrown by FB constructor
      }
    } catch(const GameOver &go){} //catches the exception which might be thrown by LockAndClearLines
  }

}
