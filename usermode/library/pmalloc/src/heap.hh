#ifndef _MNEMOSYNE_HEAP_HEAP_HH
#define _MNEMOSYNE_HEAP_HEAP_HH

#include <libpmemobj.h>

#include <mnemosyne.h>
#include <mtm.h>
#include <mtm_i.h>
#include <itm.h>

extern "C" void _ITM_nl_load_bytes(const void *src, void *dest, size_t size);
extern "C" void _ITM_nl_store_bytes(const void *src, void *dest, size_t size);

class Context {
public:
    Context(bool _do_v = true, bool _do_nv = true)
        : do_v(_do_v),
          do_nv(_do_nv)
    {
        if (_ITM_inTransaction()) {
            td = _ITM_getTransaction();
        } else {
            td = NULL;
        }
    }

    void load(uint8_t* src, uint8_t *dest, size_t size)
    {
        if (td) {
            _ITM_nl_load_bytes(src, dest, size);
        } else {
            memcpy(dest, src, size);
        }
    }

    void store(uint8_t* src, uint8_t *dest, size_t size)
    {
        if (td) {
            _ITM_nl_store_bytes(src, dest, size);
        } else {
            memcpy(dest, src, size);
        }
    }

    bool do_v;
    bool do_nv;

    _ITM_transaction * td;
};

class Heap {
public:
    Heap();
    void* pmalloc(size_t sz);
    void* pcalloc(size_t nelem, size_t sz);
    void pfree(void* ptr);
    size_t getsize(void* ptr);

private:
    PMEMobjpool *objpool;
};

#endif // _MNEMOSYNE_HEAP_HEAP_HH
