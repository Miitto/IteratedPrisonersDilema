#include "game.hpp"
#include <iostream>
#include <random>

#include "strats/allc.hpp"
#include "strats/alld.hpp"
#include "strats/ctft.hpp"
#include "strats/fgrim.hpp"
#include "strats/grim.hpp"
#include "strats/pavlov.hpp"
#include "strats/prober.hpp"
#include "strats/rnd.hpp"
#include "strats/smart_prober.hpp"
#include "strats/tft.hpp"

Game::Game(const cli::Args& args, cli::Strategy strat1, cli::Strategy strat2,
           int repeat)
    : m_args(args), gen(args.seed + repeat) {

  auto makeStrat =
      [&, this](cli::Strategy s) -> std::unique_ptr<strats::Strategy> {
    if (s.isRnd()) {
      return std::make_unique<strats::Rnd>(m_args, s.rnd(), args.seed + repeat);
    }

    switch (s.simple()) {
    case cli::Strategy::ALLC: {
      return std::make_unique<strats::AllC>(m_args);
    }
    case cli::Strategy::ALLD: {
      return std::make_unique<strats::AllD>(m_args);
    }
    case cli::Strategy::TFT: {
      return std::make_unique<strats::Tft>(m_args);
    }
    case cli::Strategy::GRIM: {
      return std::make_unique<strats::Grim>(m_args);
    }
    case cli::Strategy::PAVLOV: {
      return std::make_unique<strats::Pavlov>(m_args);
    }
    case cli::Strategy::CONTRITE: {
      return std::make_unique<strats::CTft>(m_args);
    }
    case cli::Strategy::PROBER: {
      return std::make_unique<strats::Prober>(m_args);
    }
    case cli::Strategy::FGRIM: {
      return std::make_unique<strats::FGrim>(m_args);
    }
    case cli::Strategy::SPROBER: {
      return std::make_unique<strats::SmartProber>(m_args);
    }
    }

    // Should be unreachable
    throw std::runtime_error("Unknown strategy");
  };

  m_strat1 = makeStrat(strat1);
  m_strat2 = makeStrat(strat2);
}

void Game::playRound() {
  auto& s1 = *m_strat1;
  auto& s2 = *m_strat2;

  auto choice1 = s1.getChoice();
  auto choice2 = s2.getChoice();

  if (dist(gen) < m_args.epsilon) {
    choice1 = !choice1;
  }

  if (dist(gen) < m_args.epsilon) {
    choice2 = !choice2;
  }

  auto result1 = choice1 & choice2;
  auto result2 = choice2 & choice1;

  s1.giveResult(result1, choice2);
  s2.giveResult(result2, choice1);

  double reward1 = strats::getPayoffValue(m_args.payoffs, result1);
  double reward2 = strats::getPayoffValue(m_args.payoffs, result2);

  if (m_args.enableBudget) {
    double budget1 = s1.getBudget();
    double budget2 = s2.getBudget();
    reward1 = std::max(0.0, reward1 - budget1);
    reward2 = std::max(0.0, reward2 - budget2);
  }

  m_results.emplace_back(
      RoundResult{
          .choice = choice1,
          .reward = reward1,
          .payoff = result1,
      },
      RoundResult{
          .choice = choice2,
          .reward = reward2,
          .payoff = result2,
      });
}

void Game::play() {
  for (uint32_t round = 0; round < m_args.rounds; ++round) {
    playRound();
  }
}

std::pair<double, double> Game::getTotalScores() const {
  double total1 = 0.0;
  double total2 = 0.0;
  for (auto& [r1, r2] : m_results) {
    total1 += r1.reward;
    total2 += r2.reward;
  }
  return {total1, total2};
}

std::pair<double, double>
Game::getAverageScores(const std::pair<double, double>& totals) const {
  return {totals.first / m_args.rounds, totals.second / m_args.rounds};
}

std::pair<double, double> Game::getAverageScores() const {
  auto totals = getTotalScores();
  return getAverageScores(totals);
}