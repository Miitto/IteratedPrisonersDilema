#pragma once

#include <concepts>
#include <filesystem>
#include <string_view>
#include <variant>
#include <vector>

namespace cli {
  std::vector<std::string_view> get_cli_args(int argc,
                                             const char* const argv[]);

  enum class Payoff { T = 0, R = 1, P = 2, S = 3 };

  // TODO: Change this to a union of Strategy or double for RND's randomness.
  // Can use top bit as the flag since sign bit is irrelevent. Then remove RND
  // from this enum
  class Strategy {
  public:
    enum Simple { ALLC, ALLD, TFT, GRIM, PAVLOV, CONTRITE, PROBER };

  private:
    bool isRandom : 1;
    uint64_t bits : 63;

  public:
    Strategy(double randomness)
        : isRandom(true), bits(reinterpret_cast<uint64_t&>(randomness)) {}
    Strategy(Simple s) : isRandom(false), bits(static_cast<uint64_t>(s)) {}

    bool isRnd() const { return isRandom; }
    double rnd() const {
      uint64_t b = bits;
      double& r = reinterpret_cast<double&>(b);
      return r;
    }

    Simple simple() const { return static_cast<Simple>(bits); }
  };

  enum class Format { TEXT, JSON, CSV };

  std::ostream& operator<<(std::ostream& os, const std::vector<Payoff>& p);
  std::ostream& operator<<(std::ostream& os, const std::vector<Strategy>& s);
  std::ostream& operator<<(std::ostream& os, const Format f);

  class Args {
  public:
    uint32_t rounds;
    uint32_t repeats;
    double epsilon;
    uint32_t seed;
    std::vector<Payoff> enablePayoffs;
    std::vector<Strategy> strategies;
    Format format;
    std::filesystem::path savePath;
    std::filesystem::path loadFile;
    bool evolve;
    uint32_t population;
    uint32_t generations;
    double mutationRate;

    static std::variant<Args, std::tuple<std::string, uint32_t>>
    fromArgs(const std::vector<std::string_view> args);

    friend std::ostream& operator<<(std::ostream& os, const Args& args);
  };
} // namespace cli