#include "strategy.hpp"

namespace strats {
  std::ostream& operator<<(std::ostream& os, const Payoff p) {
    switch (p) {
    case Payoff::Temptation:
      os << "Temptation";
      break;
    case Payoff::Reward:
      os << "Reward";
      break;
    case Payoff::Punishment:
      os << "Punishment";
      break;
    case Payoff::Sucker:
      os << "Sucker";
      break;
    }
    return os;
  }

  Choice getOpponentsChoice(Payoff p) {
    switch (p) {
    case Payoff::Temptation:
      return Choice::COOPERATE;
    case Payoff::Reward:
      return Choice::COOPERATE;
    case Payoff::Punishment:
      return Choice::DEFECT;
    case Payoff::Sucker:
      return Choice::DEFECT;
    }
    // Should be unreachable
    return Choice::COOPERATE;
  }
} // namespace strats