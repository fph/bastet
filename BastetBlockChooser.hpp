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

#ifndef BASTET_BLOCK_CHOOSER_HPP
#define BASTET_BLOCK_CHOOSER_HPP

#include "BlockChooser.hpp"

#include "Well.hpp"

#include <tr1/unordered_set>
#include <set>
#include <boost/functional/hash.hpp>

//boilerplate to use boost::hash as std::tr1::hash
namespace std{namespace tr1{
    template<> struct hash<Bastet::BlockPosition>{
      size_t operator()(const Bastet::BlockPosition &fb) const{
	static boost::hash<Bastet::BlockPosition> h;
	return h(fb);
      }
    };
  }}

namespace Bastet{

  static const int GameOverScore=-1000; //bogus score assigned to combinations which cause game over
  int Evaluate(const Well *w, int extralines); //assigns a score to a position w + a number of extra lines deleted while getting there

  typedef BlockPosition Vertex;

  class WellVisitor{
  public:
    WellVisitor(){}
    virtual void Visit(BlockType b, const Well *well, Vertex v){};
    virtual ~WellVisitor(){};
  };

  class RecursiveVisitor: public WellVisitor{
  public:
    RecursiveVisitor();
    virtual ~RecursiveVisitor();
    virtual void Visit(BlockType b, const Well *well, Vertex v);
    const boost::array<int,7> &GetScores() const{return _scores;}
  private:
    typedef boost::array<int,7> ScoresList;
    ScoresList _scores;
  };

  class BestScoreVisitor: public WellVisitor{
  public:
    explicit BestScoreVisitor(int bonusLines);
    virtual ~BestScoreVisitor();
    virtual void Visit(BlockType b, const Well *well, Vertex v);
    int GetScore(){return _score;}
  private:
    int _score;
    int _bonusLines;
  };

  /**
   * Tries to drop a block in all possible positions, and invokes the visitor on each one
   */
  class Searcher{
  public:
    Searcher(BlockType b, const Well *well, Vertex v, WellVisitor *visitor);
  private:
    std::tr1::unordered_set<Vertex> _visited;
    //std::set<Vertex> _visited; ^^ the above is more efficient, we need to do many inserts
    BlockType _block;
    const Well *_well;
    WellVisitor *_visitor;
    void DFSVisit(Vertex v);
  };

  class BastetBlockChooser: public BlockChooser{
  public:
    BastetBlockChooser();
    ~BastetBlockChooser();
    virtual Queue GetStartingQueue();
    virtual BlockType GetNext(const Well *well, const Queue &q);
    /**
     * computes "scores" of the candidate next blocks by dropping them in all possible positions and choosing the one that has the least max_(drop positions) Evaluate(well)
     */
    boost::array<int,7> ComputeMainScores(const Well *well, BlockType currentBlock);

  private:
    
  };

}

#endif //BASTET_BLOCK_CHOOSER_HPP
