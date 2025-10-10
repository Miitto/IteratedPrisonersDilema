#include "cli.hpp"

namespace cli {
  RGBColor getStrategyColor(cli::Strategy s) {
    using Simple = cli::Strategy::Simple;
    if (s.isRnd()) {
      return RGBColor(255, s.rnd() * 255, 128);
    }
    switch (s.simple()) {
    case Simple::ALLC:
      return RGBColor(50, 255, 50); // Green
    case Simple::ALLD:
      return RGBColor(255, 50, 50); // Red
    case Simple::TFT:
      return RGBColor(100, 100, 255); // Blue
    case Simple::GRIM:
      return RGBColor(255, 165, 50); // Orange
    case Simple::PAVLOV:
      return RGBColor(255, 255, 50); // Yellow
    case Simple::CONTRITE:
      return RGBColor(50, 255, 255); // Cyan
    case Simple::PROBER:
      return RGBColor(128, 50, 128); // Purple
    case Simple::FGRIM:
      return RGBColor(255, 192, 203); // Pink
    case Simple::SPROBER:
      return RGBColor(165, 42, 42); // Brown
    default:
      return RGBColor(255, 255, 255); // White for unknown strategies
    }
  }

  std::ostream& operator<<(std::ostream& os, const Payoffs& p) {
    os << "{T: " << p.temptation << ", R: " << p.reward
       << ", P: " << p.punishment << ", S: " << p.sucker << "}";
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const Strategy s) {
    std::stringstream ss;

    if (s.isRnd()) {
      ss << "RND" << s.rnd();
    } else {
      auto simple = s.simple();
      switch (simple) {
      case Strategy::ALLC:
        ss << "ALLC";
        break;
      case Strategy::ALLD:
        ss << "ALLD";
        break;
      case Strategy::TFT:
        ss << "TFT";
        break;
      case Strategy::GRIM:
        ss << "GRIM";
        break;
      case Strategy::PAVLOV:
        ss << "PAVLOV";
        break;
      case Strategy::CONTRITE:
        ss << "CONTRITE";
        break;
      case Strategy::PROBER:
        ss << "PROBER";
        break;
      case Strategy::FGRIM:
        ss << "FGRIM";
        break;
      case Strategy::SPROBER:
        ss << "SPROBER";
        break;
      }
    };

    os << ss.str();

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
       << "enableBudget: " << (args.enableBudget ? "true" : "false") << "\n"
       << "verbose: " << (args.verbose ? "true" : "false") << "\n";
    return os;
  }

} // namespace cli