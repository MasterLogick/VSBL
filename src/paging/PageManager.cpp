#include "PageManager.h"
#include <iostream.h>

PageManager GlobalPageManager;
uint8_t pageStruct[
        sizeof(PML4Entry[512]) + sizeof(PDPTEntryDirectory[512]) + sizeof(PDEntryPage[512]) * 4] ALIGNED(4096);

PageManager::PageManager() {
    uint64_t *tables = reinterpret_cast<uint64_t *>(pageStruct);
    memset(tables, 0, sizeof(PML4Entry[512]) + sizeof(PDPTEntryDirectory[512]) + sizeof(PDEntryPage[512]) * 4);
    PML4Table = new(tables)PML4Entry();
    tables += 512;
    initPML4Entry(PML4Table);
    PDPTEntryDirectory *PDPTEtable = new(tables) PDPTEntryDirectory[512];
    tables += 512;
    PML4Table[0].address = reinterpret_cast<uint64_t>(PDPTEtable) >> 12;
    for (long i = 0; i < 4; ++i) {
        initPDPTEntryDirectory(PDPTEtable + i);
        PDEntryPage *pages = new(tables)PDEntryPage[512];
        tables += 512;
        PDPTEtable[i].address = reinterpret_cast<uint64_t>(pages) >> 12;
        for (long j = 0; j < 512; ++j) {
            initPDEntryPage(pages + j);
            pages[j].address = j + i * 512;
        }
    }
    _paging_load_PML4Table(PML4Table);
}

void PageManager::initPML4Entry(PML4Entry *entry) {
    entry->present = 1;
    entry->rw = 1;
    entry->user = 0;
    entry->pwt = 0;
    entry->pcd = 0;
    entry->accessed = 0;
    entry->dirty = 0;
    entry->protectionKey = 0;
    entry->execDisable = 0;
}

void PageManager::initPDPTEntryDirectory(PDPTEntryDirectory *entry) {
    entry->present = 1;
    entry->rw = 1;
    entry->user = 0;
    entry->pwt = 0;
    entry->pcd = 0;
    entry->accessed = 0;
    entry->dirty = 0;
    entry->type = 0;
    entry->protectionKey = 0;
    entry->protectionKey = 0;
    entry->execDisable = 0;
}

void PageManager::initPDEntryPage(PDEntryPage *entry) {
    entry->present = 1;
    entry->rw = 1;
    entry->user = 0;
    entry->pwt = 0;
    entry->pcd = 0;
    entry->accessed = 0;
    entry->dirty = 0;
    entry->type = 1;
    entry->global = 1;
    entry->pat = 0;
    entry->protectionKey = 0;
    entry->protectionKey = 0;
    entry->execDisable = 0;
}