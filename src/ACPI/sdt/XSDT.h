#ifndef XSDT_H
#define XSDT_H

#include "../../Attributes.h"
#include "DescriptionHeader.h"

struct XSDT : public DescriptionHeader {
    DescriptionHeader *tables[0];

    int getTablesCount();

    void parseTables();
} PACKED;

extern XSDT *GlobalXSDT;
#endif //XSDT_H
