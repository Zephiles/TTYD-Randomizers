#pragma once

#include <cstdint>

namespace ttyd::OSCache {

extern "C" {

// DCEnable
// DCInvalidateRange
void DCFlushRange(void *startAddress, uint32_t numberOfBytesToFlush);
// DCStoreRange
// DCFlushRangeNoSync
// DCStoreRangeNoSync
// DCZeroRange
void ICInvalidateRange(void *startAddress, uint32_t numberOfBytesToInvalidate);
// ICFlashInvalidate
// ICEnable
// __LCEnable
// LCEnable
// LCDisable
// LCStoreBlocks
// LCStoreData
// LCQueueWait
// L2GlobalInvalidate
// DMAErrorHandler
// __OSCacheInit

}

}