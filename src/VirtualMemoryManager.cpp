#include "VirtualMemoryManager.h"
#include "terminal.h"
#include "Attributes.h"

extern "C" {
extern char Kernel_Start;
extern char Kernel_End;
}

VirtualMemoryManager GlobalVMM;

NORETURN void VIRTUAL_MEMORY_MANAGER_PANIC(const char *message) {
    terminal_printf(
            "VMM:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nVMM: PANIC: %s\nVMM:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", message);
    while (true);
}

VirtualMemoryManager::VirtualMemoryManager() :
        pageDescriptor(reinterpret_cast<base_t>(firstPage + sizeof(UsedMemoryTree) + sizeof(FreeLengthMemoryTree) +
                                                sizeof(FreeBaseMemoryTree)),
                       sizeof(firstPage) - (sizeof(UsedMemoryTree) + sizeof(FreeLengthMemoryTree) +
                                            sizeof(FreeBaseMemoryTree))) {
    usedMemory = new(firstPage)UsedMemoryTree();
    freeLengthMemory = new(firstPage + sizeof(UsedMemoryTree))FreeLengthMemoryTree();
    freeBaseMemory = new(firstPage + sizeof(UsedMemoryTree) + sizeof(FreeLengthMemoryTree))FreeBaseMemoryTree();
    VirtualMemoryBlock kernel{reinterpret_cast<base_t>(&Kernel_Start),
                              reinterpret_cast<length_t>(&Kernel_End) - reinterpret_cast<length_t>(&Kernel_Start) + 1};

    if (reinterpret_cast<base_t>(this) < kernel.base ||
        reinterpret_cast<base_t>(this) + sizeof(VirtualMemoryManager) > kernel.base + kernel.length) {
        VIRTUAL_MEMORY_MANAGER_PANIC("VMM IS LOCATED OUTSIDE OF KERNEL MEMORY");
    }
    bool blockFound = false;
    for (int i = 0; i < GlobalPMM.conventionalBlocksCount; ++i) {
        VirtualMemoryBlock block(GlobalPMM.conventionalMemory[i]);
        if (block.overlays(kernel)) {
            VirtualMemoryBlock high;
            block.split(kernel, &high);
            blockFound = true;
            usedMemory->insert(kernel.base, kernel);
            if (block.length) {
                insertFreeBlock(block);
            }
            if (high.length) {
                insertFreeBlock(high);
            }
            usedMemory->insert(kernel.base, kernel);
        } else {
            insertFreeBlock(block);
        }
    }
    if (!blockFound) {
        VIRTUAL_MEMORY_MANAGER_PANIC("VMM IS LOCATED OUTSIDE OF CONVENTIONAL MEMORY");
    }
}

void VirtualMemoryManager::insertFreeBlock(VirtualMemoryBlock &block) {
    VirtualMemoryBlock *lowPtr = freeBaseMemory->search(LESS, block.base);
    VirtualMemoryBlock low;
    if (lowPtr && lowPtr->end() + 1 >= block.base) {
        low = *lowPtr;
        removeFreeBlock(*lowPtr);
    } else {
        low = block;
    }
    VirtualMemoryBlock *highPtr = freeBaseMemory->search(GREATER, block.base);
    VirtualMemoryBlock high;
    while (highPtr && highPtr->end() <= block.end()) {
        removeFreeBlock(*highPtr);
        highPtr = freeBaseMemory->search(GREATER, block.base);
    }
    if (highPtr && highPtr->base <= block.end() + 1) {
        high = *highPtr;
        removeFreeBlock(*highPtr);
    } else {
        high = block;
    }
    low = VirtualMemoryBlock(min(low.base, block.base), max(block.end(), high.end()) - min(low.base, block.base) + 1);
    freeBaseMemory->insert(low.base, low);
    Vector<VirtualMemoryBlock, PageAllocator<void>> **vecPtr = freeLengthMemory->search(EQUAL, low.length);
    Vector<VirtualMemoryBlock, PageAllocator<void>> *vec;
    if (!vecPtr) {
        vec = new(pageAllocate(
                sizeof(Vector<VirtualMemoryBlock, PageAllocator<void>>)))Vector<VirtualMemoryBlock, PageAllocator<void>>();
        freeLengthMemory->insert(low.length, vec);
    } else {
        vec = *vecPtr;
    }
    vec->push_back(low);
}

void VirtualMemoryManager::removeFreeBlock(VirtualMemoryBlock &block) {
    freeBaseMemory->remove(block.base);
    Vector<VirtualMemoryBlock, PageAllocator<void>> **ptr = freeLengthMemory->search(EQUAL, block.length);
    if (ptr) {
        Vector<VirtualMemoryBlock, PageAllocator<void>> *vec = *ptr;
        for (size_t i = 0; i < vec->getSize(); ++i) {
            if (block.base == vec->operator[](i).base) {
                vec->remove(i);
                if (vec->getSize() == 0) {
                    freeLengthMemory->remove(block.length);
                }
                return;
            }
        }
    }
}

void *VirtualMemoryManager::pageAllocate(size_t n) {
    if (n < pageDescriptor.length) {
        pageDescriptor.length += n;
        void *retVal = reinterpret_cast<void *>(pageDescriptor.base);
        pageDescriptor.base += n;
        return retVal;
    } else {
        Vector<VirtualMemoryBlock, PageAllocator<void>> **vecPtr = freeLengthMemory->search(GREATER_OR_EQUAL,
                                                                                            n + VMM_PAGE_SIZE);
        if (vecPtr) {
            pageDescriptor = (*vecPtr)->operator[]((*vecPtr)->getSize());
            removeFreeBlock(pageDescriptor);
            return pageAllocate(n);
        } else {
            VIRTUAL_MEMORY_MANAGER_PANIC("NOT ENOUGH MEMORY");
        }
    }
}

void *VirtualMemoryManager::malloc(size_t size) {
    (void) size;
    return nullptr;
}

void VirtualMemoryManager::free(void *ptr) {
    (void) ptr;
}
