#ifndef DETECT_OS_H
#define DETECT_OS_H

// Linux
#ifdef __linux__
#include "linux_parser.h"
using LinuxParser::Pids;
using LinuxParser::ProcessorParser;
using LinuxParser::ProcessParser;
using LinuxParser::SystemParser;
#endif

// May add other OS here

#endif
