#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/program_options.hpp>
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
  };

  extern const size_t HowManyHighScores; //how many high scores to save in the file
  extern const std::string RcFileName;
  extern const std::string LocalHighScoresFileName;
  extern const std::string GlobalHighScoresFileName;

  typedef std::vector<HighScore> HighScores;

  class Config{
  private:
    boost::program_options::variables_map _options;
    boost::program_options::variables_map _highScores;
  public:
    Config();
   ~Config();
    Keys GetKeys();
    void SetKeys(const Keys &k);
    HighScores GetHighScores();
    int PushHighScore(const std::string &scorer, int score); //returns position, -1 if it did not make into the high scorers
  };

  extern Config config; //singleton
}

#endif //CONFIG_HPP
