#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  /// <summary>
  /// Strategy that plays tit-for-tat, but if it accidentally defects due to
  /// noise and gets retaliated against, it will cooperate for a turn in an
  /// attempt to apologize.
  /// </summary>
  class CTft : public Strategy {
    bool didAccidentallyDefect = false;
    Choice m_opponentLastChoice = Choice::COOPERATE;

  protected:
    Choice m_ownLastChoice = Choice::COOPERATE;

    bool didSwap(Payoff p, Choice choice) {
      Choice actual = Choice::COOPERATE;
      switch (p) {
      case Payoff::Temptation:
      case Payoff::Punishment:
        actual = Choice::DEFECT;
        break;
      }

      return actual != choice;
    }

  public:
    CTft(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::CONTRITE; }
    double getBudget() const override { return 3.0; }

    Choice getChoice() override {
      m_ownLastChoice = m_opponentLastChoice;
      return m_opponentLastChoice;
    }
    void giveResult(Payoff p, Choice oppChoice) override {
      if (didAccidentallyDefect && oppChoice == Choice::DEFECT) {
        m_opponentLastChoice = Choice::COOPERATE;
      } else {
        m_opponentLastChoice = oppChoice;
      }

      if (m_ownLastChoice == Choice::COOPERATE && didSwap(p, m_ownLastChoice)) {
        didAccidentallyDefect = true;
      } else {
        didAccidentallyDefect = false;
      }
    }

    ~CTft() override = default;
  };
} // namespace strats
