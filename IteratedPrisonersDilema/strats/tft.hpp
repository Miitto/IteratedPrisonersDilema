#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  /// <summary>
  /// Strategy that starts by cooperating, then mimics the opponent's last move.
  /// </summary>
  class Tft : public Strategy {
    Choice m_opponentLastChoice = Choice::COOPERATE;

  public:
    Tft(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::TFT; }
    double getBudget() const override { return 2.0; }

    Choice getChoice() override { return m_opponentLastChoice; }
    void giveResult(Payoff, Choice oppChoice) override {
      m_opponentLastChoice = oppChoice;
    }

    ~Tft() override = default;
  };
} // namespace strats
