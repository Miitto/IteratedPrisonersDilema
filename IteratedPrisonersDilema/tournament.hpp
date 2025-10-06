#pragma once

#include "cli/cli.hpp"

class Tournament {
  const cli::Args& m_args;

public:
  Tournament(const cli::Args& args) : m_args(args) {}

  void run();
};