#ifndef LINUX_PLATFORM_H
#define LINUX_PLATFORM_H
namespace sablin{
#if PLATFORM_INDEX == LINUX_PLATFORM_INDEX
//Memory Alignment
#if PLATFORM_ARCH == PLATFORM_ARCH_32
#define MEMORY_ALIGNMENT_DEFAULT 8u
#elif PLATFORM_ARCH == PLATFORM_ARCH_64
#define MEMORY_ALIGNMENT_DEFAULT 16u
#else
#error PLATFORM ARCH ERROR! SUPPORTED ARCH: x86_32, amd64
#endif

#define MEMORY_ALIGNMENT_MIN 4u
#define MEMORY_ALIGNMENT_MAX 16u

#endif

}
#endif
