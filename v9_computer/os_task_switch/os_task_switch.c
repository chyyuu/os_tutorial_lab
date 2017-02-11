// os_task_switch.c -- using timer interrupt to switch two tasks

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

char taskB_stack[200];
int *taskA_sp;
int *taskB_sp;
int current;

// output a char to screen
out(int port, int val)  { asm(LL,8); asm(LBL,16); asm(BOUT); }
// set the base addr of interrupt/exception handler
ivec(void *isr) { asm(LL,8); asm(IVEC); }
// set the timer
stmr(int val)   { asm(LL,8); asm(TIME); }
// halt v9 system
halt(int value)     { asm(LL,8); asm(HALT); }
// output string to screen
write(int fd, char *p, int n) { while (n--) out(fd, *p++); }

taskA()
{
  while(current < 5) {
    write(1, "A", 1);
  }
  write(1,"\ntaskA exit\n", 12);
  halt(0);
}

taskB()
{
  while(current < 6) {
    write(1, "B", 1);
  }
  write(1,"\ntaskB exit\n", 12);
  halt(0);
}

swtch(int *old, int new) // switch stacks
{
  asm(LEA, 0); // a = sp
  asm(LBL, 8); // b = old
  asm(SX, 0);  // *b = a
  asm(LL, 16); // a = new
  asm(SSP);    // sp = a
}

trap()
{
  if (++current & 1)
    swtch(&taskA_sp, taskB_sp);
  else
    swtch(&taskB_sp, taskA_sp);
}

alltraps()
{
  asm(PSHA); asm(PSHB);  asm(PSHC);
  trap();
  asm(POPC); asm(POPB);  asm(POPA);
  asm(RTI);
}

trapret()
{
  asm(POPC); asm(POPB); asm(POPA);
  asm(RTI);
}

main()
{
  current = 0;

  stmr(5000);
  ivec(alltraps);
  
  taskB_sp = &taskB_stack;
  taskB_sp += 50;
  
  
  taskB_sp -= 2; *taskB_sp = &taskB;
  taskB_sp -= 2; *taskB_sp = 0; // fault code
  taskB_sp -= 2; *taskB_sp = 0; // a  
  taskB_sp -= 2; *taskB_sp = 0; // b  
  taskB_sp -= 2; *taskB_sp = 0; // c  
  taskB_sp -= 2; *taskB_sp = &trapret;  
  
  asm(STI);
  
  taskA();
}
