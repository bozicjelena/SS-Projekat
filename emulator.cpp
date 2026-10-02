#include "../inc/emulator.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

void Emulator::readInputFile(const char *input){
  ifstream inputFile(input);
  if(!inputFile.is_open()){
    cout << "Error: File " << input << " failed to open!";
    return;
  }
  string line;
  unsigned int address;
  int byte;
  char separator;
  while(getline(inputFile, line)){
    if(line.empty()) continue;
    stringstream ss(line);
    if(!(ss >> hex >> address >> separator) || separator != ':'){
      cout << "Error: " << line << "could not be parsed!";
      continue;
    }
    int i = 0;
    while(ss >> hex >> byte){
      memory[address + i] = byte;
      i++;
    }
  }
  inputFile.close();
}

int main(int argc, char* argv[]){
  if(argc != 2){
    cout << "Invalid number of arguments!" << endl;
    return -1;
  }
  Emulator e;
  e.readInputFile(argv[1]);
  e.execute();
  return 0;
}

void Emulator::execute(){
  for(int i = 0; i < 16; i++){
    reg[i] = 0;
  }
  for(int i = 0; i < 3; i++){
    csr[i] = 0;
  }
  halt = false;
  *pc = 0x40000000;
  while(memory.find(*pc) != memory.end() && !halt){
    int oc = (memory[*pc + 3] & 0xF0) >> 4;
    int mod = memory[*pc + 3] & 0x0F;
    int regA = (memory[*pc + 2] & 0xF0) >> 4;
    int regB = memory[*pc + 2] & 0x0F;
    int regC = (memory[*pc + 1] & 0xF0) >> 4;
    int disp = (memory[*pc + 1] & 0x0F) << 8 | memory[*pc] & 0xFF;
    if(disp & 0x800){
      disp = 0xfffff000 | disp;
    }
    *pc += 4;
    switch(oc){
      case 0:
        haltInstruction(mod, regA, regB, regC, disp);
        break;
      case 1:
        intInstruction(mod, regA, regB, regC, disp);
        break;
      case 2:
        callInstruction(mod, regA, regB, regC, disp);
        break;
      case 3:
        jumpInstruction(mod, regA, regB, regC, disp);
        break;
      case 4:
        exchangeInstruction(mod, regA, regB, regC, disp);
        break;
      case 5:
        arithmeticInstructions(mod, regA, regB, regC, disp);
        break;
      case 6:
        logicalInstructions(mod, regA, regB, regC, disp);
        break;
      case 7:
        shiftInstructions(mod, regA, regB, regC, disp);
        break;
      case 8:
        storeInstruction(mod, regA, regB, regC, disp);
        break;
      case 9:
        loadInstruction(mod, regA, regB, regC, disp);
        break;
      default:
        cout << "Error: Bad instruction format!";
        return;
    }
  }
  if(halt) cout << "Emulated processor executed halt instruction" << endl;
  cout << "Emulated processor state:" << endl;
  for(int i = 0; i < 16; i++){
    cout << "r" << i << "=" << hex << reg[i] << endl;
  }
}

void Emulator::arithmeticInstructions(int mod, int regA, int regB, int regC, int disp){
  if(disp != 0){
    cout << "Error: Bad instruction format!";
    return;
  }
  switch(mod){
    case 0b0000:
      reg[regA] = reg[regB] + reg[regC];
      break;
    case 0b0001:
      reg[regA] = reg[regB] - reg[regC];
      break;
    case 0b0010:
      reg[regA] = reg[regB] * reg[regC];
      break;
    case 0b0011:
      reg[regA] = reg[regB] / reg[regC];
      break;
    default:
      cout << "Error: Bad instruction format!";
      return;
  }
}

void Emulator::logicalInstructions(int mod, int regA, int regB, int regC, int disp){
  if(disp != 0){
    cout << "Error: Bad instruction format!";
    return;
  }
  switch(mod){
    case 0b0000:
      reg[regA] = ~reg[regB];
      break;
    case 0b0001:
      reg[regA] = reg[regB] & reg[regC];
      break;
    case 0b0010:
      reg[regA] = reg[regB] | reg[regC];
      break;
    case 0b0011:
      reg[regA] = reg[regB] ^ reg[regC];
      break;
    default:
      cout << "Error: Bad instruction format!";
      return;
  }
}

void Emulator::shiftInstructions(int mod, int regA, int regB, int regC, int disp){
  if(disp != 0){
    cout << "Error: Bad instruction format!";
    return;
  }
  switch(mod){
    case 0b0000:
      reg[regA] = reg[regB] << reg[regC];
      break;
    case 0b0001:
      reg[regA] = reg[regB] >> reg[regC];
      break;
    default:
      cout << "Error: Bad instruction format!";
      return;
  }
}

void Emulator::haltInstruction(int mod, int regA, int regB, int regC, int disp){
  if(mod + regA + regB + regC + disp > 0){
    cout << "Error: Bad instruction format!";
    return;
  }
  halt = true;
}

void Emulator::exchangeInstruction(int mod, int regA, int regB, int regC, int disp){
  if(mod != 0 || regA != 0 || disp != 0){
    cout << "Error: Bad instruction format!";
    return;
  }
  int temp = reg[regB];
  reg[regB] = reg[regC];
  reg[regC] = temp;
}

void Emulator::intInstruction(int mod, int regA, int regB, int regC, int disp){
  if(mod + regA + regB + regC + disp > 0){
    cout << "Error: Bad instruction format!";
    return;
  }
  *sp -= 4;
  write(*sp, *pc);
  *sp -= 4;
  write(*sp, *status);
  *cause = 4;
  *status &= (~0x1);
  *pc = *handler;
}

void Emulator::callInstruction(int mod, int regA, int regB, int regC, int disp){
  if(regC != 0){
    cout << "Error: Bad instruction format!";
    return;
  }
  switch(mod){
    case 0b0000:
      *sp -= 4;
      write(*sp, *pc);
      *pc = reg[regA] + reg[regB] + disp;
      break;
    case 0b0001:
      *sp -= 4;
      write(*sp, *pc);
      *pc = read(reg[regA] + reg[regB] + disp);
      break;
    default:
      cout << "Error: Bad instruction format!";
      return;
  }
}

void Emulator::jumpInstruction(int mod, int regA, int regB, int regC, int disp){
  switch(mod){
    case 0b0000:
      *pc = reg[regA] + disp;
      break;
    case 0b0001:
      if(reg[regB] == reg[regC]){
        *pc = reg[regA] + disp;
      }
      break;
    case 0b0010:
      if(reg[regB] != reg[regC]){
        *pc = reg[regA] + disp;
      }
      break;
    case 0b0011:
      if(reg[regB] > reg[regC]){
        *pc = reg[regA] + disp;
      }
      break;
    case 0b1000:
      *pc = read(reg[regA] + disp);
      break;
    case 0b1001:
      if(reg[regB] == reg[regC]){
        *pc = read(reg[regA] + disp);
      }
      break;
    case 0b1010:
      if(reg[regB] != reg[regC]){
        *pc = read(reg[regA] + disp);
      }
      break;
    case 0b1011:
      if(reg[regB] > reg[regC]){
        *pc = read(reg[regA] + disp);
      }
      break;
    default:
      cout << "Error: Bad instruction format!";
      return;
  }
}

void Emulator::storeInstruction(int mod, int regA, int regB, int regC, int disp){
  int addr;
  switch(mod){
    case 0b0000:
      write(reg[regA] + reg[regB] + disp, reg[regC]);
      break;
    case 0b0010:
      addr = read(reg[regA] + reg[regB] + disp);
      write(addr, reg[regC]);
      break;
    case 0b0001:
      reg[regA] = reg[regA] - disp;
      write(reg[regA], reg[regC]);
      break;
    default:
      cout << "Error: Bad instruction format!";
      return;
  }
}

void Emulator::loadInstruction(int mod, int regA, int regB, int regC, int disp){
  switch(mod){
    case 0b0000:
      reg[regA] = csr[regB];
      break;
    case 0b0001:
      reg[regA] = reg[regB] + disp;
      break;
    case 0b0010:
      reg[regA] = read(reg[regB] + reg[regC] + disp);
      break;
    case 0b0011:
      reg[regA] = read(reg[regB]);
      reg[regB] = reg[regB] + disp;
      break;
    case 0b0100:
      csr[regA] = csr[regB];
      break;
    case 0b0101:
      csr[regA] = csr[regB] | disp;
      break;
    case 0b0110:
      csr[regA] = read(reg[regB] + reg[regC] + disp);
      break;
    case 0b0111:
      csr[regA] = read(reg[regB]);
      reg[regB] = reg[regB] + disp;
      break;
    default:
      cout << "Error: Bad instruction format!";
      return;
  }
}

int Emulator::read(unsigned int address){
  return memory[address] | memory[address + 1] << 8 | memory[address + 2] << 16 | memory[address + 3] << 24;
}

void Emulator::write(unsigned int address, int data){
  for(int i = 0; i < 4; i++){
    memory[address + i] = (data >> (8 * i)) & 0xFF;
  }
}

// g++ -g -o asembler ./src/asembler.cpp
// ./asembler -o handler.o ./tests/handler.s
// ./asembler -o math.o ./tests/math.s
// ./asembler -o isr_timer.o ./tests/isr_timer.s
// ./asembler -o isr_terminal.o ./tests/isr_terminal.s
// ./asembler -o isr_software.o ./tests/isr_software.s
// ./asembler -o main.o ./tests/main.s
// g++ -g -o linker ./src/linker.cpp
// ./linker -hex -place=my_code@0x40000000 -place=math@0xf0000000 -o program.hex handler.o main.o math.o isr_terminal.o isr_timer.o isr_software.o
// g++ -g -o emulator ./src/emulator.cpp
//./emulator program.hex