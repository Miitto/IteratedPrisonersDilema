#pragma once

#include "cli/cli.hpp"
#include "game.hpp"

class ScoreMatrix {
  const cli::Args& m_args;
  std::vector<std::vector<double>> m_matrix;

public:
  ScoreMatrix(const cli::Args& args, std::vector<std::vector<double>>&& matrix)
      : m_args(args), m_matrix(matrix) {}

  const std::vector<std::vector<double>>& getMatrix() const { return m_matrix; }

  std::vector<std::pair<cli::Strategy, double>> getAverageScores() const;

  friend std::ostream& operator<<(std::ostream& os, const ScoreMatrix& sm);
};

class Tournament {
  const cli::Args& m_args;

  std::vector<Game> m_games;

public:
  Tournament(const cli::Args& args) : m_args(args) {}

  void run();

  ScoreMatrix getScoreMatrix() const;
};