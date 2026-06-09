#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "Process.h"

std::vector<Process> readProcessesFromFile(const std::string& inputFile);

#endif
