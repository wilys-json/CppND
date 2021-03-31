#ifndef DETECT_OS_H
#define DETECT_OS_H

#ifdef __linux__
#include "linux_parser.h"
using LinuxParser::SystemParser;
using LinuxParser::ProcessorParser;
using LinuxParser::ProcessParser;
using LinuxParser::Pids;
#endif

#endif
