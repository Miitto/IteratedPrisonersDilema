#include "cli.hpp"
#include <iostream>

namespace cli {
  std::vector<std::string_view> get_cli_args(const int argc,
                                             const char* const argv[]) {
    std::vector<std::string_view> args;
    args.reserve(static_cast<std::size_t>(argc));

    for (int i = 0; i < argc; ++i) {
      args.emplace_back(argv[i]);
    }

    return args;
  }

#pragma region OptionParsing
  namespace {
    template <typename T>
    std::variant<T, std::string>
    parse(std::vector<std::string_view>::const_iterator strs,
          const std::vector<std::string_view>::const_iterator end);

    template <typename T>
    concept Parsable =
        requires(std::vector<std::string_view>::const_iterator it,
                 const std::vector<std::string_view>::const_iterator end) {
          { parse<T>(it, end) } -> std::same_as<std::variant<T, std::string>>;
        };

    template <typename ARG>
      requires Parsable<ARG>
    class Option {
      const std::string_view name;
      bool parsed = false;
      ARG value;

    public:
      operator ARG() { return value; }

      void set(const ARG val) {
        value = val;
        parsed = true;
      }
      [[nodiscard]] ARG get() const { return value; }
      [[nodiscard]] bool isParsed() const { return parsed; }
      [[nodiscard]] const std::string_view& getName() const { return name; }

      static bool isAnOption(const std::string_view str) {
        return str.starts_with("--");
      }

      constexpr Option(const std::string_view name, const ARG defaultVal)
          : name{name}, value(defaultVal) {}

      bool is(const std::string_view str) const {
        return isAnOption(str) && str.substr(2) == name;
      }

      std::variant<ARG, std::string>
      parse(std::vector<std::string_view>::const_iterator strs,
            const std::vector<std::string_view>::const_iterator end) {
        if (strs == end) {
          return std::string{"Expected option --" + std::string(name) + "."};
        }

        if (!(isAnOption(*strs) && strs->substr(2) == name)) {
          return std::string{"Not an option " + std::string{*strs}};
        }

        ++strs;

        if constexpr (std::is_same_v<ARG, bool>) {
          return true;
        }

        if (strs == end || isAnOption(*strs)) {
          return std::string{"Missing argument for option --" +
                             std::string(name) + ", expected `" +
                             typeid(ARG).name() + "`."};
        }

        const auto parsed = cli::parse<ARG>(strs, end);
        if (parsed.index() == 0) {
          return std::get<ARG>(parsed);
        } else {
          return std::string{"Failed to parse argument for option --" +
                             std::string(name) + ": " +
                             std::get<std::string>(parsed)};
        }
      }
    };

    enum ParseResult { SUCCESS = 1, FAILURE = 2, INCOMPLETE = 0 };

    template <typename T>
      requires Parsable<T>
    ParseResult
    tryParseInto(Option<T>& opt,
                 std::vector<std::string_view>::const_iterator it,
                 const std::vector<std::string_view>::const_iterator end) {
      if (it == end) {
        return ParseResult::INCOMPLETE;
      }

      if (!opt.is(*it)) {
        return ParseResult::INCOMPLETE;
      }

      auto parsed = opt.parse(it, end);
      if (parsed.index() == 0) {
        auto val = std::get<T>(parsed);
        if (opt.isParsed()) {
          std::cerr << "Warning: Option --" << std::string{opt.getName()}
                    << " specified multiple times. Using last value (" << val
                    << ")." << std::endl;
        }
        opt.set(val);
        return ParseResult::SUCCESS;
      } else {
        std::cerr << std::get<std::string>(parsed) << std::endl;
        return ParseResult::FAILURE;
      }
    }
  } // namespace
#pragma endregion

#define BREAK_IF_FAIL                                                          \
  if ((parsed & ParseResult::FAILURE) != 0) {                                  \
    return std::tuple{"Failed to parse arguments.", 1u};                       \
  }

#define PARSE(_OPT)                                                            \
  parsed |= tryParseInto(_OPT, it, args.cend());                               \
  BREAK_IF_FAIL

  std::variant<Args, std::tuple<std::string, uint32_t>>
  Args::fromArgs(const std::vector<std::string_view> args) {
    Option<uint32_t> rounds("rounds", 100);
    Option<uint32_t> repeats("repeats", 10);
    Option<double> epsilon("epsilon", 0.0);
    Option<uint32_t> seed("seed", 0);
    Option<std::vector<Payoff>> payoffs(
        "payoff", {Payoff::T, Payoff::R, Payoff ::P, Payoff::S});
    Option<std::pair<std::vector<Strategy>, double>> strategies(
        "strategies", {{Strategy::ALLC, Strategy::ALLD}, 0.5});
    Option<Format> format("format", Format::TEXT);
    Option<std::filesystem::path> save("save", "");
    Option<std::filesystem::path> load("load", "");
    Option<bool> evolve("evolve", false);
    Option<uint32_t> population("population", 100);
    Option<uint32_t> generations("generations", 50);
    Option<double> mutationRate("mutation", 0.01);

    auto it = args.cbegin();
    ++it; // skip program name
    for (; it != args.cend(); ++it) {

      int parsed = ParseResult::SUCCESS;

      PARSE(rounds);
      PARSE(repeats);
      PARSE(epsilon);
      PARSE(seed);
      PARSE(payoffs);
      PARSE(strategies);
      PARSE(format);
      PARSE(save);
      PARSE(load);
      PARSE(evolve);
      PARSE(population);
      PARSE(generations);
      PARSE(mutationRate);

      if (it != args.cend() && (parsed & ParseResult::SUCCESS) == 0) {
        return std::tuple{"Unknown option: " + std::string{*it}, 1u};
      }
    }

    return Args{.rounds = rounds,
                .repeats = repeats,
                .epsilon = epsilon,
                .seed = seed,
                .enablePayoffs = payoffs,
                .strategies = strategies,
                .format = format,
                .savePath = save,
                .loadFile = load,
                .evolve = evolve,
                .population = population,
                .generations = generations,
                .mutationRate = mutationRate};
  }

#pragma region ParseSpecializations
  namespace {
    template <>
    std::variant<bool, std::string>
    parse<bool>(std::vector<std::string_view>::const_iterator strs,
                const std::vector<std::string_view>::const_iterator end) {
      return true;
    }

    template <>
    std::variant<uint32_t, std::string>
    parse<uint32_t>(std::vector<std::string_view>::const_iterator strs,
                    const std::vector<std::string_view>::const_iterator end) {
      std::string_view numStr = *strs;
      ++strs;
      auto num = std::stoul(std::string{numStr});
      return static_cast<uint32_t>(num);
    }

    template <>
    std::variant<double, std::string>
    parse<double>(std::vector<std::string_view>::const_iterator strs,
                  const std::vector<std::string_view>::const_iterator end) {
      std::string_view numStr = *strs;
      ++strs;
      double num = std::stod(std::string{numStr});
      return num;
    }

    template <>
    std::variant<std::vector<Payoff>, std::string> parse<std::vector<Payoff>>(
        std::vector<std::string_view>::const_iterator strs,
        const std::vector<std::string_view>::const_iterator end) {
      std::vector<Payoff> payoffs;
      for (; strs != end; ++strs) {
        if (Option<std::vector<Payoff>>::isAnOption(*strs)) {
          break;
        }
        size_t pos = 0;
        size_t commaPos;

        do {
          commaPos = strs->find(',', pos);
          std::string_view token = strs->substr(pos, commaPos - pos);
          if (token == "T") {
            payoffs.push_back(Payoff::T);
          } else if (token == "R") {
            payoffs.push_back(Payoff::R);
          } else if (token == "P") {
            payoffs.push_back(Payoff::P);
          } else if (token == "S") {
            payoffs.push_back(Payoff::S);
          } else {
            return std::string{"Unknown payoff: " + std::string{token}};
          }
          pos = commaPos + 1;
        } while (commaPos != std::string_view::npos);
      }
      if (payoffs.empty()) {
        return std::string{"Expected at least one payoff."};
      }
      return payoffs;
    }

    template <>
    std::variant<std::pair<std::vector<Strategy>, double>, std::string>
    parse(std::vector<std::string_view>::const_iterator strs,
          const std::vector<std::string_view>::const_iterator end) {
      std::vector<Strategy> strategies;
      double randomness = 0.5;
      for (; strs != end; ++strs) {
        if (Option<std::vector<Strategy>>::isAnOption(*strs)) {
          break;
        }

        size_t pos = 0;
        size_t commaPos;

        do {
          commaPos = strs->find(',', pos);
          std::string_view token = strs->substr(pos, commaPos - pos);
          if (token == "ALLC") {
            strategies.push_back(Strategy::ALLC);
          } else if (token == "ALLD") {
            strategies.push_back(Strategy::ALLD);
          } else if (token == "TFT") {
            strategies.push_back(Strategy::TFT);
          } else if (token == "GRIM") {
            strategies.push_back(Strategy::GRIM);
          } else if (token == "PAVLOV") {
            strategies.push_back(Strategy::PAVLOV);
          } else if (token.starts_with("RND")) {
            if (token.size() < 6) {
              return std::string{
                  "RND strategy must be of the form RNDX where X is a "
                  "floating point number in [0,1] including the number before "
                  "the decimal point, and at least one after."};
            }
            randomness = std::stod(std::string{token.substr(3)});
            strategies.push_back(Strategy::RND);
          } else if (token == "CONTRITE") {
            strategies.push_back(Strategy::CONTRITE);
          } else if (token == "PROBER") {
            strategies.push_back(Strategy::PROBER);
          } else {
            return std::string{"Unknown strategy: " + std::string{token}};
          }
          pos = commaPos + 1;
        } while (commaPos != std::string_view::npos);
      }

      if (strategies.empty()) {
        return std::string{"Expected at least one strategy."};
      }
      return std::make_pair(strategies, randomness);
    }

    template <>
    std::variant<Format, std::string>
    parse<Format>(std::vector<std::string_view>::const_iterator strs,
                  const std::vector<std::string_view>::const_iterator end) {
      std::string_view formatStr = *strs;
      ++strs;
      if (formatStr == "text") {
        return Format::TEXT;
      } else if (formatStr == "json") {
        return Format::JSON;
      } else if (formatStr == "csv") {
        return Format::CSV;
      } else {
        return std::string{"Unknown format: " + std::string{formatStr}};
      }
    }

    template <>
    std::variant<std::filesystem::path, std::string>
    parse<std::filesystem::path>(
        std::vector<std::string_view>::const_iterator strs,
        const std::vector<std::string_view>::const_iterator end) {
      std::string_view pathStr = *strs;
      ++strs;
      return std::filesystem::path{std::string{pathStr}};
    }

  } // namespace
#pragma endregion
} // namespace cli
