#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include "PML4Entry.h"
#include "PDPTEntry.h"
#include "PDEntry.h"

class PageManager {
    PML4Entry *PML4Table;

    void initPML4Entry(PML4Entry *entry);

    void initPDPTEntryDirectory(PDPTEntryDirectory *entry);

    void initPDEntryPage(PDEntryPage *entry);

public:
    PageManager();
};

extern PageManager GlobalPageManager;

#ifdef __cplusplus
extern "C" {
#endif

void _paging_load_PML4Table(PML4Entry *table);

#ifdef __cplusplus
}
#endif

#endif //PAGEMANAGER_H
