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

  template <>
  std::variant<uint32_t, std::string>
  parse(std::vector<std::string_view>::const_iterator strs,
        const std::vector<std::string_view>::const_iterator end) {
    std::string_view numStr = *strs;
    ++strs;
    int num = std::stoi(std::string{numStr});
    return static_cast<uint32_t>(num);
  }

  namespace {
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
        opt.value = std::get<T>(parsed);
        return ParseResult::SUCCESS;
      } else {
        std::cerr << std::get<std::string>(parsed) << std::endl;
        return ParseResult::FAILURE;
      }
    }
  } // namespace

  std::variant<Args, std::tuple<std::string, uint32_t>>
  Args::fromArgs(const std::vector<std::string_view> args) {
    Option<uint32_t> rounds("rounds", 100);

    auto it = args.cbegin();
    ++it; // skip program name
    for (; it != args.cend(); ++it) {
#define BREAK_IF_FAIL                                                          \
  if ((parsed & ParseResult::FAILURE) != 0) {                                  \
    return std::tuple{"Failed to parse arguments.", 1u};                       \
  }
      int parsed = ParseResult::SUCCESS;
      parsed |= tryParseInto(rounds, it, args.cend());
      BREAK_IF_FAIL

      if (it != args.cend() && (parsed & ParseResult::SUCCESS) == 0) {
        return std::tuple{"Unknown option: " + std::string{*it}, 1u};
      }
    }

    return Args{.rounds = rounds};
  }
} // namespace cli