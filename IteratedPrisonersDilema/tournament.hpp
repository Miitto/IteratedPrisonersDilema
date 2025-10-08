#pragma once

#include "cli/cli.hpp"

#include "bracket.hpp"

class ScoreMatrix {
  const cli::Args& m_args;
  std::vector<std::vector<BracketResult>> m_matrix;

public:
  ScoreMatrix(const cli::Args& args,
              std::vector<std::vector<BracketResult>>&& matrix)
      : m_args(args), m_matrix(matrix) {}

  const std::vector<std::vector<BracketResult>>& getMatrix() const {
    return m_matrix;
  }

  std::vector<std::pair<cli::Strategy, BracketResult>> getResults() const;

  friend std::ostream& operator<<(std::ostream& os, const ScoreMatrix& sm);
};

class Tournament {
  const cli::Args& m_args;

  const std::vector<cli::Strategy>& m_strategies;

  std::vector<Bracket> m_brackets;

public:
  Tournament(const cli::Args& args, const std::vector<cli::Strategy>& strats)
      : m_args(args), m_strategies(strats) {}

  void run();

  ScoreMatrix getScoreMatrix() const;
};