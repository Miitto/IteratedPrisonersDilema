#include "strategy.hpp"

namespace strats {
  /// <summary>
  /// Determines the payoff result based on two choices in a game scenario.
  /// </summary>
  /// <param name="c1">The first player's choice (COOPERATE or DEFECT).</param>
  /// <param name="c2">The second player's choice (COOPERATE or DEFECT).</param>
  /// <returns>The payoff value corresponding to the combination of choices:
  /// Reward, Sucker, Temptation, or Punishment.</returns>
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

  /// <summary>
  /// Returns the opposite of the given Choice value.
  /// </summary>
  /// <param name="c">The Choice value to invert.</param>
  /// <returns>DEFECT if c is COOPERATE; otherwise, COOPERATE.</returns>
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