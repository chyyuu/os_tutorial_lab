
enum {
  HALT,ENT,LEV,JMP,JMPI,JSR,JSRA,LEA,LEAG,CYC,MCPY,MCMP,MCHR,MSET,
  LL,LLS,LLH,LLC,LLB,LLD,LLF,LG,LGS,LGH,LGC,LGB,LGD,LGF,
  LX,LXS,LXH,LXC,LXB,LXD,LXF,LI,LHI,LIF,
  LBL,LBLS,LBLH,LBLC,LBLB,LBLD,LBLF,LBG,LBGS,LBGH,LBGC,LBGB,LBGD,LBGF,
  LBX,LBXS,LBXH,LBXC,LBXB,LBXD,LBXF,LBI,LBHI,LBIF,LBA,LBAD,
  SL,SLH,SLB,SLD,SLF,SG,SGH,SGB,SGD,SGF,
  SX,SXH,SXB,SXD,SXF,
  ADDF,SUBF,MULF,DIVF,
  ADD,ADDI,ADDL,SUB,SUBI,SUBL,MUL,MULI,MULL,DIV,DIVI,DIVL,
  DVU,DVUI,DVUL,MOD,MODI,MODL,MDU,MDUI,MDUL,AND,ANDI,ANDL,
  OR,ORI,ORL,XOR,XORI,XORL,SHL,SHLI,SHLL,SHR,SHRI,SHRL,
  SRU,SRUI,SRUL,EQ,EQF,NE,NEF,LT,LTU,LTF,GE,GEU,GEF,
  BZ,BZF,BNZ,BNZF,BE,BEF,BNE,BNEF,BLT,BLTU,BLTF,BGE,BGEU,BGEF,
  CID,CUD,CDI,CDU,
  CLI,STI,RTI,BIN,BOUT,NOP,SSP,PSHA,PSHI,PSHF,PSHB,POPB,POPF,POPA,
  IVEC,PDIR,SPAG,TIME,LVAD,TRAP,LUSP,SUSP,LCL,LCA,PSHC,POPC,MSIZ,
  PSHG,POPG,NET1,NET2,NET3,NET4,NET5,NET6,NET7,NET8,NET9,
  POW,ATN2,FABS,ATAN,LOG,LOGT,EXP,FLOR,CEIL,HYPO,SIN,COS,TAN,ASIN,
  ACOS,SINH,COSH,TANH,SQRT,FMOD,
  IDLE
};

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef int bool;

typedef char int8_t;
typedef uchar uint8_t;
typedef short int16_t;
typedef ushort uint16_t;
typedef int int32_t;
typedef uint uint32_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

typedef uintptr_t size_t;

typedef size_t ppn_t;

uint ROUNDDOWN(uint a, uint n) {
  return a - a % n;

}

uint ROUNDUP(uint a, uint n) {
  return ROUNDDOWN(a + n - 1, n);
}

static int
trap(uint code, uint* args) {
  asm (LL, 8);
  asm (LBL, 16);
  asm (LCL, 24);
  asm (TRAP);
}

static int
syscall(int num, ...) {
  va_list ap;
  uint32_t a[5];
  uint32_t argtmp[5];
  int i, ret;
  va_start(ap, num);
  for (i = 0; i < 5; i++) {
    a[i] = va_arg(ap, uint32_t);
  }
  if (num == 102) {
    argtmp[0] = a[1];
    argtmp[1] = a[2];
    argtmp[2] = a[3];
    argtmp[3] = a[4];
    ret = trap(num, a[0], argtmp);
  } else {
    ret = trap(num, a[0], a[1]);
  }
  return ret;
}

int
sys_exit(int error_code) {
  return syscall(1, error_code);
}

int
sys_fork(void) {
  return syscall(2);
}

int
sys_wait(int pid, int *store) {
  return syscall(3, pid, store);
}

int
sys_yield(void) {
  return syscall(10);
}

int
sys_kill(int pid) {
  return syscall(12, pid);
}

int
sys_getpid(void) {
  return syscall(18);
}

int
sys_putc(int c) {
  return syscall(30, c);
}

int
sys_pgdir(void) {
  return syscall(31);
}

int
sys_gettime(void) {
  return syscall(17);
}

int
sys_read(int fd, void *base, size_t len) {
  return syscall(102, fd, base, len);
}

int
sys_open(char *path, uint32_t open_flags) {
  return syscall(100, path, open_flags);
}

int
sys_close(int fd) {
  return syscall(101, fd);
}

int sys_exec(char *name) {
  return syscall(4, name);
}

void
sys_lab6_set_priority(uint32_t priority)
{
  syscall(255, priority);
}

typedef unsigned int funcptr_t;

void call0(funcptr_t func) {
  asm (LL, 8);
  asm (PSHA);
  asm (LEV);
}

void call1(int arg0, funcptr_t func) {
  asm (LL, 16);
  asm (PSHA);
  asm (LEV);
}

void call2(int arg0, int arg1, funcptr_t func) {
  asm (LL, 24);
  asm (PSHA);
  asm (LEV);
}

void call3(int arg0, int arg1, int arg2, funcptr_t func) {
  asm (LL, 32);
  asm (PSHA);
  asm (LEV);
}

void call4(int arg0, int arg1, int arg2, int arg3, funcptr_t func) {
  asm (LL, 40);
  asm (PSHA);
  asm (LEV);
}

void *memcpy(void *d, void *s, uint n) {
  asm (LL,8); asm (LBL, 16); asm (LCL,24); asm (MCPY); asm (LL,8);
}
void *memset(void *d, uint c, uint n) {
  asm (LL,8); asm (LBLB,16); asm (LCL,24); asm (MSET); asm (LL,8);
}
void *memchr(void *s, uint c, uint n) {
  asm (LL,8); asm (LBLB,16); asm (LCL,24); asm (MCHR);
}

xstrncpy(char *s, char *t, int n) {
  while (n-- > 0 && (*s++ = *t++));
  while (n-- > 0) *s++ = 0;
}

safestrcpy(char *s, char *t, int n) {
  if (n <= 0) return;
  while (--n > 0 && (*s++ = *t++));
  *s = 0;
}

size_t
strlen(char *s) {
  size_t cnt = 0;
  while (*s != '\0') {
    s++;
    cnt++;
  }
  return cnt;
}

size_t
strnlen(char *s, size_t len) {
  size_t cnt = 0;
  while (cnt < len && *s++ != '\0') {
    cnt++;
  }
  return cnt;
}

char *
strcpy(char *dst, char *src) {

  char *p = dst;
  while ((*p++ = *src++) != '\0')
                 ;
  return dst;

}

char *
strncpy(char *dst, char *src, size_t len) {
  char *p = dst;
  while (len > 0) {
    if ((*p = *src) != '\0') {
      src++;
    }
    p++, len--;
  }
  return dst;
}

int
strcmp(char *s1, char *s2) {

  while (*s1 != '\0' && *s1 == *s2) {
    s1++, s2++;
  }
  return (int)((unsigned char)*s1 - (unsigned char)*s2);

}

int
strncmp(char *s1, char *s2, size_t n) {
  while (n > 0 && *s1 != '\0' && *s1 == *s2) {
    n--, s1++, s2++;
  }
  return (n == 0) ? 0 : (int)((unsigned char)*s1 - (unsigned char)*s2);
}

char *
strchr(char *s, char c) {
  while (*s != '\0') {
    if (*s == c) {
      return (char *)s;
    }
    s++;
  }
  return ((void *)0);
}

char *
strfind(char *s, char c) {
  while (*s != '\0') {
    if (*s == c) {
      break;
    }
    s++;
  }
  return (char *)s;
}

static char * error_string[24 + 1];

void
printfmt(void (*putch)(int, void*, int), int fd, void *putdat, char *fmt, ...);

void
vprintfmt(funcptr_t putch, int fd, void *putdat, char *fmt, va_list ap);

static uint32_t getuint(va_list *ap, int lflag) {
  return va_arg(*ap, unsigned int);
}

static int32_t getint(va_list *ap, int lflag) {
  return va_arg(*ap, int);
}

static void
printnum(funcptr_t putch, int fd, void *putdat, unsigned int num, unsigned int base, int width, int padc) {
  unsigned int result = num;
  unsigned mod;
  mod = result % base;
  result /= base;

  if (num >= base) {
    printnum(putch, fd, putdat, result, base, width - 1, padc);
  } else {

    while (--width > 0)
      call3(padc, putdat, fd, putch);
  }

  call3("0123456789abcdef"[mod], putdat, fd, putch);

}

void
vprintfmt(funcptr_t putch, int fd, void *putdat, char *fmt, va_list ap) {
  char *p;
  int ch, err;
  int num;
  int base, width, precision, lflag, altflag;
  char padc;
  while (1) {
    while ((ch = *(unsigned char *)fmt++) != '%') {
      if (ch == '\0') {
 return;
      }
      call3(ch, putdat, fd, putch);
    }

    padc = ' ';
    width = precision = -1;
    lflag = altflag = 0;

reswitch:
    switch (ch = *(unsigned char *)fmt++) {

    case '-':
      padc = '-';
      goto reswitch;

    case '0':
      padc = '0';
      goto reswitch;

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      for (precision = 0;; ++fmt) {
 precision = precision * 10 + ch - '0';
 ch = *fmt;
 if (ch < '0' || ch > '9') {
   break;
 }
      }
      goto process_precision;

    case '*':
      precision = va_arg(ap, int);
      goto process_precision;

    case '.':
      if (width < 0)
 width = 0;
      goto reswitch;

    case '#':
      altflag = 1;
      goto reswitch;

process_precision:
      if (width < 0)
 width = precision, precision = -1;
      goto reswitch;

    case 'l':
      lflag++;
      goto reswitch;

    case 'c':
      call3(va_arg(ap, int), putdat, fd, putch);
      break;

    case 'e':
      err = va_arg(ap, int);
      if (err < 0) {
 err = -err;
      }
      if (err > 24 || (p = error_string[err]) == ((void *)0)) {
 printfmt(putch, fd, putdat, "error %d", err);
      }
      else {
 printfmt(putch, fd, putdat, "%s", p);
      }
      break;

    case 's':
      if ((p = va_arg(ap, char *)) == ((void *)0)) {
 p = "(null)";
      }
      if (width > 0 && padc != '-') {
 for (width -= strnlen(p, precision); width > 0; width--) {
   call3(padc, putdat, fd, putch);
 }
      }
      for (; (ch = *p++) != '\0' && (precision < 0 || --precision >= 0); width--) {
 if (altflag && (ch < ' ' || ch > '~')) {
   call3('?', putdat, fd, putch);
 }
 else {
   call3(ch, putdat, fd, putch);
 }
      }
      for (; width > 0; width--) {
 call3(' ', putdat, fd, putch);
      }
      break;

    case 'd':
      num = getint(&ap, lflag);
      if (num < 0) {
 call3('-', putdat, fd, putch);
 num = -num;
      }
      base = 10;
      goto number;

    case 'u':
      num = getuint(&ap, lflag);
      base = 10;
      goto number;

    case 'o':
      num = getuint(&ap, lflag);
      base = 8;
      goto number;

    case 'p':
      call3('0', putdat, fd, putch);
      call3('x', putdat, fd, putch);
      num = (uintptr_t)va_arg(ap, void *);
      base = 16;
      goto number;

    case 'x':
      num = getuint(&ap, lflag);
      base = 16;
number:
      printnum(putch, fd, putdat, num, base, width, padc);
      break;

    case '%':
      call3(ch, putdat, fd, putch);
      break;

    default:
      call3('%', putdat, fd, putch);
      for (fmt--; fmt[-1] != '%'; fmt--)
                 ;
      break;
    }
  }
}

void
printfmt(void (*putch)(int, void*, int), int fd, void *putdat, char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  vprintfmt(putch, fd, putdat, fmt, ap);

}

struct sprintbuf {
  char *buf;
  char *ebuf;
  int cnt;
};

static void
sprintputch(int ch, struct sprintbuf *b) {
  b->cnt++;
  if (b->buf < b->ebuf) {
    *b->buf++ = ch;
  }
}

int
vsnprintf(char *str, size_t size, char *fmt, va_list ap) {
  struct sprintbuf b;
  b.buf = str;
  b.ebuf = str + size - 1;
  b.cnt = 0;
  if (str == ((void *)0) || b.buf > b.ebuf) {
    return -3;
  }

  vprintfmt((void*)sprintputch, -0x9527, &b, fmt, ap);

  *b.buf = '\0';
  return b.cnt;
}

int
snprintf(char *str, size_t size, char *fmt, ...) {
  va_list ap;
  int cnt;
  va_start(ap, fmt);
  cnt = vsnprintf(str, size, fmt, ap);

  return cnt;
}

static void
cputch(int c, int *cnt) {
  sys_putc(c);
  (*cnt)++;
}

int
vcprintf(char *fmt, va_list ap) {
  int cnt = 0;
  vprintfmt((void*)cputch, -0x9527, &cnt, fmt, ap);
  return cnt;
}

int
cprintf(char *fmt, ...) {
  va_list ap;
  int cnt;

  va_start(ap, fmt);
  cnt = vcprintf(fmt, ap);

  return cnt;
}

int
cputs(char *str) {
  int cnt = 0;
  char c;
  while ((c = *str++) != '\0') {
    cputch(c, &cnt);
  }
  cputch('\n', &cnt);
  return cnt;
}

void
exit(int error_code);

void
__panic(char *file, int line, char *fmt, ...) {

  va_list ap;
  va_start(ap, fmt);
  cprintf("user panic at %s:%d:\n    ", file, line);
  vcprintf(fmt, ap);
  cprintf("\n");

  exit(-10);
}

void
__warn(char *file, int line, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  cprintf("user warning at %s:%d:\n    ", file, line);
  vcprintf(fmt, ap);
  cprintf("\n");

}

void
exit(int error_code) {
  sys_exit(error_code);
  cprintf("BUG: exit failed.\n");
  while (1) ;
}

int
fork(void) {
  return sys_fork();
}

int
wait(void) {
  return sys_wait(0, ((void *)0));
}

int
waitpid(int pid, int *store) {
  return sys_wait(pid, store);
}

void
yield(void) {
  sys_yield();
}

int
kill(int pid) {
  return sys_kill(pid);
}

int
getpid(void) {
  return sys_getpid();
}

int exec(char* name) {
  return sys_exec(name);
}

void
print_pgdir(void) {
  sys_pgdir();
}

unsigned int
gettime_msec(void) {
  return (unsigned int)sys_gettime();
}

void
lab6_set_priority(uint32_t priority)
{
  sys_lab6_set_priority(priority);
}

int umain(void);

int main(void) {
  int ret = umain();
  exit(ret);
}

int
umain(void) {
  int pid, ret;
  cprintf("I am the parent. Forking the child...\n");
  if ((pid = fork()) == 0) {
    cprintf("I am the child. spinning ...\n");
    while (1) ;
  }
  cprintf("I am the parent. Running the child...\n");
  yield();
  yield();
  yield();

  cprintf("I am the parent.  Killing the child...\n");

  do { if (!((ret = kill(pid)) == 0)) { __panic("ucore/user/spin.c", 20, "assertion failed: %s", "(ret = kill(pid)) == 0"); } } while (0);
  cprintf("kill returns %d\n", ret);

  do { if (!((ret = waitpid(pid, ((void *)0))) == 0)) { __panic("ucore/user/spin.c", 23, "assertion failed: %s", "(ret = waitpid(pid, NULL)) == 0"); } } while (0);
  cprintf("wait returns %d\n", ret);

  cprintf("spin may pass.\n");
  return 0;
}
