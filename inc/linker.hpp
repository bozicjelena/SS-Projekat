#include <regex>
#include <list>
#include <map>
#include <unordered_set>
using namespace std;

  struct Symbol{
    string name;
    int section;
    int value;
    bool isGlobal;
    bool isExtern;
    int number;
    int size;
  };

  struct RelocationTableEntry{
    int type;
    int offset;
    int section;
    int symbolTableReference;
  };

  struct Section{
    int size;
    unsigned int addr;
  };

class Linker{

  list<string> places;
  list<string> inputFiles;

  map<string, Section> sectionTable;
  list<string> sectionOrder;
  
  map<string, int> globalSymbolTable;

  map<unsigned int, vector<char>> combinedCode;

public:
  void addPlace(const string& place);
  void addInputFile(const string& input);
  void countSectionSize(map<string, Symbol> table);
  void readInputFile(const string& input, auto& symbolTable, auto& relocationTable, auto& code);
  int link();
  void processPLaces(string section, unsigned int &address);
  int createGlobalSymbolTable();
  int resolveRelocationEntries();
  void makeHexFile(const char *output);

};