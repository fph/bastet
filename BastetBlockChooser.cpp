#include "BastetBlockChooser.hpp"

#include "Block.hpp"

#include <boost/functional/hash.hpp>
#include <tr1/unordered_set>
#include <cstdlib>

//debug
#include <fstream>
#include <iostream>
#include <iomanip>
#include <curses.h>

using namespace std;

namespace Bastet{
  int Evaluate(const Well *w, int extralines){
    //TODO:STUB
    return 100*extralines;
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

  BlockType BastetBlockChooser::Choose(const Well *well, BlockType currentBlock){
#if 0
    BestScoreVisitor visitor(0);
    int minscore=GameOverScore;
    BlockType minpos=O;
    for(int i=0;i<7;++i){
      Searcher(well,FallingBlock(BlockType(i),*well),&visitor);
      if(visitor.GetScore()<minscore){
	minscore=visitor.GetScore();
	minpos=BlockType(i);
      }
    }
    return minpos;
#endif

#if 0
    RecursiveVisitor visitor;
    Searcher(well,FallingBlock(currentBlock,*well),&visitor);
    return BlockType(min_element(visitor.GetScores().begin(),visitor.GetScores().end())-visitor.GetScores().begin());
#endif
    return BlockType(random()%7);
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
#if 0
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
#endif
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
