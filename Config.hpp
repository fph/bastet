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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>
#include <boost/array.hpp>

namespace Bastet{

  struct Keys{
    int Down;
    int Left;
    int Right;
    int RotateCW;
    int RotateCCW;
    int Drop;
    int Pause;
  };

  struct HighScore{
    int Score;
    std::string Scorer;
    bool operator < (const HighScore &b) const{
      return Score<b.Score;
    }
    bool operator ==(const HighScore &b) const{
      return (Score==b.Score) && (Scorer==b.Scorer);
    }
  };

  extern const size_t HowManyHighScores; //how many high scores to save in the file

  enum difficulty_t {difficulty_normal=0,difficulty_hard=1,num_difficulties=2};

  class HighScores: public std::vector<HighScore>{ //a set would not do the right job (think to ties)
  public:
    bool Qualifies(int score);
    int InsertHighScore(int score, const std::string &scorer); //returns position (1 to HowManyHighScores), -1 if you don't make into the list
  };

  extern const std::string RcFileName;
  extern const std::string LocalHighScoresFileName;
  extern const std::string GlobalHighScoresFileName;

  class Config{
  private:
    Keys _keys;
    boost::array<HighScores,num_difficulties> _hs;
  public:
    Config();
   ~Config();
    Keys *GetKeys();
    HighScores *GetHighScores(int difficulty);
    std::string GetConfigFileName() const;
    std::string GetHighScoresFileName() const;
  };

  extern Config config; //singleton
}

#endif //CONFIG_HPP
