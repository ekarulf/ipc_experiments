#ifndef STRING_COMPAT_H_Z6W0MAIR
#define STRING_COMPAT_H_Z6W0MAIR

#include <string.h>

#ifdef __LINUX__
size_t strlcat(char * __restrict dst, const char * __restrict src, size_t siz);
size_t strlcpy(char * __restrict dst, const char * __restrict src, size_t siz);
#endif

#endif /* end of include guard: STRING_COMPAT_H_Z6W0MAIR */
