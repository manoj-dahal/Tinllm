#ifndef COLI_COMPAT_H
#define COLI_COMPAT_H

#if defined(_WIN32) || defined(_WIN64)
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
  #endif
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  #include <psapi.h>
  #include <io.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <direct.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdint.h>
  #include <string.h>
  #include <time.h>

  #define COLI_PATH_SEP '\\'
  #define O_CLOEXEC 0
  #ifndef O_BINARY
    #define O_BINARY 0
  #endif
  #define COMPAT_O_RDONLY (O_RDONLY | O_BINARY)

  static inline void* compat_aligned_alloc(size_t align, size_t size) {
      return _aligned_malloc(size, align);
  }
  static inline void compat_aligned_free(void* ptr) {
      _aligned_free(ptr);
  }

  static inline double compat_time_sec(void) {
      static LARGE_INTEGER freq;
      static int init = 0;
      if (!init) { QueryPerformanceFrequency(&freq); init = 1; }
      LARGE_INTEGER counter;
      QueryPerformanceCounter(&counter);
      return (double)counter.QuadPart / (double)freq.QuadPart;
  }

  static inline double compat_rss_gb(void) {
      PROCESS_MEMORY_COUNTERS pmc;
      if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
          return (double)pmc.WorkingSetSize / (1024.0 * 1024.0 * 1024.0);
      }
      return 0.0;
  }

  static inline double compat_ram_total_gb(void) {
      MEMORYSTATUSEX ms;
      ms.dwLength = sizeof(ms);
      if (GlobalMemoryStatusEx(&ms)) return (double)ms.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
      return 0.0;
  }

  static inline double compat_ram_avail_gb(void) {
      MEMORYSTATUSEX ms;
      ms.dwLength = sizeof(ms);
      if (GlobalMemoryStatusEx(&ms)) return (double)ms.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
      return 0.0;
  }

#else
  #define COLI_PATH_SEP '/'
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdint.h>
  #include <string.h>
  #include <time.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <sys/time.h>
  #include <sys/stat.h>

  #ifndef COMPAT_O_RDONLY
    #define COMPAT_O_RDONLY O_RDONLY
  #endif

  #if defined(__APPLE__)
    #include <mach/mach.h>
    #include <sys/mount.h>
    #include <sys/sysctl.h>
  #elif defined(__linux__)
    #include <sys/sysinfo.h>
  #endif

  static inline void* compat_aligned_alloc(size_t align, size_t size) {
      void* ptr = NULL;
      if (posix_memalign(&ptr, align, size) != 0) return NULL;
      return ptr;
  }
  static inline void compat_aligned_free(void* ptr) {
      free(ptr);
  }

  static inline double compat_time_sec(void) {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
  }

  static inline double compat_rss_gb(void) {
#if defined(__APPLE__)
      struct mach_task_basic_info info;
      mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
      if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
          return (double)info.resident_size / (1024.0 * 1024.0 * 1024.0);
      }
      return 0.0;
#elif defined(__linux__)
      FILE* f = fopen("/proc/self/statm", "r");
      if (!f) return 0.0;
      long pages = 0;
      if (fscanf(f, "%*s %ld", &pages) == 1) {
          fclose(f);
          return (double)pages * (double)sysconf(_SC_PAGESIZE) / (1024.0 * 1024.0 * 1024.0);
      }
      fclose(f);
      return 0.0;
#else
      return 0.0;
#endif
  }

  static inline double compat_ram_total_gb(void) {
#if defined(__APPLE__)
      int mib[2] = { CTL_HW, HW_MEMSIZE };
      uint64_t mem = 0;
      size_t len = sizeof(mem);
      if (sysctl(mib, 2, &mem, &len, NULL, 0) == 0) return (double)mem / (1024.0 * 1024.0 * 1024.0);
      return 0.0;
#elif defined(__linux__)
      struct sysinfo si;
      if (sysinfo(&si) == 0) return (double)si.totalram * (double)si.mem_unit / (1024.0 * 1024.0 * 1024.0);
      return 0.0;
#else
      return 0.0;
#endif
  }

  static inline double compat_ram_avail_gb(void) {
#if defined(__APPLE__)
      mach_port_t host = mach_host_self();
      vm_statistics64_data_t vm_stat;
      mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
      if (host_statistics64(host, HOST_VM_INFO64, (host_info64_t)&vm_stat, &count) == KERN_SUCCESS) {
          double pagesize = (double)sysconf(_SC_PAGESIZE);
          double free_bytes = ((double)vm_stat.free_count + (double)vm_stat.inactive_count) * pagesize;
          return free_bytes / (1024.0 * 1024.0 * 1024.0);
      }
      return 0.0;
#elif defined(__linux__)
      FILE* f = fopen("/proc/meminfo", "r");
      if (f) {
          char line[256];
          while (fgets(line, sizeof(line), f)) {
              uint64_t kb = 0;
              if (sscanf(line, "MemAvailable: %lu kB", &kb) == 1) {
                  fclose(f);
                  return (double)kb / (1024.0 * 1024.0);
              }
          }
          fclose(f);
      }
      struct sysinfo si;
      if (sysinfo(&si) == 0) return (double)si.freeram * (double)si.mem_unit / (1024.0 * 1024.0 * 1024.0);
      return 0.0;
#else
      return 0.0;
#endif
  }
#endif

/* TinLLM API Compatibility Alias Shims */
static inline void* tin_aligned_alloc(size_t align, size_t size) {
    return compat_aligned_alloc(align, size);
}

static inline void tin_aligned_free(void* ptr) {
    compat_aligned_free(ptr);
}

static inline double tin_time_sec(void) {
    return compat_time_sec();
}

static inline size_t tin_get_available_ram_mb(void) {
    return (size_t)(compat_ram_avail_gb() * 1024.0);
}

#endif /* COLI_COMPAT_H */
