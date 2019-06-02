// Global OS Check.
#ifdef __linux__
#elif _WIN32
#else
#error "Not Supported on this OS"
#endif

#ifndef TOBIIMOUSE_H
#define TOBIIMOUSE_H

// Unused variable handling.
#define UNUSED(expr) do { (void)(expr); } while (0)

#endif // TOBIIMOUSE_H
