#pragma once

#include "game.hpp"
#include <optional>

struct BracketResult {
  double mean;
  double low;
  double high;

  friend std::ostream& operator<<(std::ostream& os, const BracketResult& br);
};

class Bracket {
  const cli::Args& m_args;
  std::vector<Game> m_games{};

  const cli::Strategy m_strat1;
  const cli::Strategy m_strat2;

public:
  inline Bracket(const cli::Args& args, const cli::Strategy strat1,
                 const cli::Strategy strat2)
      : m_args(args), m_strat1(strat1), m_strat2(strat2) {}

  inline const cli::Strategy& getStrat1() const { return m_strat1; }
  inline const cli::Strategy& getStrat2() const { return m_strat2; }
  inline const std::vector<Game>& getGames() const { return m_games; }

  void play();

  double getStrat1Mean() const;
  double getStrat2Mean() const;

  double getStdDev1(std::optional<double> mean = std::nullopt) const;
  double getStdDev2(std::optional<double> mean = std::nullopt) const;

  std::pair<double, double>
  getConfInterval1(std::optional<double> stdDev = std::nullopt) const;
  std::pair<double, double>
  getConfInterval2(std::optional<double> stdDev = std::nullopt) const;

  BracketResult strat1Result() const;
  BracketResult strat2Result() const;
};
