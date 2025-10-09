#include "cli.hpp"

namespace cli {
  std::ostream& operator<<(std::ostream& os, const Payoffs& p) {
    os << "{T: " << p.temptation << ", R: " << p.reward
       << ", P: " << p.punishment << ", S: " << p.sucker << "}";
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const Strategy s) {
    if (s.isRnd()) {
      std::stringstream ss;
      ss << "RND" << s.rnd();
      os << ss.str();
    } else {
      auto simple = s.simple();
      switch (simple) {
      case Strategy::ALLC:
        os << "ALLC";
        break;
      case Strategy::ALLD:
        os << "ALLD";
        break;
      case Strategy::TFT:
        os << "TFT";
        break;
      case Strategy::GRIM:
        os << "GRIM";
        break;
      case Strategy::PAVLOV:
        os << "PAVLOV";
        break;
      case Strategy::CONTRITE:
        os << "CONTRITE";
        break;
      case Strategy::PROBER:
        os << "PROBER";
        break;
      case Strategy::FGRIM:
        os << "FGRIM";
        break;
      case Strategy::SPROBER:
        os << "SPROBER";
        break;
      }
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const std::vector<Strategy>& s) {
    os << "[";
    for (size_t i = 0; i < s.size(); ++i) {
      os << s[i];
      if (i < s.size() - 1) {
        os << ", ";
      }
    }
    os << "]";
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const Format f) {
    switch (f) {
    case Format::TEXT:
      os << "TEXT";
      break;
    case Format::JSON:
      os << "JSON";
      break;
    case Format::CSV:
      os << "CSV";
      break;
    default:
      os << "UNKNOWN";
      break;
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const Args& args) {
    os << "rounds: " << args.rounds << "\n"
       << "repeats: " << args.repeats << "\n"
       << "epsilon: " << args.epsilon << "\n"
       << "seed: " << args.seed << "\n"
       << "payoffs: " << args.payoffs << "\n"
       << "strategies: " << args.strategies << "\n"
       << "format: " << args.format << "\n"
       << "savePath: " << args.savePath.string() << "\n"
       << "loadFile: " << args.loadFile.string() << "\n"
       << "evolve: " << (args.evolve ? "true" : "false") << "\n"
       << "population: " << args.population << "\n"
       << "generations: " << args.generations << "\n"
       << "mutationRate: " << args.mutationRate << "\n"
       << "verbose: " << (args.verbose ? "true" : "false") << "\n";
    return os;
  }

} // namespace cli