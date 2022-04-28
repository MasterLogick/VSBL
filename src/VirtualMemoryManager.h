#ifndef VIRTUALMEMORYMANAGER_H
#define VIRTUALMEMORYMANAGER_H

#include <stddef.h>
#include <stdint.h>
#include "Attributes.h"
#include "PhysicalMemoryManager.h"
#include "VirtualMemoryBlock.h"
#include "AVLTree.h"
#include "Vector.h"

#define VMM_PAGE_SIZE 1024

class VirtualMemoryManager;

extern VirtualMemoryManager *GlobalVMM;

class VirtualMemoryManager {
    template<typename>
    struct PageAllocator;

    typedef AVLTree<length_t, Vector<VirtualMemoryBlock, PageAllocator<void>> *, PageAllocator<void>> FreeLengthMemoryTree;
    typedef AVLTree<base_t, VirtualMemoryBlock, PageAllocator<void>> UsedMemoryTree;
    typedef AVLTree<base_t, VirtualMemoryBlock, PageAllocator<void>> FreeBaseMemoryTree;

    template<typename T>
    struct PageAllocator {
        typedef T ValueType;
        typedef size_t SizeType;

        template<typename U>
        struct Rebind {
            typedef PageAllocator<U> Other;
        };

        inline T *allocate(SizeType n) {
            return reinterpret_cast<T *>(GlobalVMM->pageAllocateAligned(sizeof(T) * n));
        }

        /**
         * Private virtual memory manager page allocator doesn't deallocate pages to prevent recursive calls
         * */
        void deallocate(T *ptr) {
            (void) ptr;
        }
    };

public:
    VirtualMemoryBlock pageDescriptor;
private:
    UsedMemoryTree *usedMemory;
    FreeLengthMemoryTree *freeLengthMemory;
public:
    FreeBaseMemoryTree *freeBaseMemory;
private:
    uint8_t firstPage[VMM_PAGE_SIZE];

    void insertFreeBlock(VirtualMemoryBlock &block);

    void removeFreeBlock(VirtualMemoryBlock &block);

public:

    VirtualMemoryManager();

    void *malloc(size_t size);

    void free(void *ptr);

    void *pageAllocateAligned(size_t n, size_t align = 1);
};

#endif //VIRTUALMEMORYMANAGER_H
