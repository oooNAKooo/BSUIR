#include "stdafx.h"
#include "hexioctrl.h"
#include "targetver.h"
#include "(pci_devs)pci_codes.h"

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

u32 calculateAddress(u32 bus, u32 device, u32 function, u32 reg)
{
    u32 address = 1;
    address = address << 15;
    address += bus;
    address = address << 5;
    address += device;
    address = address << 3;
    address += function;
    address = address << 8;
    address += reg;
    return address;
}

void getDeviceInfo(u32 bus, u32 device, u32 func, u32 reg) {
    u32 configAddress = calculateAddress(bus, device, func, reg);
    u32 regData;

    _asm {
        mov eax, configAddress
        mov edx, 0x0CF8
        out dx, eax
    }

    _asm {
        mov edx, 0x0CFC
        in eax, dx
        mov regData, eax
    }

    u32 deviceId = regData >> 16;
    u32 vendorId = regData - (deviceId << 16);

    if (vendorId == 0xffff) return;
    if (vendorId == 0 && deviceId == 0 && vendorId == 2) return;

    // Выводим информацию о названии устройства, VenID и DevID
    for (int i = 0; i < PCI_VENTABLE_LEN; i++) {
        PCI_VENTABLE val = PciVenTable[i];
        if (val.VenId == vendorId) {
            printf("Vendor: %s, VenID: %04X, DevID: %04X\n", val.VenFull, vendorId, deviceId);
            return; // Вывели информацию и завершаем функцию
        }
    }
}

void checkAllBuses() {
    for (u32 bus = 0; bus < 256; bus++)
        for (u32 dev = 0; dev < 32; dev++)
            for (u32 func = 0; func < 8; func++)
                getDeviceInfo(bus, dev, func, 0);
}

int main() {
    ALLOW_IO_OPERATIONS;

    printf("PCI Devices:\n");

    checkAllBuses();
    return 0;
}
