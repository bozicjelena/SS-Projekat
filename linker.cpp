#include "../inc/linker.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

void Linker::addPlace(const string& place){
  places.push_back(place);
}

void Linker::addInputFile(const string& input){
  inputFiles.push_back(input);
}

void Linker::readInputFile(const string& input, auto& symbolTable, auto& relocationTable, auto& code){
  ifstream in(input, ios::binary);
  if(!in.is_open()){
    cout << "Error: File " << input << " failed to open!";
    return;
  }
  unsigned numSymbols;
  in.read((char*)&numSymbols, sizeof(numSymbols));
  for(unsigned i = 0; i < numSymbols; i++){
    unsigned nameSize;
    in.read((char*)&nameSize, sizeof(nameSize));
    string name(nameSize, '\0');
    in.read(&name[0], nameSize);
    int section, value, number, size;
    bool isGlobal, isExtern;

    in.read((char*)&section, sizeof(section));
    in.read((char*)&value, sizeof(value));
    in.read((char*)&isGlobal, sizeof(isGlobal));
    in.read((char*)&isExtern, sizeof(isExtern));
    in.read((char*)&number, sizeof(number));
    in.read((char*)&size, sizeof(size));
    Symbol s;
    s.name = name;
    s.section = section;
    s.value = value;
    s.isGlobal = isGlobal;
    s.isExtern = isExtern;
    s.number = number;
    s.size = size;
    symbolTable[name] = s; 
  }


  unsigned numRelocs;
  in.read((char*)&numRelocs, sizeof(numRelocs));
  for(unsigned i = 0; i < numRelocs; i++){

    int type, section, offset, symbolTableReference;

    in.read((char*)&type, sizeof(type));
    in.read((char*)&section, sizeof(section));
    in.read((char*)&offset, sizeof(offset));
    in.read((char*)&symbolTableReference, sizeof(symbolTableReference));

    RelocationTableEntry rte;
    rte.type = type;
    rte.section = section;
    rte.offset = offset;
    rte.symbolTableReference = symbolTableReference;
    relocationTable.insert({symbolTableReference, rte});
  }

  unsigned numSections;
  in.read((char*)&numSections, sizeof(numSections));
  for(unsigned i = 0; i < numSections; i++){

    int sectionId;
    unsigned sectionSize;
    in.read((char*)&sectionId, sizeof(sectionId));
    in.read((char*)&sectionSize, sizeof(sectionSize));
    
    list<char> bytes;
    for(unsigned j = 0; j < sectionSize; j++){
      char ch;
      in.read(&ch, sizeof(ch));
      bytes.push_back(ch);
    }
    code[sectionId] = bytes;
  }
  in.close();
}

void Linker::countSectionSize(map<string, Symbol> table){
  map<int, string> sections; //pomocna struktura za odredjivanje redosleda sekcija
  list<string> order;
  for(auto sym: table){
    if(sym.second.size != -1){
      sections[sym.second.number] = sym.second.name;
    }
  }
  for(auto sec: sections){
    order.push_back(sec.second);
  }

  for(auto sym: table){
    if(sym.second.size != -1){ //ako je sym sekcija
      if(sectionTable.count(sym.second.name)){
        auto& entry = sectionTable.at(sym.second.name);
        entry.size += sym.second.size;
      }else{
        Section s;
        s.size = sym.second.size;
        s.addr = 0; //jos uvek ne znamo
        sectionTable[sym.second.name] = s;
      }
    }
  }
  unordered_set<string> s(sectionOrder.begin(), sectionOrder.end());
  for(auto o: order){
    if(s.count(o) == 0){
      sectionOrder.push_back(o);
    }
  }
}

void Linker::processPLaces(string section, unsigned int &address){
  regex place_regex = regex("-place=([a-zA-Z][_a-zA-Z0-9]*)@0x([0-9a-fA-F]+)");
  for(auto place: places){
    smatch sm;
    if(regex_match(place, sm, place_regex)){
     string sec = sm.str(1);
     if(sec == section){
      string adr = sm.str(2);
      address = stoul(adr, nullptr, 16);
      break;
     } 
    }
  }
}

int Linker::link(){
  for(string inputFile: inputFiles){
    map<string, Symbol> symbolTable;
    multimap<int, RelocationTableEntry> relocationTable;
    map<int, list<char>> code;
    readInputFile(inputFile, symbolTable, relocationTable, code);
    countSectionSize(symbolTable);
  }
  unsigned int address = 0; //procesor izvrsava instr pocev od 0x40000000
  unsigned int addr;
  for(string section: sectionOrder){
    addr = address;
    processPLaces(section, addr);
    if(addr >= address){
      sectionTable[section].addr = addr;
      address = addr + sectionTable[section].size; 
    }else{
      cout << "Error: Overlaps between sections from input files when -place cl option is taken into account!";
      return -1; 
    }
  }
  int res = createGlobalSymbolTable();
  if(res < 0){
    return -1;
  }
  res = resolveRelocationEntries();
  if(res < 0){
    return -1;
  }
  return 0;
}

int Linker::createGlobalSymbolTable(){
  map<string, int> offsets; //ofset po fajlovima
  for(string inputFile: inputFiles){
    map<string, Symbol> symbolTable;
    multimap<int, RelocationTableEntry> relocationTable;
    map<int, list<char>> code;
    readInputFile(inputFile, symbolTable, relocationTable, code);
    map<int, string> sections; //pomocna struktura
    for(auto symbol: symbolTable){
      if(symbol.second.size != -1){
        sections[symbol.second.number] = symbol.second.name;
      }
    }
    for(auto symbol: symbolTable){
      if(symbol.second.size != -1){
        offsets[sections[symbol.second.number]] = 0;
      }
    }
  }
  for(string inputFile: inputFiles){
    map<string, Symbol> symbolTable;
    multimap<int, RelocationTableEntry> relocationTable;
    map<int, list<char>> code;
    readInputFile(inputFile, symbolTable, relocationTable, code);
    map<int, string> sections; //pomocna struktura
    for(auto symbol: symbolTable){
      if(symbol.second.size != -1){
        sections[symbol.second.number] = symbol.second.name;
      }
    }
    for(auto symbol: symbolTable){
      if(symbol.second.isGlobal){
        if(globalSymbolTable.count(symbol.second.name)){
          cout << "Error: Multiple global symbol definition!";
          return -1;
        }
        globalSymbolTable[symbol.second.name] = sectionTable[sections[symbol.second.section]].addr + offsets[sections[symbol.second.section]] + symbol.second.value;
      }
    }
    for(auto symbol: symbolTable){
      if(symbol.second.size != -1){
        offsets[sections[symbol.second.section]] += symbol.second.size;
      }
    }
  }
  return 0;
}

int Linker::resolveRelocationEntries(){
  for(string inputFile: inputFiles){
    map<string, Symbol> symbolTable;
    multimap<int, RelocationTableEntry> relocationTable;
    map<int, list<char>> code;
    readInputFile(inputFile, symbolTable, relocationTable, code);
    map<int, string> sections; //pomocna struktura
    for(auto symbol: symbolTable){
      if(symbol.second.size != -1){
        sections[symbol.second.number] = symbol.second.name;
      }
    }
    map<int, string> symbols; //pomocna struktura
    for(auto symbol: symbolTable){
      if(symbol.second.size == -1){
        symbols[symbol.second.number] = symbol.second.name;
      }
    }
    for(auto &rte: relocationTable){
      Symbol s = symbolTable.at(symbols[rte.second.symbolTableReference]);
      if(s.isGlobal || s.isExtern){
        if(globalSymbolTable.count(s.name) > 0){
          auto iterator = code[rte.second.section].begin();
          advance(iterator, rte.second.offset);
          *iterator = globalSymbolTable[s.name];
          advance(iterator, 1);
          *iterator = globalSymbolTable[s.name] >> 8 | *iterator;
          advance(iterator, 1);
          *iterator = globalSymbolTable[s.name] >> 16 | *iterator;
          advance(iterator, 1);
          *iterator = globalSymbolTable[s.name] >> 24 | *iterator;
        }else{
          cout << "Error: Unresolved symbol: " + symbols[rte.second.symbolTableReference];
          return -1;
        }
      }else{
        int val = sectionTable[sections[symbolTable[symbols[rte.second.symbolTableReference]].section]].addr + symbolTable[symbols[rte.second.symbolTableReference]].value;
        auto iterator = code[rte.second.section].begin();
        advance(iterator, rte.second.offset);
        *iterator = val;
        advance(iterator, 1);
        *iterator = val >> 8 | *iterator;
        advance(iterator, 1);
        *iterator = val >> 16 | *iterator;
        advance(iterator, 1);
        *iterator = val >> 24 | *iterator;
      }
    }
    /////////////////////////////////////////////////////////////////////////////////
    for(auto c: code){
      for(auto byte: c.second){
        combinedCode[sectionTable[sections[c.first]].addr].push_back(byte);
      } 
    }
  }
  return 0;
}

void Linker::makeHexFile(const char *output){
  if(output != nullptr){
    ofstream outputFile(output);
    if(!outputFile.is_open()){
      cout << "Error: Output file failed to open!";
      return;
    }
    for(auto code: combinedCode){
      const int blockSize = 8;
      for(int i = 0; i < code.second.size(); i+= blockSize){
        bool allZero = true;
        for(int j = 0; j < blockSize && (i + j) < code.second.size(); j++){
          if(code.second[i + j] != 0){
            allZero = false;
            break;
          }
        }
        if(allZero) continue;
        outputFile << setw(8) << setfill('0') << hex << code.first + i << ": ";
        for(int j = 0; j < blockSize && (i + j) < code.second.size(); j++){
          outputFile << setw(2) << setfill('0') << hex << (static_cast<int>(code.second[i + j]) & 0xFF) << " ";
        }
        outputFile << endl;
      }
    }
    outputFile.close();
  }
}

int main(int argc, char* argv[]){
  Linker l;

  //opcije komandne linije
  regex place_regex = regex("-place=([a-zA-Z][_a-zA-Z0-9]*)@0x([0-9a-fA-F]+)");
  regex input_regex = regex("^([a-zA-Z][_a-zA-Z0-9]*)\\.o$");
  regex output_regex = regex("^([a-zA-Z][_a-zA-Z0-9]*)\\.hex$");

  string outputFileName;
  bool hex = false;
  string arg;
  if(false){ //if(argv[0] != "./linker"){
    return -1;
  }
  int i = 1;
  for(; i < argc; i++){
    arg = argv[i];
    smatch sm;
    if(arg == "-o"){
      i++;
      arg = argv[i];
      if(regex_match(arg, sm, output_regex)){
        outputFileName = arg;
      }else{
        return -1;
      }
    }else if(arg == "-hex"){
      hex = true;
    }else if(regex_match(arg, sm, place_regex)){
      l.addPlace(arg);
    }else if(regex_match(arg, sm, input_regex)){
      l.addInputFile(arg);
      break;
    }else{
      return -1;
    }
  }
  i++;
  for(; i < argc; i++){
    arg = argv[i];
    smatch sm;
    if(regex_match(arg, sm, input_regex)){
      l.addInputFile(arg);
    }else{
      return -1;
    }
  }
  if(!hex){
    return -1;
  }
  int res = l.link();
  l.makeHexFile(outputFileName.c_str());
  return res;
}