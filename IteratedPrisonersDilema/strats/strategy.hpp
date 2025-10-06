#pragma once

#include "cli/cli.hpp"

namespace strats {
  enum class Payoff { Temptation, Reward, Punishment, Sucker };
  enum Choice { COOPERATE = 0, DEFECT = 1 };

  class Strategy {
  protected:
    const cli::Payoffs& m_payoffs;

    Strategy(const cli::Payoffs& payoffs) : m_payoffs(payoffs) {}

  public:
    virtual cli::Strategy getStrat() = 0;

    virtual Choice getChoice() const = 0;
    virtual void giveResult(Payoff result) = 0;

    virtual bool suseptibleToError() const { return true; }

    virtual ~Strategy() = default;
  };
} // namespace strats
