#include "game.hpp"
#include <iostream>
#include <random>

#include "strats/allc.hpp"
#include "strats/alld.hpp"
#include "strats/ctft.hpp"
#include "strats/grim.hpp"
#include "strats/pavlov.hpp"
#include "strats/prober.hpp"
#include "strats/rnd.hpp"
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
    case cli::Strategy::PROBER:
      return std::make_unique<strats::Prober>(m_args);
    }

    // Should be unreachable
    throw std::runtime_error("Unknown strategy");
  };

  m_strat1 = makeStrat(strat1);
  m_strat2 = makeStrat(strat2);
}

namespace {
  constexpr std::pair<strats::Payoff, strats::Payoff> RESULT_MATRIX[2][2] = {
      {{strats::Payoff::Reward, strats::Payoff::Reward},         // C, C
       {strats::Payoff::Sucker, strats::Payoff::Temptation}},    // C, D
      {{strats::Payoff::Temptation, strats::Payoff::Sucker},     // D, C
       {strats::Payoff::Punishment, strats::Payoff::Punishment}} // D, D
  };
}

void Game::playRound() {
  auto& s1 = *m_strat1;
  auto& s2 = *m_strat2;

  auto choice1 = s1.getChoice();
  auto choice2 = s2.getChoice();

  auto flipChoice = [](strats::Choice c) -> strats::Choice {
    return (c == strats::Choice::COOPERATE) ? strats::Choice::DEFECT
                                            : strats::Choice::COOPERATE;
  };

  if (dist(gen) < m_args.epsilon) { // Mistake for strat1
    choice1 = flipChoice(choice1);
  }

  if (dist(gen) < m_args.epsilon) {
    choice2 = flipChoice(choice2);
  }

  auto payoffPair =
      RESULT_MATRIX[static_cast<int>(choice1)][static_cast<int>(choice2)];

  auto& [result1, result2] = payoffPair;

  s1.giveResult(result1, choice2);
  s2.giveResult(result2, choice1);

  m_results.emplace_back(
      RoundResult{
          .choice = choice1,
          .reward = strats::getPayoffValue(m_args.payoffs, result1),
          .payoff = result1,
      },
      RoundResult{
          .choice = choice2,
          .reward = strats::getPayoffValue(m_args.payoffs, result2),
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