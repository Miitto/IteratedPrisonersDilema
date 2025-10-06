#pragma once

#include "cli/cli.hpp"
#include "strats/strategy.hpp"

#include <memory>

class Game {
  const cli::Args& m_args;

  std::unique_ptr<strats::Strategy> m_strat1{};
  std::unique_ptr<strats::Strategy> m_strat2{};

  std::vector<std::pair<strats::Payoff, strats::Payoff>> m_results{};

  void playRound();

public:
  Game(const cli::Args& args, cli::Strategy strat1, cli::Strategy strat2);

  void play();
};