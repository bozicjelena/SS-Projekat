#include "../inc/asembler.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

int Asembler::currNumber = 0;


void Asembler::processInputFile(const char *input) {
  if(input == nullptr){
    cout << "Error: Bad input!";
    return;
  }
  ifstream inputFile(input);
  if(!inputFile.is_open()){
    cout << "Error: Input file failed to open!";
    return;
  }
  string asmLine;
  while(getline(inputFile, asmLine)){
    asmLine = regex_replace(asmLine, comment, "$1");
    asmLine = regex_replace(asmLine, tab, " ");
    asmLine = regex_replace(asmLine, spaces, " ");
    asmLine = regex_replace(asmLine, comma_space, ", ");
    asmLine = regex_replace(asmLine, label_space, ": ");
    asmLine = regex_replace(asmLine, boundary_spaces, "$2");
    if(asmLine != " " && asmLine != "") asmLines.push_back(asmLine);
  }
}

void Asembler::processAsmLines(){
  for(string line: asmLines) {
    smatch s;
    if(regex_match(line, s, label)){
      if(processLabel(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,label_ins_or_dir)){
      if(processLabel(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
      auto pos = find(asmLines.begin(), asmLines.end(), line);
      auto nextPos = next(pos, 1);
      asmLines.insert(nextPos, s.str(2));
    }else if(regex_match(line,s,dir_global)){
      if(processGlobal(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,dir_extern)){
      if(processExtern(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,dir_section)){
      if(processSection(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,dir_word)){
      if(processWord(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s, dir_skip)){
      if(processSkip(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s, dir_end)){
      Symbol& s = symbolTable.at(currSectionName);
      s.size = lc;
      lc = 0;
      return;
    }else if(regex_match(line,s,ins_halt)){
      if(processHalt() < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_int)){
      if(processInt() < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_iret)){
      if(processIret() < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_call_l)){
      if(processCall_l(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_call_s)){
      if(processCall_s(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ret)){
      if(processRet() < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_jmp_l)){
      if(processJmp_l(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_jmp_s)){
      if(processJmp_s(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_beq_l)){
      if(processBeq_l(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_beq_s)){
      if(processBeq_s(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_bne_l)){
      if(processBne_l(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_bne_s)){
      if(processBne_s(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_bgt_l)){
      if(processBgt_l(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_bgt_s)){
      if(processBgt_s(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_push)){
      if(processPush(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_pop)){
      if(processPop(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_xchg)){
      if(processXchg(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_add)){
      if(processAdd(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_sub)){
      if(processSub(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_mul)){
      if(processMul(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_div)){
      if(processDiv(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_not)){
      if(processNot(s.str(1)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_and)){
      if(processAnd(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_or)){
      if(processOr(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_xor)){
      if(processXor(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_shl)){
      if(processShl(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_shr)){
      if(processShr(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_imm_l)){
      if(processLd_imm_l(s.str(1), s.str(2)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_imm_s)){
      if(processLd_imm_s(s.str(1), s.str(2)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_mem_dir_l)){
      if(processLd_mem_dir_l(s.str(1),s.str(2)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_mem_dir_s)){
      if(processLd_mem_dir_s(s.str(1), s.str(2)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_reg_dir)){
      if(processLd_reg_dir(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_reg_ind)){
      if(processLd_reg_ind(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_reg_ind_pom_l)){
      if(processLd_reg_ind_pom_l(s.str(1), s.str(3), s.str(4)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_ld_reg_ind_pom_s)){
      if(processLd_reg_ind_pom_s(s.str(1), s.str(3), s.str(4)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_st_mem_dir_l)){
      if(processSt_mem_dir_l(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_st_mem_dir_s)){
      if(processSt_mem_dir_s(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }
    /*else if(regex_match(line,s,ins_st_reg_dir)){
      if(processSt_reg_dir(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }*/
    else if(regex_match(line,s,ins_st_reg_ind)){
      if(processSt_reg_ind(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_st_reg_ind_pom_l)){
      if(processSt_reg_ind_pom_l(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_st_reg_ind_pom_s)){
      if(processSt_reg_ind_pom_s(s.str(1), s.str(3), s.str(5)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_csrrd)){
      if(processCsrrd(s.str(1), s.str(2)) < 0){
        cout << "Error!";
        return;
      }
    }else if(regex_match(line,s,ins_csrwr)){
      if(processCsrwr(s.str(1), s.str(3)) < 0){
        cout << "Error!";
        return;
      }
    }
  }
}

int Asembler::processLabel(string label){
  if(symbolTable.count(label)){
    Symbol& s = symbolTable.at(label);
    if(s.isExtern){
      cout << "Error: Symbol is extern!";
      return -1;
    }
    s.section = currSection;
    s.value = lc;
  } else{
    Symbol s;
    s.name = label;
    s.section = currSection;
    s.value = lc;
    s.isGlobal = false;
    s.isExtern = false;
    s.number = currNumber;
    currNumber++;
    s.size = -1;
    symbolTable[label] = s; //dodajemo simbol u tabelu
  }

  return 0;
}

int Asembler::processGlobal(string str){
  list<string> symbols;
  stringstream ss(str);
  string symbol;
  while(getline(ss, symbol, ',')){
    size_t start = symbol.find_first_not_of(" ");
    size_t end = symbol.find_last_not_of(" ");
    if(start != std::string::npos && end != std::string::npos){
      symbol = symbol.substr(start, end - start + 1);
      symbols.push_back(symbol);
    }
  }
  for(string s: symbols){
    if(symbolTable.count(s)){
      Symbol& symb = symbolTable.at(s);
      if(symb.isExtern){
        cout << "Error: Symbol is extern!";
        return -1;
      } else{
        symb.isGlobal = true;
      }
    } else{
      Symbol symb;
      symb.name = s;
      symb.section = 0; //jos uvek ne znamo
      symb.value = 0;
      symb.isGlobal = true;
      symb.isExtern = false;
      symb.number = currNumber;
      currNumber++;
      symb.size = -1;
      symbolTable[s] = symb; //dodajemo simbol u tabelu
    }
  }
  return 0;
}

int Asembler::processExtern(string str){
  list<string> symbols;
  stringstream ss(str);
  string symbol;
  while(getline(ss, symbol, ',')){
    size_t start = symbol.find_first_not_of(" ");
    size_t end = symbol.find_last_not_of(" ");
    if(start != std::string::npos && end != std::string::npos){
      symbol = symbol.substr(start, end - start + 1);
      symbols.push_back(symbol);
    }
  }
  for(string s: symbols){
    if(symbolTable.count(s)){
      Symbol& symb = symbolTable.at(s);
      if(symb.isGlobal){
        cout << "Error: Symbol is global!";
        return -1;
      } else if(symb.section != 0){
        cout << "Error: Symbol cannot be defined and extern in the same file!";
        return -1;
      }
    } else{
      Symbol symb;
      symb.name = s;
      symb.section = 0; //definisan u drugom fajlu
      symb.value = 0;
      symb.isGlobal = false;
      symb.isExtern = true;
      symb.number = currNumber;
      currNumber++;
      symb.size = -1;
      symbolTable[s] = symb; //dodajemo simbol u tabelu
    }
  }
  return 0;
}

int Asembler::processSection(string sectionName){
  if(symbolTable.count(sectionName)){
    cout << "Error: Section already defined!";
    return -1;
  }
  Symbol& s = symbolTable.at(currSectionName);
  s.size = lc;

  lc = 0;
  currSection = currNumber;
  currNumber ++;
  currSectionName = sectionName;

  Symbol newSection;
  newSection.name = currSectionName;
  newSection.section = currSection;
  newSection.value = 0;
  newSection.isGlobal = newSection.isExtern = false;
  newSection.number = currSection;
  newSection.size = 0;
  symbolTable[currSectionName] = newSection;
  return 0;
}

int Asembler::processWord(string str){
  if(currSectionName == "UND"){
    cout << "Error: .word in undefined section!";
    return -1;
  }

  list<string> symbols;
  stringstream ss(str);
  string symbol;
  while(getline(ss, symbol, ',')){
    size_t start = symbol.find_first_not_of(" ");
    size_t end = symbol.find_last_not_of(" ");
    if(start != std::string::npos && end != std::string::npos){
      symbol = symbol.substr(start, end - start + 1);
      symbols.push_back(symbol);
    }
  }
  for(string s: symbols){
    smatch sm;
    if(regex_match(s, sm, simbol)){
      Info i;
      i.section = currSection;
      i.addr = lc;
      i.type = 0;
      i.word = true;
      if(symbolTable.count(s)){
        Symbol& symbol = symbolTable.at(s);
        symbol.infoList.push_back(i);
      } else{
        Symbol symbol;
        symbol.name = s;
        symbol.section = 0;
        symbol.value = 0;
        symbol.isGlobal = symbol.isExtern = false;
        symbol.number = currNumber;
        currNumber++;
        symbol.size = -1;
        symbol.infoList.push_back(i);
        symbolTable[s] = symbol;
      }
      code[currSection].push_back(0);
      code[currSection].push_back(0);
      code[currSection].push_back(0);
      code[currSection].push_back(0);
    }else if(regex_match(s, sm, literal_dec)){
        int l = stoi(s);
        char first = (char)(l & 0xff);
        char second = (char)((l >> 8) & 0xff);
        char third = (char)((l >> 16) & 0xff);
        char fourth = (char)((l >> 24) & 0xff);
        code[currSection].push_back(first);
        code[currSection].push_back(second);
        code[currSection].push_back(third);
        code[currSection].push_back(fourth);
    }else if(regex_match(s, sm, literal_hex)){
        int l = stoul(s,nullptr, 16);
        char first = (char)(l & 0xff);
        char second = (char)((l >> 8) & 0xff);
        char third = (char)((l >> 16) & 0xff);
        char fourth = (char)((l >> 24) & 0xff);
        code[currSection].push_back(first);
        code[currSection].push_back(second);
        code[currSection].push_back(third);
        code[currSection].push_back(fourth);
    }
    lc += 4;
  }
  return 0;
}

int Asembler::processSkip(string str){
  smatch s;
  int l;
  if(regex_match(str, s, literal_dec)){
    l = stoi(str);
  }else if(regex_match(str, s, literal_hex)){
    l = stoul(str, nullptr, 16);
  }else{
    cout << "Error: Assembler directive .skip must be followed by a literal!";
    return -1;
  }
  for(int i = 0; i < l; i++){
    code[currSection].push_back(0);
  }
  lc += l;
  return 0;
}

int Asembler::processHalt(){
  if(currSectionName == "UND"){
    cout << "Error: Instruction halt in undefined section!";
    return -1;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  lc += 4;
  return 0;
}

int Asembler::processInt(){
  if(currSectionName == "UND"){
    cout << "Error: Instruction int in undefined section!";
    return -1;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back(16);
  lc += 4;
  return 0;
}

int Asembler::processCall_l(string str){
  if(currSectionName == "UND"){
    cout << "Error: Instruction call in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(str, s, literal_dec)){
    l = stoi(str);
  }else if(regex_match(str, s, literal_hex)){
    l = stoul(str, nullptr, 16);
  }
  if(l >= -2048 && l <= 2047){
    //literal moze da se zapise u 12b
    char d1 = (char)(l & 0xff);
    char d2 = (char)((l >> 8) & 0xff);
    code[currSection].push_back(d1);
    code[currSection].push_back(d2);
    code[currSection].push_back(0);
    code[currSection].push_back(32);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;

    literalPool[{section, l}].push_back(offset);

    code[currSection].push_back(0);
    code[currSection].push_back(0);
    code[currSection].push_back(15 << 4);
    code[currSection].push_back(33); // pc <= mem32[pc + d]
  }
  lc += 4;
  return 0;
}

int Asembler::processCall_s(string str){
  if(currSectionName == "UND"){
    cout << "Error: Instruction call in undefined section!";
    return -1;
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(str)){
    Symbol& symb = symbolTable.at(str);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = str;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[str] = symb;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(15 << 4));
  code[currSection].push_back(33);
  lc += 4;
  return 0;
}

int Asembler::processJmp_l(string str){
  if(currSectionName == "UND"){
    cout << "Error: Instruction jmp in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(str, s, literal_dec)){
    l = stoi(str);
  }else if(regex_match(str, s, literal_hex)){
    l = stoul(str, nullptr, 16);
  }
  if(l >= -2048 && l <= 2047){
    char d1 = (char)(l & 0xff);
    char d2 = (char)((l >> 8) & 0xff);
    code[currSection].push_back(d1);
    code[currSection].push_back(d2);
    code[currSection].push_back(0);
    code[currSection].push_back(48);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;
    literalPool[{section, l}].push_back(offset);
    code[currSection].push_back(0);
    code[currSection].push_back(0);
    code[currSection].push_back(15 << 4);
    code[currSection].push_back(56);
  }
  lc += 4;
  return 0;
}

int Asembler::processJmp_s(string str){
  if(currSectionName == "UND"){
    cout << "Error: Instruction jmp in undefined section!";
    return -1;
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(str)){
    Symbol& symb = symbolTable.at(str);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = str;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[str] = symb;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(15 << 4));
  code[currSection].push_back(56);
  lc += 4;
  return 0;
}

int Asembler::processBeq_l(string gpr1, string gpr2, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction beq in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(op, s, literal_dec)){
    l = stoi(op);
  }else if(regex_match(op, s, literal_hex)){
    l = stoul(op, nullptr, 16);
  }
  int g1,g2;
  if(gpr1 == "sp"){
    g1 = 14;
  }else if(gpr1 == "pc"){
    g1 = 15;
  }else if(gpr1.length() == 3){
    g1 = stoi(gpr1.substr(1,2));
  }else{
    g1 = stoi(gpr1.substr(1,1));
  }
  if(gpr2 == "sp"){
    g2 = 14;
  }else if(gpr2 == "pc"){
    g2 = 15;
  }else if(gpr2.length() == 3){
    g2 = stoi(gpr2.substr(1,2));
  }else{
    g2 = stoi(gpr2.substr(1,1));
  }

  if(l >= -2048 && l <= 2047){
    int x = (g2 << 12) | l;
    code[currSection].push_back((char)x);
    code[currSection].push_back((char)(x >> 8));
    code[currSection].push_back((char)g1);
    code[currSection].push_back(49);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;
    literalPool[{section, l}].push_back(offset);
    code[currSection].push_back(0);
    code[currSection].push_back((char)(g2 << 4));
    code[currSection].push_back((char)(g1 | (15 << 4)));
    code[currSection].push_back(57);
  }
  lc += 4;
  return 0;
}

int Asembler::processBeq_s(string gpr1, string gpr2, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction beq in undefined section!";
    return -1;
  }
  int g1, g2;
  if(gpr1 == "sp"){
    g1 = 14;
  }else if(gpr1 == "pc"){
    g1 = 15;
  }else if(gpr1.length() == 3){
    g1 = stoi(gpr1.substr(1,2));
  }else{
    g1 = stoi(gpr1.substr(1,1));
  }
  if(gpr2 == "sp"){
    g2 = 14;
  }else if(gpr2 == "pc"){
    g2 = 15;
  }else if(gpr2.length() == 3){
    g2 = stoi(gpr2.substr(1,2));
  }else{
    g2 = stoi(gpr2.substr(1,1));
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(op)){
    Symbol& symb = symbolTable.at(op);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = op;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[op] = symb;
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(g2 << 4));
  code[currSection].push_back((char)(g1 | 15 << 4));
  code[currSection].push_back(57);
  lc += 4;
  return 0;
}

int Asembler::processBne_l(string gpr1, string gpr2, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction bne in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(op, s, literal_dec)){
    l = stoi(op);
  }else if(regex_match(op, s, literal_hex)){
    l = stoul(op, nullptr, 16);
  }
  int g1,g2;
  if(gpr1 == "sp"){
    g1 = 14;
  }else if(gpr1 == "pc"){
    g1 = 15;
  }else if(gpr1.length() == 3){
    g1 = stoi(gpr1.substr(1,2));
  }else{
    g1 = stoi(gpr1.substr(1,1));
  }
  if(gpr2 == "sp"){
    g2 = 14;
  }else if(gpr2 == "pc"){
    g2 = 15;
  }else if(gpr2.length() == 3){
    g2 = stoi(gpr2.substr(1,2));
  }else{
    g2 = stoi(gpr2.substr(1,1));
  }
  if(l >= -2048 && l <= 2047){
    int x = (g2 << 12) | l;
    code[currSection].push_back((char)x);
    code[currSection].push_back((char)(x >> 8));
    code[currSection].push_back((char)g1);
    code[currSection].push_back(50);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;
    literalPool[{section, l}].push_back(offset);
    code[currSection].push_back(0);
    code[currSection].push_back((char)(g2 << 4));
    code[currSection].push_back((char)(g1 | (15 << 4)));
    code[currSection].push_back(58);
  }
  lc += 4;
  return 0;
}

int Asembler::processBne_s(string gpr1, string gpr2, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction bne in undefined section!";
    return -1;
  }
  int g1, g2;
  if(gpr1 == "sp"){
    g1 = 14;
  }else if(gpr1 == "pc"){
    g1 = 15;
  }else if(gpr1.length() == 3){
    g1 = stoi(gpr1.substr(1,2));
  }else{
    g1 = stoi(gpr1.substr(1,1));
  }
  if(gpr2 == "sp"){
    g2 = 14;
  }else if(gpr2 == "pc"){
    g2 = 15;
  }else if(gpr2.length() == 3){
    g2 = stoi(gpr2.substr(1,2));
  }else{
    g2 = stoi(gpr2.substr(1,1));
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(op)){
    Symbol& symb = symbolTable.at(op);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = op;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[op] = symb;
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(g2 << 4));
  code[currSection].push_back((char)(g1 | 15 << 4));
  code[currSection].push_back(58);
  lc += 4;
  return 0;
}

int Asembler::processBgt_l(string gpr1, string gpr2, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction bgt in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(op, s, literal_dec)){
    l = stoi(op);
  }else if(regex_match(op, s, literal_hex)){
    l = stoul(op, nullptr, 16);
  }
  
  int g1,g2;
  if(gpr1 == "sp"){
    g1 = 14;
  }else if(gpr1 == "pc"){
    g1 = 15;
  }else if(gpr1.length() == 3){
    g1 = stoi(gpr1.substr(1,2));
  }else{
    g1 = stoi(gpr1.substr(1,1));
  }
  if(gpr2 == "sp"){
    g2 = 14;
  }else if(gpr2 == "pc"){
    g2 = 15;
  }else if(gpr2.length() == 3){
    g2 = stoi(gpr2.substr(1,2));
  }else{
    g2 = stoi(gpr2.substr(1,1));
  }
  if(l >= -2048 && l <= 2047){
    int x = (g2 << 12) | l;
    code[currSection].push_back((char)x);
    code[currSection].push_back((char)(x >> 8));
    code[currSection].push_back((char)g1);
    code[currSection].push_back(51);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;
    literalPool[{section, l}].push_back(offset);
    code[currSection].push_back(0);
    code[currSection].push_back((char)(g2 << 4));
    code[currSection].push_back((char)(g1 | (15 << 4)));
    code[currSection].push_back(59);
  }
  lc += 4;
  return 0;
}

int Asembler::processBgt_s(string gpr1, string gpr2, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction bgt in undefined section!";
    return -1;
  }
  int g1, g2;
  if(gpr1 == "sp"){
    g1 = 14;
  }else if(gpr1 == "pc"){
    g1 = 15;
  }else if(gpr1.length() == 3){
    g1 = stoi(gpr1.substr(1,2));
  }else{
    g1 = stoi(gpr1.substr(1,1));
  }
  if(gpr2 == "sp"){
    g2 = 14;
  }else if(gpr2 == "pc"){
    g2 = 15;
  }else if(gpr2.length() == 3){
    g2 = stoi(gpr2.substr(1,2));
  }else{
    g2 = stoi(gpr2.substr(1,1));
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(op)){
    Symbol& symb = symbolTable.at(op);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = op;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[op] = symb;
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(g2 << 4));
  code[currSection].push_back((char)(g1) | 15 << 4);
  code[currSection].push_back(59);
  lc += 4;
  return 0;
}

int Asembler::processXchg(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction xchg in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back(d);
  code[currSection].push_back(64);
  lc += 4;
  return 0;
}

int Asembler::processAdd(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction add in undefined section!";
    return -1;
  }
  int s = 0;
  int d = 0;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(80);
  lc += 4;
  return 0;
}

int Asembler::processSub(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction sub in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(81);
  lc += 4;
  return 0;
}

int Asembler::processMul(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction mul in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(82);
  lc += 4;
  return 0;
}

int Asembler::processDiv(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction div in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(83);
  lc += 4;
  return 0;
}

int Asembler::processNot(string str){
  if(currSectionName == "UND"){
    cout << "Error: Instruction not in undefined section!";
    return -1;
  }
  int s;
  if(str == "sp"){
    s = 14;
  }else if(str == "pc"){
    s = 15;
  }else if(str.length() == 3){
    s = stoi(str.substr(1,2));
  }else{
    s = stoi(str.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4 | s));
  code[currSection].push_back(96);
  lc += 4;
  return 0;
}

int Asembler::processAnd(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction and in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(97);
  lc += 4;
  return 0;
}

int Asembler::processOr(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction or in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(98);
  lc += 4;
  return 0;
}

int Asembler::processXor(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction xor in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(99);
  lc += 4;
  return 0;
}

int Asembler::processShl(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction shl in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d = stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(112);
  lc += 4;
  return 0;
}

int Asembler::processShr(string gprs, string gprd){
  if(currSectionName == "UND"){
    cout << "Error: Instruction shr in undefined section!";
    return -1;
  }
  int s,d;
  if(gprs == "sp"){
    s = 14;
  }else if(gprs == "pc"){
    s = 15;
  }else if(gprs.length() == 3){
    s = stoi(gprs.substr(1,2));
  }else{
    s = stoi(gprs.substr(1,1));
  }
  if(gprd == "sp"){
    d = 14;
  }else if(gprd == "pc"){
    d = 15;
  }else if(gprd.length() == 3){
    d= stoi(gprd.substr(1,2));
  }else{
    d = stoi(gprd.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(s << 4));
  code[currSection].push_back((char)(d << 4 | d));
  code[currSection].push_back(113);
  lc += 4;
  return 0;
}

//ld

int Asembler::processLd_imm_l(string op, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ld in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(op, s, literal_dec)){
    l = stoi(op);
  }else if(regex_match(op, s, literal_hex)){
    l = stoul(op, nullptr, 16);
  }
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(l >= -2048 && l <= 2047){
    code[currSection].push_back(l);
    code[currSection].push_back((char)(l >> 8));
    code[currSection].push_back((char)(index << 4));
    code[currSection].push_back(145);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;
    literalPool[{section, l}].push_back(offset);
    code[currSection].push_back(0);
    code[currSection].push_back(0);
    code[currSection].push_back((char)(index << 4 | 15));
    code[currSection].push_back(146);
  }
  lc += 4;
  return 0;
}

int Asembler::processLd_imm_s(string op, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ld in undefined section!";
    return -1;
  }
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(op)){
    Symbol& symb = symbolTable.at(op);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = op;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[op] = symb;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4 | 15));
  code[currSection].push_back(146);
  lc += 4;
  return 0;
}

int Asembler::processLd_mem_dir_l(string op, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ld in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(op, s, literal_dec)){
    l = stoi(op);
  }else if(regex_match(op, s, literal_hex)){
    l = stoul(op, nullptr, 16);
  }
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(l >= -2048 && l <= 2047){
    code[currSection].push_back(l);
    code[currSection].push_back((char)(l >> 8));
    code[currSection].push_back((char)(index << 4));
    code[currSection].push_back(146);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;
    literalPool[{section, l}].push_back(offset);
    ///moraju se napisati dve istrukcije kako bi se postiglo mem ind adresiranje
    code[currSection].push_back(0);
    code[currSection].push_back(0);
    code[currSection].push_back((char)(index << 4 | 15));
    code[currSection].push_back(146);
    code[currSection].push_back(0);
    code[currSection].push_back(0);
    code[currSection].push_back((char)(index << 4 | index));
    code[currSection].push_back(146);
    lc += 4;
  }
  lc += 4;
  return 0;
}

int Asembler::processLd_mem_dir_s(string op, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ld in undefined section!";
    return -1;
  }
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(op)){
    Symbol& symb = symbolTable.at(op);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = op;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[op] = symb;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4) | 15);
  code[currSection].push_back(146);
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4) | index);
  code[currSection].push_back(146);
  lc += 8;
  return 0;
}

int Asembler::processLd_reg_dir(string op, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ld in undefined section!";
    return -1;
  }
  int index, src;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(op == "sp"){
    src = 14;
  }else if(op == "pc"){
    src = 15;
  }else if(op.length() == 3){
    src = stoi(op.substr(1,2));
  }else{
    src = stoi(op.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4 | src));
  code[currSection].push_back(145);
  lc += 4;
  return 0;
}

int Asembler::processLd_reg_ind(string op, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ld in undefined section!";
    return -1;
  }
  int index, src;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(op == "sp"){
    src = 14;
  }else if(op == "pc"){
    src = 15;
  }else if(op.length() == 3){
    src = stoi(op.substr(1,2));
  }else{
    src = stoi(op.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4 | src));
  code[currSection].push_back(146);
  lc += 4;
  return 0;
}

int Asembler::processLd_reg_ind_pom_l(string op, string pom, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ld in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(pom, s, literal_dec)){
    l = stoi(pom);
  }else if(regex_match(pom, s, literal_hex)){
    l = stoul(pom, nullptr, 16);
  }
  if(l < -2048 || l > 2047){
    return -1;
  }
  int index, src;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(op == "sp"){
    src = 14;
  }else if(op == "pc"){
    src = 15;
  }else if(op.length() == 3){
    src = stoi(op.substr(1,2));
  }else{
    src = stoi(op.substr(1,1));
  }
  code[currSection].push_back(l);
  code[currSection].push_back((char)(l >> 8));
  code[currSection].push_back((char)(index << 4 | src));
  code[currSection].push_back(146);
  lc += 4;
  return 0;
}

int Asembler::processLd_reg_ind_pom_s(string op, string pom, string gpr){
  return -1;
}

//st

int Asembler::processSt_mem_dir_l(string gpr, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction st in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(op, s, literal_dec)){
    l = stoi(op);
  }else if(regex_match(op, s, literal_hex)){
    l = stoul(op, nullptr, 16);
  }
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(l >= -2048 && l <= 2047){
    code[currSection].push_back(l);
    code[currSection].push_back((char)((l >> 8) | (index << 4)));
    code[currSection].push_back(0);
    code[currSection].push_back(128);
  }else{
    //literal stavljamo u literal pool
    int offset = lc;
    string section = currSectionName;
    literalPool[{section, l}].push_back(offset);
    code[currSection].push_back(0);
    code[currSection].push_back((char)(index << 4));
    code[currSection].push_back((char)(15 << 4));
    code[currSection].push_back(130);   
  }
  lc += 4;
  return 0;
}

int Asembler::processSt_mem_dir_s(string gpr, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction st in undefined section!";
    return -1;
  }
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  Info i;
  i.section = currSection;
  i.addr = lc;
  i.type = 0;
  i.word = false;
  if(symbolTable.count(op)){
    Symbol& symb = symbolTable.at(op);
    symb.infoList.push_back(i);
  }else{
    Symbol symb;
    symb.name = op;
    symb.section = 0;
    symb.value = 0;
    symb.isGlobal = symb.isExtern = false;
    symb.number = currNumber;
    currNumber++;
    symb.size = -1;
    symb.infoList.push_back(i);
    symbolTable[op] = symb;
  }
  
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4));
  code[currSection].push_back((char)(15 << 4));
  code[currSection].push_back(130);
  lc += 4;
  return 0;
}

/*
int Asembler::processSt_reg_dir(string gpr, string op){
  kako popuniti opcode???????????????????????????????
}
*/

int Asembler::processSt_reg_ind(string gpr, string op){
  if(currSectionName == "UND"){
    cout << "Error: Instruction st in undefined section!";
    return -1;
  }
  int index, dst;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(op == "sp"){
    dst = 14;
  }else if(op == "pc"){
    dst = 15;
  }else if(op.length() == 3){
    dst = stoi(op.substr(1,2));
  }else{
    dst = stoi(op.substr(1,1));
  }
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4));
  code[currSection].push_back((char)(dst << 4));
  code[currSection].push_back(128); /// maybe
  lc += 4;
  return 0;
}

int Asembler::processSt_reg_ind_pom_l(string gpr, string op, string pom){
  if(currSectionName == "UND"){
    cout << "Error: Instruction st in undefined section!";
    return -1;
  }
  smatch s;
  int l;
  if(regex_match(pom, s, literal_dec)){
    l = stoi(pom);
  }else if(regex_match(pom, s, literal_hex)){
    l = stoul(pom, nullptr, 16);
  }
  if(l < -2048 || l > 2047){
    return -1;
  }
  int index, dst;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(op == "sp"){
    dst = 14;
  }else if(op == "pc"){
    dst = 15;
  }else if(op.length() == 3){
    dst = stoi(op.substr(1,2));
  }else{
    dst = stoi(op.substr(1,1));
  }
  code[currSection].push_back(l);
  code[currSection].push_back((char)((index << 4) | (l >> 8)));
  code[currSection].push_back((char)(dst << 4));
  code[currSection].push_back(128); /// maybe
  lc += 4;
  return 0;
}

int Asembler::processSt_reg_ind_pom_s(string gpr, string op, string pom){
  return -1;
}

//csrrd & csrwr

int Asembler::processCsrrd(string csr, string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction csrrd in undefined section!";
    return -1;
  }
  int index, src;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(csr == "status"){
    src = 0;
  }else if(csr == "handler"){
    src = 1;
  }else if(csr == "cause"){
    src = 2;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(index << 4 | src));
  code[currSection].push_back(144);
  lc += 4;
  return 0;
}

int Asembler::processCsrwr(string gpr, string csr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction csrwr in undefined section!";
    return -1;
  }
  int index, dst;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  if(csr == "status"){
    dst = 0;
  }else if(csr == "handler"){
    dst = 1;
  }else if(csr == "cause"){
    dst = 2;
  }
  code[currSection].push_back(0);
  code[currSection].push_back(0);
  code[currSection].push_back((char)(dst << 4 | index));
  code[currSection].push_back(148);
  lc += 4;
  return 0;
}

//push
int Asembler::processPush(string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction push in undefined section!";
    return -1;
  }
  smatch s;
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  code[currSection].push_back(4);
  code[currSection].push_back((char)(index << 4));
  code[currSection].push_back((char)(14 << 4));
  code[currSection].push_back(129);
  lc += 4;
  return 0;
}

//pop
int Asembler::processPop(string gpr){
  if(currSectionName == "UND"){
    cout << "Error: Instruction pop in undefined section!";
    return -1;
  }
  smatch s;
  int index;
  if(gpr == "sp"){
    index = 14;
  }else if(gpr == "pc"){
    index = 15;
  }else if(gpr.length() == 3){
    index = stoi(gpr.substr(1,2));
  }else{
    index = stoi(gpr.substr(1,1));
  }
  code[currSection].push_back(4);
  code[currSection].push_back(0);
  code[currSection].push_back((char)((index << 4) | 14));
  code[currSection].push_back(147);
  lc += 4;
  return 0;
}

int Asembler::processIret(){
  if(currSectionName == "UND"){
    cout << "Error: Instruction iret in undefined section!";
    return -1;
  }
  code[currSection].push_back(4);
  code[currSection].push_back(0);
  code[currSection].push_back((char)((0 << 4) | 14)); //status - 0
  code[currSection].push_back(151);
  lc += 4;
  code[currSection].push_back(4);
  code[currSection].push_back(0);
  code[currSection].push_back((char)((15 << 4) | 14));
  code[currSection].push_back(147);
  lc += 4;
  return 0;
}

int Asembler::processRet(){
  if(currSectionName == "UND"){
    cout << "Error: Instruction ret in undefined section!";
    return -1;
  }
  code[currSection].push_back(4);
  code[currSection].push_back(0);
  code[currSection].push_back((char)((15 << 4) | 14));
  code[currSection].push_back(147);
  lc += 4;
  return 0;
}

int Asembler::createRelocatonTable(){
  for(const auto& x: symbolTable){
    if(x.second.infoList.size()){
      for(const Info& i: x.second.infoList){
        if(i.word){
          RelocationTableEntry rte;
          rte.offset = i.addr;
          rte.section = i.section;
          rte.type = i.type;
          rte.symbolTableReference = x.second.number;
          relocationTable.insert({x.second.number, rte});
        }else{
          for(auto& symbol: symbolTable){
            if(symbol.second.number == i.section){
              Symbol& sec = symbol.second;
              code[sec.section].push_back(0);
              code[sec.section].push_back(0);
              code[sec.section].push_back(0);
              code[sec.section].push_back(0);
              int displacement = sec.size - i.addr - 4;
              auto iterator = code[sec.section].begin();
              advance(iterator, i.addr);
              *iterator = displacement;
              advance(iterator, 1);
              *iterator = displacement >> 8 | *iterator;

              RelocationTableEntry rte;
              rte.offset = sec.size;
              rte.section = i.section;
              rte.type = i.type;
              rte.symbolTableReference = x.second.number;
              relocationTable.insert({x.second.number, rte});
              sec.size += 4;
            }
          }
        }
      }
    }
  }
  return 0;
}

void Asembler::makeOutputFile(const char *output){
  if(output != nullptr){
    ofstream outputFile("asembler_" + string(output));
    if(!outputFile.is_open()){
      cout << "Error: Output file failed to open!";
      return;
    }
    outputFile << "# Symbol Table" << endl;
    outputFile << "Name                 | Section | Value  | Global | Extern | Number | Size" << endl;
    for(const auto& x: symbolTable){
      outputFile << left << setw(20) << x.second.name << " | "
      << setw(7) << x.second.section << " | "
      << setw(6)  << x.second.value << setfill(' ') << " | "
      << setw(6) << (x.second.isGlobal ? "YES" : "NO") << " | "
      << setw(6) << (x.second.isExtern ? "YES" : "NO") << " | "
      << setw(6) << x.second.number << " | "
      << setw(4) << x.second.size;
      outputFile << endl;
    }
    outputFile << endl;
    outputFile << "# Relocation Table" << endl;
    outputFile << "Type | Section | Offset | Symbol Table Refference" << endl;
    for(const auto& x: relocationTable){
      outputFile << left << setw(4) << x.second.type << " | "
      << setw(7) << x.second.section << " | "
      << setw(6) << x.second.offset << " | "
      << x.second.symbolTableReference
      << endl;
    }
    outputFile << endl;
    outputFile << "# Code" << endl;
    for(const auto& x: code){
      outputFile << "Section " << x.first << ":" << endl;
      int i = 0;
      for(const auto& c: x.second){
        i++;
        outputFile << hex << setw(2) << (int)((unsigned char)c);
        if(i % 4 == 0){
          outputFile << endl;
        }else{
          outputFile << " | ";
        }
      }
      outputFile << endl;
    }
    outputFile.close();
    //binarni fajl
    ofstream binaryFile(output, ios::binary);
    if(!binaryFile.is_open()){
      cout << "Error: Binary file failed to open!";
      return;
    }
    unsigned numSymbols = symbolTable.size();
    binaryFile.write((char*)&numSymbols, sizeof(numSymbols));
    for(auto &sym: symbolTable){
      string name = sym.second.name;
      unsigned nameSize = name.size();
      binaryFile.write((char*)&nameSize, sizeof(nameSize));
      binaryFile.write(name.c_str(), nameSize);

      binaryFile.write((char*)&sym.second.section, sizeof(sym.second.section));
      binaryFile.write((char*)&sym.second.value, sizeof(sym.second.value));
      binaryFile.write((char*)&sym.second.isGlobal, sizeof(sym.second.isGlobal));
      binaryFile.write((char*)&sym.second.isExtern, sizeof(sym.second.isExtern));
      binaryFile.write((char*)&sym.second.number, sizeof(sym.second.number));
      binaryFile.write((char*)&sym.second.size, sizeof(sym.second.size));
      
    }
    unsigned numRelocs = relocationTable.size();
    binaryFile.write((char*)&numRelocs, sizeof(numRelocs));
    for(auto &rel: relocationTable){
      binaryFile.write((char*)&rel.second.type, sizeof(rel.second.type));
      binaryFile.write((char*)&rel.second.section, sizeof(rel.second.section));
      binaryFile.write((char*)&rel.second.offset, sizeof(rel.second.offset));
      binaryFile.write((char*)&rel.second.symbolTableReference, sizeof(rel.second.symbolTableReference));
    }
    unsigned numSections = code.size();
    binaryFile.write((char*)&numSections, sizeof(numSections));
    for(auto &sec: code){
      int sectionId = sec.first;
      list<char> bytes = sec.second;
      unsigned sectionSize = bytes.size();
      binaryFile.write((char*)&sectionId, sizeof(sectionId));
      binaryFile.write((char*)&sectionSize, sizeof(sectionSize));
      for(char byte: bytes){
        binaryFile.write(&byte, sizeof(byte));
      }
    }
    binaryFile.close();
  }
}

void Asembler::processLiteralPool(){
  for(auto& x: literalPool){
    string section = x.first.first;
    int value = x.first.second;
    Symbol& s = symbolTable.at(section);
    code[s.section].push_back((char)value);
    code[s.section].push_back((char)(value >> 8));
    code[s.section].push_back((char)(value >> 16));
    code[s.section].push_back((char)(value >> 24));
    for(auto& o: x.second){
      int offset = o;
      int displacement = s.size - offset - 4;
      auto iterator = code[s.section].begin();
      advance(iterator, offset);
      *iterator = displacement;
      advance(iterator, 1);
      *iterator = displacement >> 8 | *iterator;
    }
    s.size += 4;
  }
}

void Asembler::assemble(const char* input, const char* output){
  
  lc = 0;
  currSection = 0;
  currSectionName = "UND";
  Symbol undSection;
  undSection.name = "UND";
  undSection.section = 0;
  undSection.value = 0;
  undSection.isGlobal = false;
  undSection.isExtern = false;
  undSection.number = currNumber;
  currNumber++;
  undSection.size = 0;
  symbolTable.insert({"UND", undSection});
  processInputFile(input);
  processAsmLines();
  processLiteralPool();
  createRelocatonTable();
  makeOutputFile(output);
}

int main(int argc, char* argv[]){
  if(1){    //if(argv[0] == "./asembler"){
    if(argc != 4){
      cout << "Error!";
      return -1;
    }else if(strcmp(argv[1], "-o") != 0){
      cout << "Error!";
      return -2;
    }
    Asembler asembler;
    asembler.assemble(argv[3], argv[2]);
    return 0;
  }
}