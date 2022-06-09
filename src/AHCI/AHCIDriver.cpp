#include "AHCIDriver.h"
#include <iostream.h>
#include <PCI/PCIConfigSpaceRegister.h>
#include <VirtualMemoryManager.h>
#include "HBAPortController.h"
#include "SerialATA/RegisterH2DFIS.h"

AHCIDriver::AHCIDriver(PCIDeviceConfigSpace device) : deviceConfigSpace(device) {
    cout << "AHCI:AHCI HBA detected at " << deviceConfigSpace.bus << ":" << deviceConfigSpace.device << ":"
         << deviceConfigSpace.function << endl;
    uint32_t address = device.getBAR(5)();
    cout << "AHCI:HBA memory address: 0x" << basic_stream::HEX << address << basic_stream::DECIMAL << endl;
    address &= ~((1 << 13) - 1);
    hbaMemoryRegister = reinterpret_cast<HBAMemoryRegister *>(address);
    cout << "AHCI:AHCI version: "
         << hbaMemoryRegister->genericHostControl.version.superMajor
         << hbaMemoryRegister->genericHostControl.version.major
         << "."
         << hbaMemoryRegister->genericHostControl.version.minor
         << hbaMemoryRegister->genericHostControl.version.subMinor
         << endl;
    cout << "AHCI:HBA ";
    if (this->hbaMemoryRegister->genericHostControl.is64AddressCapable()) {
        cout << "supports 64-bit addressing" << endl;
    } else {
        cout << "does not support 64-bit addressing" << endl;
    }
    cout << "AHCI:available ports:";
    uint32_t ports = hbaMemoryRegister->genericHostControl.portsImplemented;
    if (ports) {
        for (int i = 0, j = 1; i < 32; ++i, j <<= 1) {
            if (j & ports) {
                cout << " " << i;
                availablePorts.push_back(i);
            }
        }
    } else {
        cout << " none";
    }
    cout << endl;
    cout << "AHCI:HBA works: " << hbaMemoryRegister->genericHostControl.hostCaps << endl;
    if (ports) {
        for (int i = 0, j = 1; i < 32; ++i, j <<= 1) {
            if (j & ports) {
                PortControlRegister *port = hbaMemoryRegister->ports + i;
                HBAPortController *controller = new HBAPortController(port);
                controller->init();
                cout << "AHCI:port " << i << " SATA status: " << port->sataStatus << endl;
                cout << "AHCI:port " << i << " SATA error: " << port->sataError << endl;
                cout << "AHCI:port " << i << " command list address: " << port->commandListBaseAddress << endl;
                cout << "AHCI:port " << i << " received list address: " << port->recievedFIS << endl;
                Command c{};
                RegisterH2DFIS devDiag{};
                memset(&devDiag, 0, sizeof(RegisterH2DFIS));
                devDiag.sectCount = 0;
                devDiag.command = 0xec;
                devDiag.c = 1;
                c.write = false;
                devDiag.fisType = RegisterH2D;
                c.commandFis = &devDiag;
                c.fisBSize = sizeof(RegisterH2DFIS);
                char *data = new(GlobalVMM->pageAllocateAligned(0x200, 2))char[200];
                PhysicalRegionDescriptor prd{};
                prd.byteCount = 0x200;
                prd.dataBase = data;
                prd.intOnCompletion = true;
                c.prdt = &prd;
                c.prdtl = 1;
                cout << "AHCI:port interrupt status: " << port->interruptStatus << endl;
                bool result = controller->execute(&c);
                cout << "AHCI:port exec result: " << result << endl;
                cout << "AHCI:port interrupt status: " << port->interruptStatus << endl;
                cout << "AHCI:port sata error: " << port->sataError << endl;
                cout << "AHCI:port device diagnostic result: " << port->taskFileData << endl;
                cout << "AHCI:port identity serial: ";
                for (int k = 20; k < 40; ++k) {
                    cout << data[k];
                }
                cout << endl;
                cout << "AHCI:port identity firmware: ";
                for (int k = 23 * 2; k < 27 * 2; ++k) {
                    cout << data[k];
                }
                cout << endl;
                cout << "AHCI:port identity model: ";
                for (int k = 27 * 2; k < 47 * 2; ++k) {
                    cout << data[k];
                }
                cout << endl;
                devDiag.sectCount = 1;
                devDiag.device = 1 << 6;
                devDiag.command = 0xc8;
                result = controller->execute(&c);
                cout << "AHCI:port exec result: " << result << endl;
                cout << "AHCI:port interrupt status: " << port->interruptStatus << endl;
                cout << "AHCI:port sata error: " << port->sataError << endl;
                cout << "AHCI:port device diagnostic result: " << port->taskFileData << endl;
                cout << "AHCI:read zero sector: ";
                for (int k = 0; k < 0x200; ++k) {
                    cout << data[k];
                }
                cout << endl;
                break;
            }
        }
    }
    cout << endl;
}
