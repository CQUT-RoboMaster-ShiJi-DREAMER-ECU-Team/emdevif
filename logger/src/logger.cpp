/**
 * @file logger.cpp
 * @brief
 */

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module;
#else
#include "emdevif/logger.hpp"
#endif

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module emdevif.logger;
#endif

#if (EMDEVIF_LOGGER_MODE == 0)
#include "./sync.hpp"
#elif (EMDEVIF_LOGGER_MODE == 1)

#elif (EMDEVIF_LOGGER_MODE == 2)
// Nothing to include, the implementation is provided by user.
#endif
