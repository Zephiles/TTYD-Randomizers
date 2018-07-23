#include "clearcache.h"

#include <ttyd/OSCache.h>

#include <cstdint>

namespace mod::clearcache {

void clearCache(uint32_t Address, uint32_t BytesToClear)
{
  ttyd::OSCache::DCFlushRange(reinterpret_cast<void *>(Address), BytesToClear);
  ttyd::OSCache::ICInvalidateRange(reinterpret_cast<void *>(Address), BytesToClear);
}

}