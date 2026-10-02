#include <regex>
#include <list>
#include <map>
using namespace std;

class Asembler{
  public:
  void assemble(const char* input, const char* output);

  private:
  struct Info{
    int addr;
    int type; // 0 - apsolutan; 1 - relativan
    int section;
    bool word;
  };

  struct Symbol{
    string name;
    int section;
    int value;
    bool isGlobal;
    bool isExtern;
    int number;
    int size;
    list<Info> infoList;
  };

  struct RelocationTableEntry{
    int type;
    int offset;
    int section;
    int symbolTableReference;
  };

  list<string> asmLines;
  void processInputFile(const char *input);
  void processAsmLines();
  void makeOutputFile(const char *output);

  map<string, Symbol> symbolTable;
  multimap<int, RelocationTableEntry> relocationTable;

  static int currNumber;
  int lc; //location counter
  int currSection;
  string currSectionName;

  map<int, list<char>> code;

  map<pair<string, int>, list<int>> literalPool;
  void processLiteralPool();

  regex comment = regex("^([^#]*)#.*");
  regex tab = regex("\\t");
  regex spaces = regex(" {2,}");
  regex comma_space = regex(" ?, ?");
  regex label_space = regex(" ?: ?");
  regex boundary_spaces = regex("^( *)([^ ].*[^ ])( *)$");

  regex label = regex("^([a-zA-Z][a-zA-Z0-9_]*):$");
  regex label_ins_or_dir = regex("^([a-zA-Z][a-zA-Z0-9_]*):(.*)$");
  
  regex dir_global = regex("^\\.global ([a-zA-Z][a-zA-Z0-9_]*(, [a-zA-Z][a-zA-Z0-9_]*)*)$");
  regex dir_extern = regex("^\\.extern ([a-zA-Z][a-zA-Z0-9_]*(, [a-zA-Z][a-zA-Z0-9_]*)*)$");
  regex dir_section = regex("^\\.section ([a-zA-Z][a-zA-Z0-9_]*)$");
  regex dir_word = regex("^\\.word (([a-zA-Z][a-zA-Z0-9_]*|-?[0-9]+|0x[0-9a-fA-F]+)(, ([a-zA-Z][a-zA-Z0-9_]*|-?[0-9]+|0x[0-9a-fA-F]+))*)$");
  regex dir_skip = regex("^\\.skip ([0-9]+|0x[0-9a-fA-F]+)$");
  regex dir_end = regex("^\\.end$");

  regex literal_dec = regex("^[0-9]+$");
  regex literal_hex = regex("^0x[0-9a-fA-F]+$");
  regex simbol = regex("^([a-zA-Z][a-zA-Z0-9_]*)$");

  regex ins_halt = regex("^halt$");
  regex ins_int = regex("^int$");
  regex ins_iret = regex("^iret$");
  regex ins_call_l = regex("^call ([0-9]+|0x[0-9A-Fa-f]+)$");
  regex ins_call_s = regex("^call ([a-zA-Z][a-zA-Z0-9_]*)$");
  regex ins_ret = regex("^ret$");
  regex ins_jmp_l = regex("^jmp ([0-9]+|0x[0-9A-Fa-f]+)$");
  regex ins_jmp_s = regex("^jmp ([a-zA-Z][a-zA-Z0-9_]*)$");
  regex ins_beq_l = regex("^beq %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc), ([0-9]+|0x[0-9A-Fa-f]+)$");
  regex ins_beq_s = regex("^beq %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc), ([a-zA-Z][a-zA-Z0-9_]*)$");
  regex ins_bne_l = regex("^bne %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc), ([0-9]+|0x[0-9A-Fa-f]+)$");
  regex ins_bne_s = regex("^bne %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc), ([a-zA-Z][a-zA-Z0-9_]*)$");
  regex ins_bgt_l = regex("^bgt %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc), ([0-9]+|0x[0-9A-Fa-f]+)$");
  regex ins_bgt_s = regex("^bgt %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc), ([a-zA-Z][a-zA-Z0-9_]*)$");
  regex ins_push = regex("^push %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_pop = regex("^pop %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_xchg = regex("^xchg %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_add = regex("^add %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_sub = regex("^sub %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_mul = regex("^mul %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_div = regex("^div %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_not = regex("^not %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_and = regex("^and %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_or = regex("^or %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_xor = regex("^xor %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_shl = regex("^shl %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_shr = regex("^shr %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_imm_l = regex("^ld \\$([0-9]+|0x[0-9A-Fa-f]+), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_imm_s = regex("^ld \\$([a-zA-Z][a-zA-Z0-9_]*), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_mem_dir_l = regex("^ld ([0-9]+|0x[0-9A-Fa-f]+), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_mem_dir_s = regex("^ld ([a-zA-Z][a-zA-Z0-9_]*), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_reg_dir = regex("^ld %(r([0-9]|1[0-5])|sp|pc), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_reg_ind = regex("^ld \\[%(r([0-9]|1[0-5])|sp|pc)\\], %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_reg_ind_pom_l = regex("^ld \\[%(r([0-9]|1[0-5])|sp|pc) \\+ ([0-9]+|0x[0-9A-Fa-f]+)\\], %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_ld_reg_ind_pom_s = regex("^ld \\[%(r([0-9]|1[0-5])|sp|pc) \\+ ([a-zA-Z][a-zA-Z0-9_]*)\\], %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_st_mem_dir_l = regex("^st %(r([0-9]|1[0-5])|sp|pc), ([0-9]+|0x[0-9A-Fa-f]+)$");
  regex ins_st_mem_dir_s = regex("^st %(r([0-9]|1[0-5])|sp|pc), ([a-zA-Z][a-zA-Z0-9_]*)$");
  regex ins_st_reg_ind = regex("^st %(r([0-9]|1[0-5])|sp|pc), \\[%(r([0-9]|1[0-5])|sp|pc)\\]$");
  regex ins_st_reg_ind_pom_l = regex("^st %(r([0-9]|1[0-5])|sp|pc), \\[%(r([0-9]|1[0-5])|sp|pc) \\+ ([0-9]+|0x[0-9A-Fa-f]+)\\]$");
  regex ins_st_reg_ind_pom_s = regex("^st %(r([0-9]|1[0-5])|sp|pc), \\[%(r([0-9]|1[0-5])|sp|pc) \\+ ([a-zA-Z][a-zA-Z0-9_]*)\\]$");
  regex ins_csrrd = regex("^csrrd %(status|handler|cause), %(r([0-9]|1[0-5])|sp|pc)$");
  regex ins_csrwr = regex("^csrwr %(r([0-9]|1[0-5])|sp|pc), %(status|handler|cause)$");

  int processLabel(string str);
  int processGlobal(string str);
  int processExtern(string str);
  int processSection(string str);
  int processWord(string str);
  int processSkip(string str);
  int processHalt();
  int processInt();
  int processIret();
  int processCall_l(string str);
  int processCall_s(string str);
  int processRet();
  int processJmp_l(string str);
  int processJmp_s(string str);
  int processBeq_l(string gpr1, string gpr2, string op);
  int processBeq_s(string gpr1, string gpr2, string op);
  int processBne_l(string gpr1, string gpr2, string op);
  int processBne_s(string gpr1, string gpr2, string op);
  int processBgt_l(string gpr1, string gpr2, string op);
  int processBgt_s(string gpr1, string gpr2, string op);
  int processPush(string gpr);
  int processPop(string gpr);
  int processXchg(string gprs, string gprd);
  int processAdd(string gprs, string gprd);
  int processSub(string gprs, string gprd);
  int processMul(string gprs, string gprd);
  int processDiv(string gprs, string gprd);
  int processNot(string str);
  int processAnd(string gprs, string gprd);
  int processOr(string gprs, string gprd);
  int processXor(string gprs, string gprdr);
  int processShl(string gprs, string gprd);
  int processShr(string gprs, string gprd);
  int processLd_imm_l(string op, string gpr); //ld op, gpr gpr <= op
  int processLd_imm_s(string op, string gpr);
  int processLd_mem_dir_l(string op, string gpr);
  int processLd_mem_dir_s(string op, string gpr);
  int processLd_reg_dir(string op, string gpr);
  int processLd_reg_ind(string op, string gpr);
  int processLd_reg_ind_pom_l(string op, string pom, string gpr);
  int processLd_reg_ind_pom_s(string op, string pom, string gpr);
  int processSt_mem_dir_l(string gpr, string op); //st gpr, op op <= gpr
  int processSt_mem_dir_s(string gpr, string op);
  int processSt_reg_ind(string gpr, string op);
  int processSt_reg_ind_pom_l(string gpr, string op, string pom);
  int processSt_reg_ind_pom_s(string gpr, string op, string pom);
  int processCsrrd(string csr, string gpr);
  int processCsrwr(string gpr, string csr);

  int createRelocatonTable();
};