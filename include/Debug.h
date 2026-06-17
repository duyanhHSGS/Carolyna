#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#include <iostream>
#define DEBUG_LOG(expr) do { std::cerr << expr << std::endl; } while (false)
#define DEBUG_ONLY(stmt) do { stmt; } while (false)
#else
#define DEBUG_LOG(expr) do { } while (false)
#define DEBUG_ONLY(stmt) do { } while (false)
#endif

#endif  // DEBUG_H
