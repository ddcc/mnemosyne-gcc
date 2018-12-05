#include "heap.hh"

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <mnemosyne.h>


static int constructor(PMEMobjpool *pop, void *ptr, void *arg) {
    return 0;
}

Heap::Heap()
{
    #define PERSISTENT_NAME     "/pmem-fs/pmdk"
    #define PERSISTENT_LAYOUT   POBJ_LAYOUT_NAME(pmdk)
    #define PERSISTENT_SIZE     8UL * 1024 * 1024 * 1024

    if (!(objpool = pmemobj_open(PERSISTENT_NAME, PERSISTENT_LAYOUT))) {
        if (!(objpool = pmemobj_create(PERSISTENT_NAME, PERSISTENT_LAYOUT, PERSISTENT_SIZE, S_IWUSR | S_IRUSR)))
            abort();
    }
}

void* Heap::pmalloc(size_t sz)
{
    PMEMoid oid;
    int ret = pmemobj_alloc(objpool, &oid, sz, sz, constructor, NULL);
    if (!ret)
        return pmemobj_direct(oid);
    return NULL;
}

void* Heap::pcalloc(size_t nelem, size_t sz)
{
    PMEMoid oid;
    int ret = pmemobj_xalloc(objpool, &oid, nelem * sz, sz, POBJ_XALLOC_ZERO, constructor, NULL);
    if (!ret)
        return pmemobj_direct(oid);
    return NULL;
}

void Heap::pfree(void* ptr)
{
    PMEMoid oid = pmemobj_oid(ptr);
    if (!OID_IS_NULL(oid))
        pmemobj_free(&oid);
}

size_t Heap::getsize(void* ptr)
{
    PMEMoid oid = pmemobj_oid(ptr);
    if (!OID_IS_NULL(oid))
        return pmemobj_alloc_usable_size(oid);
}
