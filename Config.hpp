#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>

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
    HighScores _hs;
  public:
    Config();
   ~Config();
    Keys *GetKeys();
    HighScores *GetHighScores();
    std::string GetConfigFileName() const;
    std::string GetHighScoresFileName() const;
  };

  extern Config config; //singleton
}

#endif //CONFIG_HPP
