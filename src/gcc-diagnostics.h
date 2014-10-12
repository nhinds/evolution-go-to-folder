#ifndef GCC_DIAGNOSTICS_H
#define GCC_DIAGNOSTICS_H

// Allows suppressing warnings in gcc
// Source: http://dbp-consulting.com/tutorials/SuppressingGCCWarnings.html
// Original Authors of GCC_DIAG_OFF/GCC_DIAG_ON: Jonathan Wakely and Patrick Horgan

#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 402
#define GCC_DIAG_STR(s) #s
#define GCC_DIAG_JOINSTR(x,y) GCC_DIAG_STR(x ## y)
# define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
# define GCC_DIAG_PRAGMA(x) GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
# if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
#  define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(push) \
	GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#  define GCC_DIAG_ON(x) GCC_DIAG_PRAGMA(pop)
# else
#  define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#  define GCC_DIAG_ON(x)  GCC_DIAG_PRAGMA(warning GCC_DIAG_JOINSTR(-W,x))
# endif
#else
# define GCC_DIAG_OFF(x)
# define GCC_DIAG_ON(x)
#endif

// Usage:
//   IGNORING(deprecated-declarations,
//     ...
//   )
#define IGNORING(warning, code) GCC_DIAG_OFF(warning) code GCC_DIAG_ON(warning)

#endif // GCC_DIAGNOSTICS_H