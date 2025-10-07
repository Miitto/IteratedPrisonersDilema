#pragma once

#include "cli/cli.hpp"
#include "strats/strategy.hpp"

#include <memory>
#include <random>

class Game {
  const cli::Args& m_args;

  std::unique_ptr<strats::Strategy> m_strat1{};
  std::unique_ptr<strats::Strategy> m_strat2{};

  std::vector<std::pair<strats::Payoff, strats::Payoff>> m_results{};

  std::mt19937 gen;
  std::uniform_real_distribution<double> dist{0.0, 1.0};

  void playRound();

public:
  Game(const cli::Args& args, cli::Strategy strat1, cli::Strategy strat2,
       int repeat);

  void play();

  std::pair<double, double> getTotalScores() const;
  std::pair<double, double> getAverageScores() const;
  std::pair<double, double>
  getAverageScores(const std::pair<double, double>& totals) const;

  void printResults(std::ostream& os) const;

  const strats::Strategy& getStrat1() const { return *m_strat1; }
  const strats::Strategy& getStrat2() const { return *m_strat2; }
};