using namespace std;
#include <map>

class Emulator{
  private:
  int reg[16];
  int csr[3];
  int* sp = &reg[14];
  int* pc = &reg[15];
  int* status = &csr[0];
  int* handler = &csr[1];
  int* cause = &csr[2];
  
  bool halt;

  public:
  map<unsigned int, uint8_t> memory;
  void readInputFile(const char *input);
  void execute();
  void arithmeticInstructions(int mod, int regA, int regB, int regC, int disp);
  void logicalInstructions(int mod, int regA, int regB, int regC, int disp);
  void shiftInstructions(int mod, int regA, int regB, int regC, int disp);
  void haltInstruction(int mod, int regA, int regB, int regC, int disp);
  void exchangeInstruction(int mod, int regA, int regB, int regC, int disp);
  void intInstruction(int mod, int regA, int regB, int regC, int disp);
  void callInstruction(int mod, int regA, int regB, int regC, int disp);
  void jumpInstruction(int mod, int regA, int regB, int regC, int disp);
  void storeInstruction(int mod, int regA, int regB, int regC, int disp);
  void loadInstruction(int mod, int regA, int regB, int regC, int disp);
  int read(unsigned int address);
  void write(unsigned int address, int data);
};