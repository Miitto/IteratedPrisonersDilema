#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  /// <summary>
  /// Strategy that cooperates until the opponent defects twice in a row, then
  /// defects forever
  /// </summary>
  class FGrim : public Strategy {
    bool m_triggered = false;
    Choice m_oppLastChoice = Choice::COOPERATE;

  public:
    FGrim(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::FGRIM; }
    double getBudget() const override { return 2.0; }

    Choice getChoice() override {
      return m_triggered ? Choice::DEFECT : Choice::COOPERATE;
    }

    void giveResult(Payoff, Choice oppChoice) override {
      // Trigger if opponent defects twice in a row
      if (oppChoice == Choice::DEFECT && m_oppLastChoice == Choice::DEFECT)
        m_triggered = true;

      // Store opponents choice
      m_oppLastChoice = oppChoice;
    }

    ~FGrim() override = default;
  };
} // namespace strats
