#ifndef TPKIT_API_EXPORT_H_
#define TPKIT_API_EXPORT_H_

#undef TPKIT_API
#if defined(TPKIT_STATIC)
#define TPKIT_API
#elif defined(_WIN32) || defined(__CYGWIN__)
#if defined(BUILD_TPKIT)
#define TPKIT_API __declspec(dllexport)
#else
#define TPKIT_API __declspec(dllimport)
#endif // defined(BUILD_TPKIT)
#elif defined(__GNUC__) || defined(__clang__)
#define TPKIT_API __attribute__((visibility("default")))
#endif // defined(TPKIT_STATIC)

#endif // TPKIT_API_EXPORT_H_
