#ifndef dkf_log_h
#define dkf_log_h

#ifdef DKF_LOG

#include <iostream>
#include <utility>
#include <sstream>
#include <iomanip>

// This function IS used. Just added the __unused to supress Xcode warnings.
__unused static void _log_cout(){}

template<typename First, typename ...Rest>
static void _log_cout(First && first, Rest && ...rest)
{
    std::cout << std::forward<First>(first);
    _log_cout(std::forward<Rest>(rest)...);
}

#define LOG_PREC(n) std::fixed, std::setprecision(n)

#define LOG(level, ...) \
    _log_cout("[", std::setw(5), #level, " ", __FILE__, ":", __func__, ":", __LINE__, "] ", ##__VA_ARGS__, "\n");

/* For C
#define LOG(level, format, ...) \
printf("[%5s %s:%s:%d] " format "\n", \
#level, __FILE__, __func__, __LINE__, ##__VA_ARGS__);
*/

#define DLOG(...) LOG(DEBUG, ##__VA_ARGS__);
#define ILOG(...) LOG(INFO, ##__VA_ARGS__);
#define WLOG(...) LOG(WARN, ##__VA_ARGS__);
#define ELOG(...) LOG(ERROR, ##__VA_ARGS__);

#else

#define LOG_PREC(n)

#define LOG(...)
#define DLOG(...)
#define ILOG(...)
#define WLOG(...)
#define ELOG(...)

#endif

#endif /* dkf_log_h */
