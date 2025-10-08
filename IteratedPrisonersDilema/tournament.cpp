#include "tournament.hpp"
#include "colors.hpp"
#include "game.hpp"

#include <iostream>

void Tournament::run() {
  for (size_t i = 0; i < m_strategies.size(); ++i) {
    for (size_t j = i; j < m_strategies.size(); ++j) {
      auto& strat1 = m_strategies[i];
      auto& strat2 = m_strategies[j];

      Bracket bracket(m_args, strat1, strat2);
      bracket.play();
      m_brackets.push_back(std::move(bracket));
    }
  }
}

ScoreMatrix Tournament::getScoreMatrix() const {
  size_t n = m_args.strategies.size();
  std::vector<std::vector<BracketResult>> matrix(
      n, std::vector<BracketResult>(n, {}));

  auto getStratIndex = [&](const cli::Strategy& strat) {
    for (size_t i = 0; i < m_args.strategies.size(); ++i) {
      if (strat == m_args.strategies[i])
        return i;
    }
    return size_t(-1); // Not found
  };

  for (const auto& game : m_brackets) {
    auto strat1 = game.strat1Result();
    auto strat2 = game.strat2Result();
    auto idx1 = getStratIndex(game.getStrat1());
    auto idx2 = getStratIndex(game.getStrat2());
    if (idx1 == size_t(-1) || idx2 == size_t(-1)) {
      throw std::runtime_error(
          "Strategy not found in tournament strategies, should be unreachable");
    }
    matrix[idx1][idx2] = strat1;
    matrix[idx2][idx1] = strat2;
  }

  return ScoreMatrix(m_args, std::move(matrix));
}

std::vector<std::pair<cli::Strategy, BracketResult>>
ScoreMatrix::getResults() const {
  size_t n = m_matrix.size();
  std::vector<std::pair<cli::Strategy, BracketResult>> avgs;
  avgs.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    double total = 0.0;
    for (size_t j = 0; j < n; ++j) {
      total += m_matrix[i][j].mean;
    }

    double mean = total / m_args.strategies.size();

    double stdDev = 0.0;
    for (size_t j = 0; j < n; ++j) {
      auto diff = m_matrix[i][j].mean - mean;
      stdDev += diff * diff;
    }

    stdDev = std::sqrt(stdDev / m_args.strategies.size());

    double confInterval = 1.96 * (stdDev / std::sqrt(m_args.strategies.size()));
    BracketResult res{mean, mean - confInterval, mean + confInterval};

    avgs.emplace_back(m_args.strategies[i], res);
  }
  std::sort(avgs.begin(), avgs.end(), [](const auto& a, const auto& b) {
    return a.second.mean > b.second.mean;
  });

  return avgs;
}

namespace {
  void printTextMatrix(std::ostream& os,
                       const std::vector<std::vector<BracketResult>>& matrix,
                       const std::vector<cli::Strategy>& strategies,
                       bool useColors, const cli::Payoffs& payoffs) {
    size_t n = matrix.size();
    size_t maxStratLen = 0;
    for (const auto& strat : strategies) {
      std::ostringstream ss;
      ss << strat;
      maxStratLen = std::max(maxStratLen, ss.str().length());
    }
    size_t maxWidth =
        std::max(maxStratLen, static_cast<size_t>(17)); // Width for scores

    os << "Payoffs: T=" << payoffs.temptation << ", R=" << payoffs.reward
       << ", P=" << payoffs.punishment << ", S=" << payoffs.sucker << "\n\n";
    os << "Score Matrix:\n";
    os << std::setw(maxStratLen + 1) << ' ';
    for (size_t j = 0; j < n; ++j) {
      os << std::setw(maxWidth + 1) << strategies[j];
    }
    os << "\n";
    for (size_t i = 0; i < n; ++i) {
      os << std::setw(maxStratLen) << strategies[i] << ": ";
      for (size_t j = 0; j < n; ++j) {
        if (useColors) {
          auto value = matrix[i][j];
          auto lerpAmount = std::min(1.0, value.mean / payoffs.temptation);

          RGBColor startColor(255, 50, 50); // Red
          RGBColor endColor(50, 255, 50);   // Green

          RGBColor lerpedColor = startColor.lerp(endColor, lerpAmount);

          os << "\033[38;2;" << lerpedColor.r << ";" << lerpedColor.g << ";"
             << lerpedColor.b << "m";
        }
        os << std::fixed << std::setprecision(2) << std::setw(maxWidth)
           << matrix[i][j] << "\033[39m ";
      }
      os << "\n";
    }
  }

  void printTextAverageScores(
      std::ostream& os,
      const std::vector<std::pair<cli::Strategy, BracketResult>>& avgScores,
      bool useColors, double maxScore) {
    os << "\nAverage Scores:\n";
    size_t maxStratLen = 0;
    for (const auto& [strat, _] : avgScores) {
      std::ostringstream ss;
      ss << strat;
      maxStratLen = std::max(maxStratLen, ss.str().length());
    }
    for (const auto& [strat, score] : avgScores) {
      os << std::setw(maxStratLen) << strat << ": ";
      if (useColors) {
        RGBColor startColor(255, 0, 0); // Red
        RGBColor endColor(0, 255, 0);   // Green
        double lerpAmount = std::clamp(score.mean / maxScore, 0.0,
                                       1.0); // Assuming max score ~5
        RGBColor lerpedColor = startColor.lerp(endColor, lerpAmount);
        os << "\033[38;2;" << lerpedColor.r << ";" << lerpedColor.g << ";"
           << lerpedColor.b << "m";
      }
      os << std::fixed << std::setprecision(2) << score;
      if (useColors) {
        os << "\033[39m"; // Reset to default color
      }
      os << "\n";
    }
  }

  void printJsonMatrix(std::ostream& os,
                       const std::vector<std::vector<BracketResult>>& matrix,
                       const std::vector<cli::Strategy>& strats) {
    os << "{\n";
    for (int i = 0; i < matrix.size(); ++i) {
      os << "  \"" << strats[i] << "\": [\n";
      for (int j = 0; j < matrix[i].size(); ++j) {
        os << "    {\n      \"name\": \"" << strats[j]
           << "\",\n      \"mean\": " << matrix[i][j].mean << ",\n"
           << "\",\n      \"low\": " << matrix[i][j].low << ",\n"
           << "\",\n      \"high\": " << matrix[i][j].high << "\n    }";
        if (j != matrix[i].size()) {
          os << ",";
        }
        os << "\n";
      }
      os << "  ]";
      if (i != matrix.size() - 1) {
        os << ",";
      }
      os << "\n";
    }
    os << "}\n";
  }
} // namespace

std::ostream& operator<<(std::ostream& os, const ScoreMatrix& sm) {
  const auto& matrix = sm.getMatrix();
  size_t n = matrix.size();

  switch (sm.m_args.format) {
  case cli::Format::TEXT: {
    printTextMatrix(os, matrix, sm.m_args.strategies, sm.m_args.savePath == "",
                    sm.m_args.payoffs);
    auto averages = sm.getResults();
    printTextAverageScores(os, averages, sm.m_args.savePath == "",
                           sm.m_args.payoffs.temptation);
    break;
  }
  case cli::Format::CSV: {
    os << "Player";
    for (size_t j = 0; j < n; ++j) {
      os << ",P" << j;
    }
    os << "\n";
    for (size_t i = 0; i < n; ++i) {
      os << "P" << i;
      for (size_t j = 0; j < n; ++j) {
        os << "," << std::fixed << std::setprecision(2) << matrix[i][j];
      }
      os << "\n";
    }
    break;
  }
  case cli::Format::JSON: {
    printJsonMatrix(os, matrix, sm.m_args.strategies);
    break;
  }
  }
  return os;
}