/* gecode/support/config.hpp.  Generated from config.hpp.in by configure.  */
/* gecode/support/config.hpp.in.  Generated from configure.ac by autoheader.  */

/* Disable autolink because all the dependencies are handled by CMake. */
#define GECODE_BUILD_SUPPORT
#define GECODE_BUILD_KERNEL
#define GECODE_BUILD_SEARCH
#define GECODE_BUILD_INT
#define GECODE_BUILD_SET
#define GECODE_BUILD_FLOAT
#define GECODE_BUILD_MINIMODEL
#define GECODE_BUILD_FLATZINC
#define GECODE_BUILD_DRIVER
#define GECODE_BUILD_GIST



/* Whether to include audit code */
/* #4undef GECODE_AUDIT */

/* User-defined prefix of dll names */
#define GECODE_DLL_USERPREFIX ""

/* User-defined suffix of dll names */
#define GECODE_DLL_USERSUFFIX ""

/* Supported version of FlatZinc */
#define GECODE_FLATZINC_VERSION "1.6"

/* Whether gcc understands visibility attributes */
/* #undef GECODE_GCC_HAS_CLASS_VISIBILITY */

/* whether __builtin_ffsl is available */
/* #undef GECODE_HAS_BUILTIN_FFSL */

/* Whether to build FLOAT variables */
#define GECODE_HAS_FLOAT_VARS /**/

/* Whether Gist is available */
/* #undef GECODE_HAS_GIST */

/* Whether GNU hash_map is available */
/* #undef GECODE_HAS_GNU_HASH_MAP */

/* Whether to build INT variables */
#define GECODE_HAS_INT_VARS /**/

/* Whether MPFR is available */
/* #undef GECODE_HAS_MPFR */

/* Whether we have mtrace for memory leak debugging */
/* #undef GECODE_HAS_MTRACE */

/* Whether Qt is available */
/* #undef GECODE_HAS_QT */

/* Whether to build SET variables */
#define GECODE_HAS_SET_VARS /**/

/* Whether unistd.h is available */
/* #undef GECODE_HAS_UNISTD_H */

/* Gecode version */
#define GECODE_LIBRARY_VERSION "4-3-0"

/* Heap memory alignment */
/* #undef GECODE_MEMORY_ALIGNMENT */

/* How to check allocation size */
/* #undef GECODE_MSIZE */

/* Whether to track peak heap size */
/* #undef GECODE_PEAKHEAP */

/* Whether we need malloc.h */
/* #undef GECODE_PEAKHEAP_MALLOC_H */

/* Whether we need malloc/malloc.h */
/* #undef GECODE_PEAKHEAP_MALLOC_MALLOC_H */

/* Whether we are compiling static libraries */
#define GECODE_STATIC_LIBS 1

/* Whether we have Mac OS threads */
/* #undef GECODE_THREADS_OSX */

/* Whether we have posix threads */
/* #undef GECODE_THREADS_PTHREADS */

/* Whether we have posix spinlocks */
/* #undef GECODE_THREADS_PTHREADS_SPINLOCK */

/* Whether we have windows threads */
#define GECODE_THREADS_WINDOWS 1

/* Use clock() for time-measurement */
#define GECODE_USE_CLOCK 1

/* Use gettimeofday for time-measurement */
/* #undef GECODE_USE_GETTIMEOFDAY */

/* Gecode version */
#define GECODE_VERSION "4.3.0"

/* Gecode version */
#define GECODE_VERSION_NUMBER 400300

/* How to tell the compiler to really, really inline */
#define forceinline inline

// STATISTICS: support-any

/* Define to 1 if you have the `getpagesize' function. */
/* #undef HAVE_GETPAGESIZE */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have a working `mmap' system call. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
/* #undef HAVE_SYS_PARAM_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "users@gecode.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GECODE"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GECODE 4.3.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gecode"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.3.0"

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1
