#pragma once

#include <ostream>

#include "../cli/cli.hpp"

namespace strats {

  enum class Payoff { Temptation, Reward, Punishment, Sucker };

  inline double getPayoffValue(const cli::Payoffs& payoffs, Payoff p) {
    switch (p) {
    case Payoff::Temptation:
      return payoffs.temptation;
    case Payoff::Reward:
      return payoffs.reward;
    case Payoff::Punishment:
      return payoffs.punishment;
    case Payoff::Sucker:
      return payoffs.sucker;
    }
    // Should be unreachable
    return 0.0;
  }

  enum Choice { COOPERATE = 0, DEFECT = 1 };

  std::ostream& operator<<(std::ostream& os, const Payoff p);

  class Strategy {
  protected:
    const cli::Payoffs& m_payoffs;

    Strategy(const cli::Payoffs& payoffs) : m_payoffs(payoffs) {}

  public:
    virtual cli::Strategy getStrat() = 0;

    virtual Choice getChoice() const = 0;
    virtual void giveResult(Payoff result, Choice oppChoice) = 0;

    virtual ~Strategy() = default;
  };
} // namespace strats
