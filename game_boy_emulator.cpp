#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <SDL3/SDL.h>
using namespace std;

vector<uint8_t> ROM;
uint8_t VRAM[8192];
uint8_t Work_RAM[8192];
vector<uint8_t> External_RAM;
uint8_t OAM[160];
uint8_t HRAM[127];
uint8_t IER;
uint16_t mixed_registars;
uint16_t mixed_bytes;

struct input_and_output
{
    uint8_t joypad_input;
    uint8_t serial_transfer[2];
    uint8_t timer_and_divider[4];
    uint8_t interrupts;
    uint8_t audio[23];
    uint8_t wave_pattern[16];
    uint8_t LCD_control[6];
    uint8_t boot_ROM_mapping_controlls;
    uint8_t BG_and_OBJ_palettes[3];
};
struct CPU
{
    uint8_t A = 0;
    uint8_t B = 0;
    uint8_t C = 0;
    uint8_t D = 0;
    uint8_t E = 0;
    uint8_t H = 0;
    uint8_t L = 0;
    uint8_t F = 0;
    uint16_t PC = 0;
    uint16_t SP = 0;
}; // create struct with 8 bit and 16 bit 8 bit are registares  16 bit are registare that hold addresses

void BC_get(uint8_t B, uint8_t C, uint16_t &mix_registar)
{
    mixed_registars = B << 8;
    mix_registar += C;
}
void BC_set(uint8_t &B, uint8_t &C, uint16_t mix_registar)
{
    C = mix_registar;
    B = mix_registar >> 8;
}
void AF_get(uint8_t A, uint8_t F, uint16_t &mix_registar)
{
    mixed_registars = A << 8;
    mix_registar += F;
}
void AF_set(uint8_t &A, uint8_t &F, uint16_t mix_registar)
{
    A = mix_registar;
    F = mix_registar >> 8;
}
void DE_get(uint8_t D, uint8_t E, uint16_t &mix_registar)
{
    mixed_registars = D << 8;
    mix_registar += E;
}
void DE_set(uint8_t &D, uint8_t &E, uint16_t mix_registar)
{
    D = mix_registar;
    E = mix_registar >> 8;
}
void HL_get(uint8_t H, uint8_t L, uint16_t &mix_registar)
{
    mixed_registars = H << 8;
    mix_registar += L;
}
void HL_set(uint8_t &H, uint8_t &L, uint16_t mix_registar)
{
    H = mix_registar;
    L = mix_registar >> 8;
}
uint8_t read_byte_8(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x3FFF)
    {
        return ROM[address];
    } // rom fixed
    else if (address >= 0x4000 && address <= 0x7FFF)
    {
        return ROM[address];
    } // rom bank 1 switchable
    else if (address >= 0x8000 && address <= 0x9FFF)
    {
        return VRAM[address - 0x8000];
    } // vram
    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        return External_RAM[address - 0xa000];
    } // external ram
    else if (address >= 0xC000 && address <= 0xDFFF)
    {
        return Work_RAM[address - 0xc000];
    } // work ram
    else if (address >= 0xE000 && address <= 0xFDFF)
    {
        return Work_RAM[address - 0xE000];
    } // echo ram
    else if (address >= 0xFE00 && address <= 0xFE9F)
    {
        return OAM[address - 0xFE00];
    } // oam
    else if (address >= 0xFEA0 && address <= 0xFEFF)
    {
        return 0xff;
    } // unusable
    else if (address >= 0xFF00 && address <= 0xFF7F)
    {

    } // i/o
    else if (address >= 0xFF80 && address <= 0xFFFE)
    {
        return HRAM[address - 0xFF80];
    } // HRAM
    else if (address == 0xFFFF)
    {
        return IER;
    } // ier
} // reads a byte and does what it says
void read_byte_16(uint16_t &address)
{
    mixed_bytes += read_byte_8(address + 1);
    address += 1;
    mixed_bytes <<= 8;
    mixed_bytes += read_byte_8(address - 1);
}
void load_rom()
{
    ifstream read("cpu_instrs.gb", ios::binary); // open gameboy rom file.
    char byte;
    while (read.read(&byte, 1))
    {
        ROM.push_back(static_cast<uint8_t>(byte));
    }
} // load rom
void run_opcode(CPU &cpu, uint8_t opcode)
{
    switch (opcode)
    {
    case 0x00:
        break;
    case 0x01:

        break;
    case 0x02:

        break;
    case 0x03:
        BC_get(cpu.B, cpu.C, mixed_registars);
        mixed_registars += 01;
        BC_set(cpu.B, cpu.C, mixed_registars);
        break;
    case 0x04:
        if (cpu.B += 0b00000001 > 255)
        { // need to check becse it alway go to 0 and can not stay at 256 becase it max is that
            cpu.B += 0b00000001;
            if (cpu.F >= 0b00010000)
            {
                cpu.F += 0b00010000;
            }
            else
            {
                cpu.F -= 0b10000000;
            }
        }
        else
        {
            cpu.F -= 0b10000000;
        }
        break;
    case 0x05:

        break;
    case 0x06:

        break;
    case 0x07:

        break;
    case 0x08:

        break;
    case 0x09:

        break;
    case 0x0A:

        break;
    case 0x0B:

        break;
    case 0x0C:
        cpu.C += 0b00000001;
        break;
    case 0x0D:
        cpu.C -= 0b00000001;
        if (cpu.F >= 0b01000000)
        { // need to check if it contanit it not if in it becase if it 10000000 it bigger even thought taht not there
            cpu.F += 0b01000000;
        }
        break;
    case 0x0E:

        break;
    case 0x0F:

        break;
    case 0x11:

        break;
    case 0x12:

        break;
    case 0x13:

        break;
    case 0x14:
        cpu.D += 0b00000001;
        break;
    case 0x15:
        cpu.D -= 0b00000001;
        if (cpu.F >= 0b01000000)
        { // need to check if it contanit it not if in it becase if it 10000000 it bigger even thought taht not there
            cpu.F += 0b01000000;
        }
        break;
    }
} // what the code tell us what to do
uint8_t fetch_byte(CPU &cpu)
{
    uint8_t byte = read_byte_8(cpu.PC);
    cpu.PC += 1;
    return byte;
} // read byte plus pc+1
void write_byte_8(uint16_t address, uint8_t value)
{
    if (address >= 0x8000 && address <= 0x9FFF)
    {
        VRAM[address - 0x8000] = value;
    } // vram
    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        External_RAM[address - 0xA000] = value;
    } // external ram
    else if (address >= 0xC000 && address <= 0xDFFF)
    {
        Work_RAM[address - 0xC000] = value;
    } // work ram
    else if (address >= 0xE000 && address <= 0xFDFF)
    {
        Work_RAM[address - 0xE000] = value;
    } // echo ram
    else if (address >= 0xFE00 && address <= 0xFE9F)
    {
        OAM[address - 0xFE00] = value;
    } // oam
    else if (address >= 0xFF00 && address <= 0xFF7F)
    {

    } // i/o
    else if (address >= 0xFF80 && address <= 0xFFFE)
    {
        HRAM[address - 0xFF80] = value;
    } // HRAM
    else if (address == 0xFFFF)
    {
        IER = value;
    } // ier
}
int main()
{
    CPU cpu;         // make a object called cpu from CPU
    uint8_t opcode;  // 8bit variable called opcode
    load_rom();      // runs fuction
    cpu.PC = 0x0100; // update pc to 0x0100
    while (true)
    {
        opcode = fetch_byte(cpu); // turn fuction into opcode and +1 to pc
        run_opcode(cpu, opcode);  // check what opcode does e
        write_byte_8(cpu.PC, opcode);
    }
}