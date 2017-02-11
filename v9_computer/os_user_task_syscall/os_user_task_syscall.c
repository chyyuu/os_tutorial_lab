// os_user_task_syscall.c -- one user task and implement write syscall

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

// system calls
enum {
  S_fork=1, S_exit,   S_wait,   S_pipe,   S_write,  S_read,   S_close,  S_kill,
  S_exec,   S_open,   S_mknod,  S_unlink, S_fstat,  S_link,   S_mkdir,  S_chdir,
  S_dup2,   S_getpid, S_sbrk,   S_sleep,  S_uptime, S_lseek,  S_mount,  S_umount,
  S_socket, S_bind,   S_listen, S_poll,   S_accept, S_connect, 
};

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

enum {    // processor fault codes
  FMEM,   // bad physical address
  FTIMER, // timer interrupt
  FKEYBD, // keyboard interrupt
  FPRIV,  // privileged instruction
  FINST,  // illegal instruction
  FSYS,   // software trap
  FARITH, // arithmetic trap
  FIPAGE, // page fault on opcode fetch
  FWPAGE, // page fault on write
  FRPAGE, // page fault on read
  USER=16 // user mode exception 
};

char taskA_stack[1000];
char taskA_kstack[1000];

int *taskA_sp;

int current;

out(port, val)  { asm(LL,8); asm(LBL,16); asm(BOUT); }
ivec(void *isr) { asm(LL,8); asm(IVEC); }
stmr(int val)   { asm(LL,8); asm(TIME); }
halt(value)     { asm(LL,8); asm(HALT); }

sys_write(fd, char *p, n) { int i; for (i=0; i<n; i++) out(fd, p[i]); return i; }

write() { asm(LL,8); asm(LBL,16); asm(LCL,24); asm(TRAP,S_write); }

taskA()
{
  while(current < 10)
    write(1, "_", 1);

  write(1,"taskA exit\n", 11);
  halt(0);
}

trap(int *sp, int c, int b, int a, int fc, unsigned *pc)
//trap(int *sp, int c, int b, int a, int fc, unsigned *pc)
{
  switch (fc) {
  case FSYS + USER: // syscall
    switch (pc[-1] >> 8) {
    case S_write: a = sys_write(a, b, c); break;
    default: sys_write(1, "panic! unknown syscall\n", 23); asm(HALT);
    }
    break;
    
  case FTIMER:  // timer
  case FTIMER + USER:
    out(1,'x');
    current++;
    break;
    
  default:
    default: sys_write(1, "panic! unknown interrupt\n", 25); asm(HALT);  
  }
}

alltraps()
{
  asm(PSHA);
  asm(PSHB);
  asm(PSHC);
  asm(LUSP);
  asm(PSHA);
  trap();                // registers passed by reference/magic
  asm(POPA);
  asm(SUSP);
  asm(POPC);
  asm(POPB);
  asm(POPA);
  asm(RTI);
}

trapret()
{
  asm(POPA); //a=*sp, sp+=8 byte
  asm(SUSP); //usp=a
  asm(POPC); //c=*sp, sp+=8 byte
  asm(POPB); //b=*sp, sp+=8 byte
  asm(POPA); //a=*sp, sp+=8 byte
  asm(RTI);  //return from interrupt, mode=USER, pc=&taskA
}

main()
{
  int *kstack;
  
  stmr(5000);
  ivec(alltraps);

  taskA_sp = &taskA_kstack[1000];
  taskA_sp -= 2; *taskA_sp = &taskA;
  taskA_sp -= 2; *taskA_sp = USER; // fault code
  taskA_sp -= 2; *taskA_sp = 0; // a
  taskA_sp -= 2; *taskA_sp = 0; // b
  taskA_sp -= 2; *taskA_sp = 0; // c
  taskA_sp -= 2; *taskA_sp = &taskA_stack[1000]; //user stack
  taskA_sp -= 2; *taskA_sp = &trapret;

  kstack = taskA_sp; //kernel stack
  
  asm(LL, 4); // a = kstack
  asm(SSP);   // sp = a
  asm(LEV);   // return
}
