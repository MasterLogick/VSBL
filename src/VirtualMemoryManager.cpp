#include "VirtualMemoryManager.h"
#include "iostream.h"
#include "Attributes.h"

extern char Kernel_Start;
extern char Kernel_End;

VirtualMemoryManager *GlobalVMM;

NORETURN void VIRTUAL_MEMORY_MANAGER_PANIC(const char *message) {
    cout << "VMM:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nVMM: PANIC: " << message
         << "\nVMM:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nEXECUTION TERMINATED\n";
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
            if (block.length) {
                insertFreeBlock(block);
            }
            if (high.length) {
                insertFreeBlock(high);
            }
        } else {
            insertFreeBlock(block);
        }
    }
    if (!blockFound) {
        VIRTUAL_MEMORY_MANAGER_PANIC("KERNEL IS LOCATED OUTSIDE OF CONVENTIONAL MEMORY");
    }
}

void VirtualMemoryManager::insertFreeBlock(VirtualMemoryBlock &block) {
    VirtualMemoryBlock *lowPtr = freeBaseMemory->search(LESS, block.base);
    base_t begin;
    if (lowPtr && lowPtr->end() + 1 >= block.base) {
        begin = lowPtr->base;
        removeFreeBlock(*lowPtr);
    } else {
        begin = block.base;
    }
    VirtualMemoryBlock *highPtr = freeBaseMemory->search(GREATER, block.base);
    base_t end;
    while (highPtr && highPtr->end() <= block.end()) {
        removeFreeBlock(*highPtr);
        highPtr = freeBaseMemory->search(GREATER, block.base);
    }
    if (highPtr && highPtr->base <= block.end() + 1) {
        end = highPtr->end();
        removeFreeBlock(*highPtr);
    } else {
        end = block.end();
    }
    VirtualMemoryBlock merged = VirtualMemoryBlock(begin, end - begin + 1);
    freeBaseMemory->insert(begin, merged);
    Vector<VirtualMemoryBlock, PageAllocator<void>> **vecPtr = freeLengthMemory->search(EQUAL, merged.length);
    Vector<VirtualMemoryBlock, PageAllocator<void>> *vec;
    if (!vecPtr) {
        vec = new(pageAllocateAligned(
                sizeof(Vector<VirtualMemoryBlock, PageAllocator<void>>)))Vector<VirtualMemoryBlock, PageAllocator<void>>();
        freeLengthMemory->insert(merged.length, vec);
    } else {
        vec = *vecPtr;
    }
    vec->push_back(merged);
}

void VirtualMemoryManager::removeFreeBlock(VirtualMemoryBlock &block) {
    freeBaseMemory->remove(block.base);
    Vector<VirtualMemoryBlock, PageAllocator<void>> **ptr = freeLengthMemory->search(EQUAL, block.length);
    if (ptr) {
        Vector<VirtualMemoryBlock, PageAllocator<void>> *vec = *ptr;
        for (int i = 0; i < vec->getSize(); ++i) {
            if (block.base == vec->operator[](i).base) {
                vec->remove(i);
                i--;
            }
        }
        if (vec->getSize() == 0) {
            freeLengthMemory->remove(block.length);
        }
    }
}

void *VirtualMemoryManager::pageAllocateAligned(size_t n, size_t align) {
    base_t alignedBase = ((pageDescriptor.base + sizeof(UsedMemoryTree::Node) + align - 1) / align) * align;
    if (alignedBase + n - 1 <= pageDescriptor.end()) {
        if (alignedBase - sizeof(UsedMemoryTree::Node) != pageDescriptor.base) {
            VirtualMemoryBlock block = VirtualMemoryBlock(pageDescriptor.base,
                                                          alignedBase - sizeof(UsedMemoryTree::Node) -
                                                          pageDescriptor.base);
            insertFreeBlock(block);
            pageDescriptor = VirtualMemoryBlock(alignedBase - sizeof(UsedMemoryTree::Node),
                                                pageDescriptor.end() - alignedBase + sizeof(UsedMemoryTree::Node) + 1);
        }
        pageDescriptor.length -= n + sizeof(UsedMemoryTree::Node);
        UsedMemoryTree::Node *descriptor = new(reinterpret_cast<void *>(pageDescriptor.base))UsedMemoryTree::Node(
                n + sizeof(UsedMemoryTree::Node),
                VirtualMemoryBlock(pageDescriptor.base, n + sizeof(UsedMemoryTree::Node)));
        pageDescriptor.base += n + sizeof(UsedMemoryTree::Node);
        usedMemory->insertNode(descriptor);
        return reinterpret_cast<void *>(descriptor->val.base + sizeof(UsedMemoryTree::Node));
    } else {
        length_t nextLength = n - 1;
        while (true) {
            Vector<VirtualMemoryBlock, PageAllocator<void>> **vecPtr =
                    freeLengthMemory->search(GREATER, nextLength + sizeof(UsedMemoryTree::Node) + VMM_PAGE_SIZE);
            if (vecPtr) {
                Vector<VirtualMemoryBlock, PageAllocator<void>> *vec = *vecPtr;
                for (int i = 0; i < vec->getSize(); ++i) {
                    VirtualMemoryBlock &descriptor = vec->operator[](i);
                    nextLength = descriptor.length;
                    alignedBase = (descriptor.base + sizeof(UsedMemoryTree::Node) + align - 1) / align;
                    if (alignedBase + n - 1 <= descriptor.end()) {
                        VirtualMemoryBlock prevPD = pageDescriptor;
                        pageDescriptor = descriptor;
                        if (pageDescriptor.length) {
                            insertFreeBlock(prevPD);
                        }
                        removeFreeBlock(pageDescriptor);
                        return pageAllocateAligned(n, align);
                    }
                }
            } else {
                VIRTUAL_MEMORY_MANAGER_PANIC("NOT ENOUGH MEMORY");
            }
        }
    }
}

void *VirtualMemoryManager::malloc(size_t size) {
    return pageAllocateAligned(size);
}

void VirtualMemoryManager::free(void *ptr) {
    VirtualMemoryBlock *blockPtr = usedMemory->search(LESS_OR_EQUAL, reinterpret_cast<base_t>(ptr));
    if (blockPtr) {
        VirtualMemoryBlock block = *blockPtr;
        usedMemory->remove(block.base);
        insertFreeBlock(block);
    } else {
        VIRTUAL_MEMORY_MANAGER_PANIC("VIRTUAL MEMORY FREE ERROR");
    }
}
