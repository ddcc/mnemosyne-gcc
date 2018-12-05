#include <stdlib.h>
#include <string.h>

#include <mutex>

#include "heap.hh"

#include <mtm_i.h>
#include <itm.h>

static Heap* heap;
std::mutex heapmtx;

inline static Heap * getHeap (void)
{
    heapmtx.lock();
    if (heap) {
        heapmtx.unlock();
        return heap;
    }
    heap = new Heap();
    heapmtx.unlock();
    return heap;
}

extern "C"
void mtm_init_heap() {
    getHeap();
}

extern "C"
void * mtm_pmalloc (size_t sz)
{
    void* addr;

    Heap* heap = getHeap();
    addr = heap->pmalloc(sz);

	return addr;
}

extern "C"
void mtm_pmalloc_undo (void* ptr)
{
    Heap* heap = getHeap();
    heap->pfree(ptr);
}


extern "C"
void * mtm_pcalloc (size_t nelem, size_t elsize)
{
    void* addr;

    Heap* heap = getHeap();
    addr = heap->pcalloc(nelem, elsize);

    return addr;
}


extern "C"
void mtm_pfree (void* ptr)
{
    // TODO
    // Do we need this?
    // pfree like pmalloc shall only be called inside a transaction to ensure
    // failure atomicity
}

extern "C"
void mtm_pfree_prepare (void* ptr)
{
    Heap* heap = getHeap();
    heap->pfree(ptr);
}

extern "C"
void mtm_pfree_commit (void* ptr)
{
    Heap* heap = getHeap();
    heap->pfree(ptr);
}

extern "C"
size_t mtm_get_obj_size(void *ptr)
{
    Heap* heap = getHeap();
    return heap->getsize(ptr);
}

extern "C" void * mtm_prealloc (void * ptr, size_t sz)
{
    //TODO

	return NULL;
}
