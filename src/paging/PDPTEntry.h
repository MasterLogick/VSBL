#ifndef PDPTENTRY_H
#define PDPTENTRY_H

#include "PageStructureEntry.h"

PageStrcutureEntry(PDPTEntryPage, type:
    1, global:
                       1, : 3, pat:
                       1, : 17, address:
                       22)

PageStrcutureEntry(PDPTEntryDirectory, type:
    1, : 4, address:
                       40)

#endif //PDPTENTRY_H
