#pragma once
#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG(msg) \
        std::cerr << "[DEBUG] " << __FILE__ << ":" << __LINE__ << " - " << msg << std::endl
#else
#define DEBUG_LOG(msg)
#endif