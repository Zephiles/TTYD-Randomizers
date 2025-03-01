#include <ttyd/memory.h>

void *operator new(std::size_t size)
{
    return ttyd::memory::__memAlloc(0, size);
}
void *operator new[](std::size_t size)
{
    return ttyd::memory::__memAlloc(0, size);
}
void operator delete(void *ptr)
{
    return ttyd::memory::__memFree(0, ptr);
}
void operator delete[](void *ptr)
{
    return ttyd::memory::__memFree(0, ptr);
}

void operator delete(void *ptr, std::size_t size)
{
    (void)size;
    ttyd::memory::__memFree(0, ptr);
}

void operator delete[](void *ptr, std::size_t size)
{
    (void)size;
    ttyd::memory::__memFree(0, ptr);
}
