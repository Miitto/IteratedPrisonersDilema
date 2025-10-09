#include "strategy.hpp"

namespace strats {
  Payoff operator&(Choice c1, Choice c2) {
    if (c1 == COOPERATE && c2 == COOPERATE) {
      return Payoff::Reward;
    } else if (c1 == COOPERATE && c2 == DEFECT) {
      return Payoff::Sucker;
    } else if (c1 == DEFECT && c2 == COOPERATE) {
      return Payoff::Temptation;
    } else { // (c1 == DEFECT && c2 == DEFECT)
      return Payoff::Punishment;
    }
  }

  Choice operator!(Choice c) { return (c == COOPERATE) ? DEFECT : COOPERATE; }

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

} // namespace strats