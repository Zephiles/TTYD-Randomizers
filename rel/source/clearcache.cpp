#include "clearcache.h"

#include <ttyd/OSCache.h>

#include <cstdint>

namespace clearcache
{
    void clear_DC_IC_Cache(void *ptr, uint32_t size)
    {
        ttyd::OSCache::DCFlushRange(ptr, size);
        ttyd::OSCache::ICInvalidateRange(ptr, size);
    }
} // namespace clearcache
