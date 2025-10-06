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
} // namespace strats