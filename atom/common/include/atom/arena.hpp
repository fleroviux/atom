
#pragma once

#include <atom/integer.hpp>
#include <atom/panic.hpp>
#include <cstdlib>

#if defined(WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#elif defined(__APPLE__)
  #include <sys/mman.h>
#endif

//#define ATOM_ARENA_USE_MALLOC

namespace atom {

class Arena {
  public:
    explicit Arena(size_t capacity) {
#if defined(WIN32) && !defined(ATOM_ARENA_USE_MALLOC)
      m_base_address = (u8*)VirtualAlloc(nullptr, capacity, MEM_COMMIT, PAGE_READWRITE);
#elif defined(__APPLE__) && !defined(ATOM_ARENA_USE_MALLOC)
      m_base_address = (u8*)mmap(nullptr, capacity, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#else
      m_base_address = (u8*)std::malloc(capacity);
#endif

      if(m_base_address == nullptr) {
        ATOM_PANIC("atom: out of memory");
      }
      m_maximum_address = m_base_address + capacity;
      Reset();
    }

   ~Arena() {
      const size_t capacity = m_maximum_address - m_base_address;

#if defined(WIN32) && !defined(ATOM_ARENA_USE_MALLOC)
      VirtualFree(m_base_address, capacity, MEM_RELEASE);
#elif defined(__APPLE__) && !defined(ATOM_ARENA_USE_MALLOC)
      munmap(m_base_address, capacity);
#else
     (void)capacity;
      std::free(m_base_address);
#endif
    }

    void Reset() {
      m_current_address = m_base_address;
    }

    void* Allocate(size_t number_of_bytes) {
      u8* address = m_current_address;
      m_current_address += number_of_bytes;
      if(m_current_address <= m_maximum_address) {
        return address;
      }
      return nullptr;
    }

  private:
    u8* m_base_address{};
    u8* m_current_address{};
    u8* m_maximum_address{};
};

}  // namespace atom
