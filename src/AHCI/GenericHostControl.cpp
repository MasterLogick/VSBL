#include "GenericHostControl.h"

bool GenericHostControl::is64AddressCapable() {
    return hostCaps & (1 << 31);
}
