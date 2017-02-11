// os_timer_interrupt.c -- simple timer interrupt demo

// instruction set
enum {
  HALT,ENT ,LEV ,JMP ,JMPI,JSR ,JSRA,LEA ,LEAG,CYC ,MCPY,MCMP,MCHR,MSET, // system & extended
  LL  ,LLS ,LLH ,LLC ,LLB ,LLD ,LLF ,LG  ,LGS ,LGH ,LGC ,LGB ,LGD ,LGF , // load a
  LX  ,LXS ,LXH ,LXC ,LXB ,LXD ,LXF ,LI  ,LHI ,LIF ,
  LBL ,LBLS,LBLH,LBLC,LBLB,LBLD,LBLF,LBG ,LBGS,LBGH,LBGC,LBGB,LBGD,LBGF, // load b
  LBX ,LBXS,LBXH,LBXC,LBXB,LBXD,LBXF,LBI ,LBHI,LBIF,LBA ,LBAD,
  SL  ,SLH ,SLB ,SLD ,SLF ,SG  ,SGH ,SGB ,SGD ,SGF ,                     // store a
  SX  ,SXH ,SXB ,SXD ,SXF ,
  ADDF,SUBF,MULF,DIVF,                                                   // arithmetic
  ADD ,ADDI,ADDL,SUB ,SUBI,SUBL,MUL ,MULI,MULL,DIV ,DIVI,DIVL,
  DVU ,DVUI,DVUL,MOD ,MODI,MODL,MDU ,MDUI,MDUL,AND ,ANDI,ANDL,
  OR  ,ORI ,ORL ,XOR ,XORI,XORL,SHL ,SHLI,SHLL,SHR ,SHRI,SHRL,
  SRU ,SRUI,SRUL,EQ  ,EQF ,NE  ,NEF ,LT  ,LTU ,LTF ,GE  ,GEU ,GEF ,      // logical
  BZ  ,BZF ,BNZ ,BNZF,BE  ,BEF ,BNE ,BNEF,BLT ,BLTU,BLTF,BGE ,BGEU,BGEF, // conditional
  CID ,CUD ,CDI ,CDU ,                                                   // conversion
  CLI ,STI ,RTI ,BIN ,BOUT,NOP ,SSP ,PSHA,PSHI,PSHF,PSHB,POPB,POPF,POPA, // misc
  IVEC,PDIR,SPAG,TIME,LVAD,TRAP,LUSP,SUSP,LCL ,LCA ,PSHC,POPC,MSIZ,
  PSHG,POPG,NET1,NET2,NET3,NET4,NET5,NET6,NET7,NET8,NET9,
  POW ,ATN2,FABS,ATAN,LOG ,LOGT,EXP ,FLOR,CEIL,HYPO,SIN ,COS ,TAN ,ASIN, // math
  ACOS,SINH,COSH,TANH,SQRT,FMOD,
  IDLE                                                                   // do nothing
};

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
// output a char to screen
out(port, val)  { asm(LL,8); asm(LBL,16); asm(BOUT); }
// halt v9 system
halt(val)       { asm(LL,8); asm(HALT); }
// set the base addr of interrupt/exception handler
ivec(void *isr) { asm(LL,8); asm(IVEC); }
// set the timer
stmr(int val)   { asm(LL,8); asm(TIME); }

// the interrupt/exception hander
alltraps()
{
// store the general registers:a,b
   asm(PSHA);
   asm(PSHB);
// output some timer info
   out(1, '\n'); out(1, 'T');out(1, 'i');out(1, 'c');out(1, 'k');out(1, '\n');
// restore the general registers:a,b
   asm(POPB);
   asm(POPA);
// back to interrupted prog
   asm(RTI);
}

int current;
main()
{
   out(1, 'H');out(1, 'e');out(1, 'l');out(1, 'l');out(1, 'o');out(1, ' ');
   out(1, 'W');out(1, 'o');out(1, 'r');out(1, 'l');out(1, 'd');out(1, '!');
   out(1, '\n');
   current = 0;

   stmr(5000);
   ivec(alltraps);
  
   task1_sp = &task1_stack;
   task1_sp += 50;
  
  
   task1_sp -= 2; *task1_sp = &task1;
   task1_sp -= 2; *task1_sp = 0; // fault code
   task1_sp -= 2; *task1_sp = 0; // a  
   task1_sp -= 2; *task1_sp = 0; // b  
   task1_sp -= 2; *task1_sp = 0; // c  
   task1_sp -= 2; *task1_sp = &trapret;  
  
   asm(STI);
  
   task0();
}
