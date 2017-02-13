
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

struct list_entry {
  struct list_entry *prev, *next;
};

typedef struct list_entry list_entry_t;

void
__list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) {
  prev->next = next->prev = elm;
  elm->next = next;
  elm->prev = prev;
}

void
__list_del(list_entry_t *prev, list_entry_t *next) {
  prev->next = next;
  next->prev = prev;
}

void
list_init(list_entry_t *elm) {
  elm->prev = elm->next = elm;
}

void
list_add_after(list_entry_t *listelm, list_entry_t *elm) {
  __list_add(elm, listelm, listelm->next);
}

void
list_add(list_entry_t *listelm, list_entry_t *elm) {
  list_add_after(listelm, elm);
}

void
list_add_before(list_entry_t *listelm, list_entry_t *elm) {
  __list_add(elm, listelm->prev, listelm);
}

void
list_del(list_entry_t *listelm) {
  __list_del(listelm->prev, listelm->next);
}

void
list_del_init(list_entry_t *listelm) {
  list_del(listelm);
  list_init(listelm);
}

bool
list_empty(list_entry_t *list) {
  return list->next == list;
}

list_entry_t *
list_next(list_entry_t *listelm) {
  return listelm->next;
}

list_entry_t *
list_prev(list_entry_t *listelm) {
  return listelm->prev;
}

int in(port) { asm(LL,8); asm(BIN); }
out(port, val) { asm(LL,8); asm(LBL,16); asm(BOUT); }
ivec(void *isr) { asm(LL,8); asm(IVEC); }
lvadr() { asm(LVAD); }
uint msiz() { asm(MSIZ); }
stmr(val) { asm(LL,8); asm(TIME); }
pdir(val) { asm(LL,8); asm(PDIR); }
spage(val) { asm(LL,8); asm(SPAG); }
splhi() { asm(CLI); }
splx(int e) { if (e) asm(STI); }

set_bit(int nr, uint* addr) {
  int e = splhi();
  *addr = (*addr) | (1 << nr);
  splx(e);
}

clear_bit(int nr, uint* addr) {
  int e = splhi();
  *addr = ((*addr) | (1 << nr)) ^ (1 << nr);
  splx(e);
}

change_bit(int nr, uint* addr) {
  int e = splhi();
  *addr = (*addr) ^ (1 << nr);
  splx(e);
}

int test_bit(int nr, uint* addr) {
  int e = splhi();
  if (((*addr) & (1 << nr)) == 0) {
    splx(e);
    return 0;
  }
  splx(e);
  return 1;
}

bool
test_and_set_bit(int nr, uint* addr) {
  int e = splhi();
  int oldbit = ((*addr) & (1 << nr));
  *addr = (*addr) | (1 << nr);
  splx(e);
  return oldbit != 0;
}

bool
test_and_clear_bit(int nr, uint* addr) {
  int e = splhi();
  int oldbit = ((*addr) & (1 << nr));
  *addr = ((*addr) | (1 << nr)) ^ (1 << nr);
  splx(e);
  return oldbit != 0;
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

static void cout(char c) {
  out(1, c);
}

static void cputch(int c, int *cnt, int unused) {
  cout(c);
  (*cnt)++;
}

void
cputchar(int c) {
  cout(c);
}

static int
vkprintf(char *fmt, va_list ap) {
  int cnt = 0;
  vprintfmt(cputch, -0x9527, &cnt, fmt, ap);
  return cnt;
}

int printf(char *fmt, ...) {
  va_list ap;
  int cnt;
  va_start(ap, fmt);
  cnt = vkprintf(fmt, ap);

  return cnt;
}

panic(char *s)
{
  asm (CLI);
  out(1,'p'); out(1,'a'); out(1,'n'); out(1,'i'); out(1,'c'); out(1,':'); out(1,' ');
  while (*s) out(1,*s++);
  out(1,'\n');
  asm (HALT);
}

assert(int x) {
  if (x == 0)
    panic("assertion failed: %s", x);
}

bool
__intr_save(void) {
  return splhi();
}

void
__intr_restore(bool flag) {
  splx(flag);
}

typedef bool lock_t;

void
lock_init(lock_t *lock) {
  *lock = 0;
}

bool
try_lock(lock_t *lock) {
  return !test_and_set_bit(0, lock);
}

void schedule(void);

void
lock(lock_t *lock) {
  while (!try_lock(lock)) {
    schedule();
  }
}

void
unlock(lock_t *lock) {
  if (!test_and_clear_bit(0, lock)) {
    panic("Unlock failed.\n");
  }
}

typedef uintptr_t pte_t;
typedef uintptr_t pde_t;
typedef pte_t swap_entry_t;

struct Page {
  int ref;
  uint32_t flags;
  unsigned int property;
  list_entry_t page_link;
  list_entry_t pra_page_link;
  uintptr_t pra_vaddr;
};

typedef struct {
  list_entry_t free_list;
  unsigned int nr_free;
} free_area_t;

char *memdisk;

struct Page *pages;

size_t npage = 0;

pde_t *boot_pgdir = ((void *)0);

uintptr_t boot_cr3;

uint endbss;
uint page_enable;
uint tlb_clear_enable;

uint PADDR(uint kva) {
  uint __m_kva;
  __m_kva = kva;
  if (page_enable == 1)
    return __m_kva - 0xC0000000;
  else
    return __m_kva;
}

uint KADDR(uint pa) {
  uint __m_pa;
  uint __m_ppn;
  __m_pa = pa;
  __m_ppn = (((uintptr_t)(__m_pa)) >> 12);
  if (page_enable == 1)
    return __m_pa + 0xC0000000;
  else
    return __m_pa;
}

ppn_t
page2ppn(struct Page *page) {
  return page - pages;
}

uintptr_t
page2pa(struct Page *page) {
  return page2ppn(page) << 12;
}

struct Page *
pa2page(uintptr_t pa) {
  if ((((uintptr_t)(pa)) >> 12) >= npage) {
    panic("pa2page called with invalid pa");
  }
  return &pages[(((uintptr_t)(pa)) >> 12)];
}

void *
page2kva(struct Page *page) {
  return KADDR(page2pa(page));
}

struct Page *
kva2page(void *kva) {
  return pa2page(PADDR(kva));
}

struct Page *
pte2page(pte_t pte) {
  if (!(pte & 0x001)) {
    panic("pte2page called with invalid pte");
  }
  return pa2page(((uintptr_t)(pte) & ~0xFFF));
}

struct Page *
pde2page(pde_t pde) {
  return pa2page(((uintptr_t)(pde) & ~0xFFF));
}

int
page_ref(struct Page *page) {
  return page->ref;
}

void
set_page_ref(struct Page *page, int val) {
  page->ref = val;
}

int
page_ref_inc(struct Page *page) {
  page->ref += 1;
  return page->ref;
}

int
page_ref_dec(struct Page *page) {
  page->ref -= 1;
  return page->ref;
}

pte_t * vpt = (pte_t *)(0xFAC00000);
pde_t * vpd = (pde_t *)((uintptr_t)((((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)) << 22 | (((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)) << 12 | (0)));

struct pmm_manager {
  char* name;
  void* init;
  void* init_memmap;
  void* alloc_pages;
  void* free_pages;
  void* nr_free_pages;
  void* check;
};

void load_default_pmm_manager();

struct pmm_manager* pmm_manager;
struct pmm_manager default_pmm_manager;

void
init_pmm_manager(void) {
  load_default_pmm_manager();
  pmm_manager = &default_pmm_manager;
  printf("memory management: %s\n", pmm_manager->name);
  call0(pmm_manager->init);
}

void
init_memmap(struct Page *base, size_t n) {
  call2(base, n, pmm_manager->init_memmap);
}

struct mm_struct;

struct vma_struct {
  struct mm_struct *vm_mm;
  uintptr_t vm_start;
  uintptr_t vm_end;
  uint32_t vm_flags;
  list_entry_t list_link;
};

struct mm_struct {
  list_entry_t mmap_list;
  struct vma_struct *mmap_cache;
  pde_t *pgdir;
  int map_count;
  void *sm_priv;
  int mm_count;
  lock_t mm_lock;
};

int swap_init_ok = 0;

struct mm_struct *check_mm_struct;

int swap_out(struct mm_struct *mm, int n, int in_tick);

struct Page *
alloc_pages(size_t n) {

  struct Page *page=((void *)0);
  bool intr_flag;

  while (1)
  {
    do { intr_flag = __intr_save(); } while (0);
    {
      page = call1(n, pmm_manager->alloc_pages);
    }
    __intr_restore(intr_flag);;

    if (page != ((void *)0) || n > 1 || swap_init_ok == 0) break;

    swap_out(check_mm_struct, n, 0);
  }
  return page;

}

void
free_pages(struct Page *base, size_t n) {
  bool intr_flag;
  do { intr_flag = __intr_save(); } while (0);
  {
    call2(base, n, pmm_manager->free_pages);
  }
  __intr_restore(intr_flag);;
}

size_t
nr_free_pages(void) {
  size_t ret;
  bool intr_flag;
  do { intr_flag = __intr_save(); } while (0);
  {
    ret = call0(default_pmm_manager.nr_free_pages);
  }
  __intr_restore(intr_flag);;
  return ret;
}

void page_init(void) {
  uintptr_t freemem;
  uint32_t maxpa, begin, end;
  int i;
  maxpa = msiz() - 1024 * 4096;
  memdisk = 0xC0000000 + maxpa;

  if (maxpa > 0x38000000) {
    maxpa = 0x38000000;
  }
  npage = maxpa / 4096;
  pages = (struct Page *)ROUNDUP((uint)(endbss) + 4096 + 3, 4096);
  for (i = 0; i < npage; i++) {
    set_bit(0, &((pages + i)->flags));
  }
  freemem = ((uintptr_t)pages + sizeof(struct Page) * npage);
  begin = freemem;
  end = maxpa;
  if (begin < end) {
    begin = ROUNDUP(begin, 4096);
    end = ROUNDDOWN(end, 4096);
    if (begin < end)
      init_memmap(pa2page(begin), (end - begin) / 4096);
  }
}

void enable_paging() {
  pdir(boot_cr3);
  spage(1);
}

void* boot_alloc_page(void) {
  struct Page *p = alloc_pages(1);
  if (p == ((void *)0)) {
    panic("boot_alloc_page failed.\n");
  }
  return page2pa(p);
}

pte_t* get_pte(pde_t *pgdir, uintptr_t la, bool create) {
  pde_t *pdep = &pgdir[((((uintptr_t)(la)) >> 22) & 0x3FF)];
  struct Page *page;
  uintptr_t pa;

  if (!(*pdep & 0x001)) {
    if (!create || (page = alloc_pages(1)) == ((void *)0)) {
      return ((void *)0);
    }
    set_page_ref(page, 1);
    pa = page2pa(page);
    memset(KADDR(pa), 0, 4096);
    *pdep = pa | 0x004 | 0x002 | 0x001;
  }
  return &((pte_t *)KADDR(((uintptr_t)(*pdep) & ~0xFFF)))[((((uintptr_t)(la)) >> 12) & 0x3FF)];
}

struct Page* get_page(pde_t *pgdir, uintptr_t la, pte_t **ptep_store) {
  pte_t *ptep = get_pte(pgdir, la, 0);
  if (ptep_store != ((void *)0)) {
    *ptep_store = ptep;
  }
  if (ptep != ((void *)0) && *ptep & 0x001) {
    return pte2page(*ptep);
  }
  return ((void *)0);
}

void tlb_invalidate(pde_t *pgdir, uintptr_t la) {
  if (tlb_clear_enable)
    spage(1);
}

void page_remove_pte(pde_t *pgdir, uintptr_t la, pte_t *ptep) {

  struct Page * page;
  if (*ptep & 0x001) {
    page = pte2page(*ptep);
    if (page_ref_dec(page) == 0) {
      free_pages(page, 1);
    }
    *ptep = 0;
    tlb_invalidate(pgdir, la);
  }
}

void page_remove(pde_t *pgdir, uintptr_t la) {
  pte_t *ptep = get_pte(pgdir, la, 0);
  if (ptep != ((void *)0)) {
    page_remove_pte(pgdir, la, ptep);
  }
}

int page_insert(pde_t *pgdir, struct Page *page, uintptr_t la, uint32_t perm) {
  pte_t *ptep = get_pte(pgdir, la, 1);
  struct Page *p;
  if (ptep == ((void *)0)) {

  }

  page_ref_inc(page);
  if (*ptep & 0x001) {
    p = pte2page(*ptep);
    if (p == page) {
      page_ref_dec(page);
    }
    else {
      page_remove_pte(pgdir, la, ptep);
    }
  }
  *ptep = page2pa(page) | 0x001 | perm;

  tlb_invalidate(pgdir, la);
  return 0;
}

int swap_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in);

struct Page *
pgdir_alloc_page(pde_t *pgdir, uintptr_t la, uint32_t perm) {

  struct Page *page = alloc_pages(1);

  if (page != ((void *)0)) {
    if (page_insert(pgdir, page, la, perm) != 0) {
      free_pages(page, 1);
      return ((void *)0);
    }
    if (swap_init_ok) {
      if(check_mm_struct!=((void *)0)) {
 swap_map_swappable(check_mm_struct, la, page, 0);
 page->pra_vaddr=la;
 assert(page_ref(page) == 1);

      }
      else {

      }
    }

  }

  return page;
}

void check_alloc_page(void) {
  call0(pmm_manager->check);
  printf("check_alloc_page() succeeded!\n");
}

void check_pgdir(void) {
  struct Page *p1, *p2;
  pte_t *ptep;

  assert(npage <= 0x38000000 / 4096);
  assert(boot_pgdir != ((void *)0) && (uint32_t)(((uintptr_t)(boot_pgdir)) & 0xFFF) == 0);
  assert(get_page(boot_pgdir, 0x0, ((void *)0)) == ((void *)0));

  p1 = alloc_pages(1);

  assert(page_insert(boot_pgdir, p1, 0x0, 0) == 0);
  assert((ptep = get_pte(boot_pgdir, 0x0, 0)) != ((void *)0));
  assert(pte2page(*ptep) == p1);
  assert(page_ref(p1) == 1);

  ptep = &((pte_t *)KADDR(((uintptr_t)(boot_pgdir[0]) & ~0xFFF)))[1];
  assert(get_pte(boot_pgdir, 4096, 0) == ptep);

  p2 = alloc_pages(1);
  assert(page_insert(boot_pgdir, p2, 4096, 0x004 | 0x002) == 0);
  assert((ptep = get_pte(boot_pgdir, 4096, 0)) != ((void *)0));
  assert(*ptep & 0x004);
  assert(*ptep & 0x002);
  assert(boot_pgdir[0] & 0x004);
  assert(page_ref(p2) == 1);

  assert(page_insert(boot_pgdir, p1, 4096, 0) == 0);
  assert(page_ref(p1) == 2);
  assert(page_ref(p2) == 0);
  assert((ptep = get_pte(boot_pgdir, 4096, 0)) != ((void *)0));
  assert(pte2page(*ptep) == p1);
  assert((*ptep & 0x004) == 0);

  page_remove(boot_pgdir, 0x0);
  assert(page_ref(p1) == 1);
  assert(page_ref(p2) == 0);

  page_remove(boot_pgdir, 4096);
  assert(page_ref(p1) == 0);
  assert(page_ref(p2) == 0);

  assert(page_ref(pde2page(boot_pgdir[0])) == 1);
  free_pages(pde2page(boot_pgdir[0]), 1);
  boot_pgdir[0] = 0;

  printf("check_pgdir() succeeded!\n");
}

void boot_map_segment(pde_t *pgdir, uintptr_t la, size_t size, uintptr_t pa, uint32_t perm) {
  size_t n = ROUNDUP(size + (((uintptr_t)(la)) & 0xFFF), 4096) / 4096;
  pte_t * ptep;
  assert((((uintptr_t)(la)) & 0xFFF) == (((uintptr_t)(pa)) & 0xFFF));
  la = ROUNDDOWN(la, 4096);
  pa = ROUNDDOWN(pa, 4096);
  for (; n > 0; n--, la += 4096, pa += 4096) {
    ptep = get_pte(pgdir, la, 1);
    assert(ptep != ((void *)0));
    *ptep = pa | 0x001 | perm;
  }
}

void check_boot_pgdir(void) {
  pte_t *ptep;
  int i;
  struct Page *p;
  char *str = "ucore: Hello world!!";

  for (i = 0; i < npage; i += 4096) {
    ptep = get_pte(boot_pgdir, KADDR(i), 0);
    assert(ptep != ((void *)0));
    assert(((uintptr_t)(*ptep) & ~0xFFF) == i);
  }

  assert(((uintptr_t)(boot_pgdir[((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)]) & ~0xFFF) == PADDR(boot_pgdir));
  assert(boot_pgdir[0] == 0);

  p = alloc_pages(1);

  assert(page_insert(boot_pgdir, p, 0x100, 0x002) == 0);
  assert(page_ref(p) == 1);

  assert(page_insert(boot_pgdir, p, 0x100 + 4096, 0x002) == 0);
  assert(page_ref(p) == 2);

  strcpy((void *)0x100, str);
  assert(strcmp((void *)0x100, (void *)(0x100 + 4096)) == 0);

  *(char *)(page2kva(p) + 0x100) = '\0';
  assert(strlen((char *)0x100) == 0);

  free_pages(p, 1);
  free_pages(pde2page(boot_pgdir[0]), 1);
  boot_pgdir[0] = 0;

  printf("check_boot_pgdir() succeeded!\n");
}

char *
perm2str(int perm) {
  char str[4];
  str[0] = (perm & 0x004) ? 'u' : '-';
  str[1] = 'r';
  str[2] = (perm & 0x002) ? 'w' : '-';
  str[3] = '\0';
  return str;
}

int get_pgtable_items(size_t left, size_t right, size_t start, uintptr_t *table, size_t *left_store, size_t *right_store) {
  int perm;
  if (start >= right) {
    return 0;
  }
  while (start < right && !(table[start] & 0x001)) {
    start++;
  }
  if (start < right) {
    if (left_store != ((void *)0)) {
      *left_store = start;
    }
    perm = (table[start++] & (0x004 | 0x002 | 0x001));
    while (start < right && (table[start] & (0x004 | 0x002 | 0x001)) == perm) {
      start++;
    }
    if (right_store != ((void *)0)) {
      *right_store = start;
    }
    return perm;
  }
  return 0;
}

uint tmp;
free_area_t free_area;

void print_pgdir(void) {
  size_t left, right = 0, perm;
  size_t l, r;

  printf("-------------------- BEGIN --------------------\n");
  while ((perm = get_pgtable_items(0, 1024, right, vpd, &left, &right)) != 0) {
    printf("PDE(%x) %x-%x %x ", right - left,
           left * (4096 * 1024), right * (4096 * 1024), (right - left) * (4096 * 1024));
    printf("%s\n", perm2str(perm));
    r = left * 1024;
    while ((perm = get_pgtable_items(left * 1024, right * 1024, r, vpt, &l, &r)) != 0) {
      printf("  |-- PTE(%x) %x-%x %x ", r - l,
             l * 4096, r * 4096, (r - l) * 4096);
      printf("%s\n", perm2str(perm));
    }
  }
  printf("--------------------- END ---------------------\n");
}

uint getsp() {
  asm (LEA, 8);
}

int geta() {

}

void kmalloc_init();

void check_and_return() {
  list_entry_t *head = &free_area.free_list;

  boot_pgdir = (uint)(boot_pgdir) + 0xC0000000;

  boot_pgdir[0] = 0;

  load_default_pmm_manager();
  pmm_manager = &default_pmm_manager;

  pages = (uint)(pages) + 0xC0000000;
  while (1) {
    head->prev = (uint)(head->prev) + 0xC0000000;
    head->next = (uint)(head->next) + 0xC0000000;
    if (head->next == &free_area.free_list) break;
    head = list_next(head);
  }

  check_boot_pgdir();
  print_pgdir();

  kmalloc_init();

  *(uint*)(getsp()+8) += 0xC0000000;

}

static char kstack[4096];

void
pmm_init() {
  int *ksp;

  page_enable = 0;

  tlb_clear_enable = 0;

  init_pmm_manager();

  page_init();

  check_alloc_page();

  boot_pgdir = boot_alloc_page();
  memset(boot_pgdir, 0, 4096);
  boot_cr3 = boot_pgdir;

  check_pgdir();

  boot_pgdir[((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)] = (uint)(boot_pgdir) | 0x001 | 0x002;

  boot_map_segment(boot_pgdir, 0xC0000000, 0x38000000, 0, 0x002);
  boot_map_segment(boot_pgdir, memdisk, 1024 * 4096, msiz() - 1024 * 4096, 0x002);

  boot_pgdir[0] = boot_pgdir[((((uintptr_t)(0xC0000000)) >> 22) & 0x3FF)];

  page_enable = 1;

  pdir(boot_cr3);
  spage(1);

  tlb_clear_enable = 1;

  ksp = 0xC0000000 + getsp();
  asm (LL, 4);
  asm (SSP);
  *(uint *)(getsp()+8) += 0xC0000000;
  *(uint *)(getsp()+16) += 0xC0000000;
  ksp = (uint)(check_and_return) + 0xC0000000;
  asm (LL, 4);
  asm (JSRA);
}

void
unmap_range(pde_t *pgdir, uintptr_t start, uintptr_t end) {

  pte_t *ptep;

  assert(start % 4096 == 0 && end % 4096 == 0);
  assert((0x00200000 <= (start) && (start) < (end) && (end) <= 0xB0000000));

  do {
    ptep = get_pte(pgdir, start, 0);
    if (ptep == ((void *)0)) {
      start = ROUNDDOWN(start + (4096 * 1024), (4096 * 1024));
      continue;
    }
    if (*ptep != 0) {
      page_remove_pte(pgdir, start, ptep);
    }
    start += 4096;
  } while (start != 0 && start < end);
}

void
exit_range(pde_t *pgdir, uintptr_t start, uintptr_t end) {

  int pde_idx;

  assert(start % 4096 == 0 && end % 4096 == 0);
  assert((0x00200000 <= (start) && (start) < (end) && (end) <= 0xB0000000));

  start = ROUNDDOWN(start, (4096 * 1024));
  do {
    pde_idx = ((((uintptr_t)(start)) >> 22) & 0x3FF);
    if (pgdir[pde_idx] & 0x001) {
      free_pages(pde2page(pgdir[pde_idx]), 1);
      pgdir[pde_idx] = 0;
    }
    start += (4096 * 1024);
  } while (start != 0 && start < end);
}

int
copy_range(pde_t *to, pde_t *from, uintptr_t start, uintptr_t end, bool share) {
  pte_t *ptep, *nptep;
  uint32_t perm;
  struct Page *page;
  struct Page *npage;
  int ret;
  void * kva_src;
  void * kva_dst;

  assert(start % 4096 == 0 && end % 4096 == 0);
  assert((0x00200000 <= (start) && (start) < (end) && (end) <= 0xB0000000));

  do {

    ptep = get_pte(from, start, 0);
    if (ptep == ((void *)0)) {
      start = ROUNDDOWN(start + (4096 * 1024), (4096 * 1024));
      continue;
    }

    if (*ptep & 0x001) {
      if ((nptep = get_pte(to, start, 1)) == ((void *)0)) {
 return -4;
      }
      perm = (*ptep & (0x004 | 0x002 | 0x001));

      page = pte2page(*ptep);

      npage = alloc_pages(1);
      assert(page!=((void *)0));
      assert(npage!=((void *)0));
      ret=0;

      kva_src = page2kva(page);
      kva_dst = page2kva(npage);

      memcpy(kva_dst, kva_src, 4096);

      ret = page_insert(to, npage, start, perm);
      assert(ret == 0);
    }
    start += 4096;
  } while (start != 0 && start < end);
  return 0;
}

void default_init(void) {
  list_init(&(free_area.free_list));
  (free_area.nr_free) = 0;
}

void default_init_memmap(struct Page *base, size_t n) {
  struct Page *p;
  assert(n > 0);
  p = base;
  for (; p != base + n; p++) {
    assert(test_bit(0, &((p)->flags)));
    p->flags = 0;
    set_bit(1, &((p)->flags));
    p->property = 0;
    set_page_ref(p, 0);
    list_add_before(&(free_area.free_list), &(p->page_link));
  }
  (free_area.nr_free) += n;
  base->property = n;
}

struct Page* default_alloc_pages(size_t n) {
  list_entry_t *le, *len;
  struct Page *p, *pp, *ppp;
  int i;
  assert(n > 0);
  if (n > (free_area.nr_free)) {
    return ((void *)0);
  }
  le = &(free_area.free_list);
  while((le=list_next(le)) != &(free_area.free_list)) {
    p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
    if(p->property >= n) {
      for(i=0; i<n; i++) {
 len = list_next(le);
 pp = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
 set_bit(0, &((pp)->flags));
 clear_bit(1, &((pp)->flags));
 list_del(le);
 le = len;
      }
      if(p->property>n) {
 ppp = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
 ppp->property = p->property - n;
      }
      clear_bit(1, &((p)->flags));
      set_bit(0, &((p)->flags));
      (free_area.nr_free) -= n;
      return p;
    }
  }
  return ((void *)0);
}

void default_free_pages(struct Page *base, size_t n) {
  list_entry_t *le;
  struct Page * p;
  assert(n > 0);
  assert(test_bit(0, &((base)->flags)));

  le = &(free_area.free_list);
  while((le=list_next(le)) != &(free_area.free_list)) {
    p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
    if(p>base) {
      break;
    }
  }

  for(p=base; p<base+n; p++) {
    list_add_before(le, &(p->page_link));
  }
  base->flags = 0;
  set_page_ref(base, 0);
  clear_bit(1, &((base)->flags));
  set_bit(1, &((base)->flags));
  base->property = n;

  p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
  if( base+n == p ) {
    base->property += p->property;
    p->property = 0;
  }
  le = list_prev(&(base->page_link));
  p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
  if(le!=&(free_area.free_list) && p==base-1) {
    while(le!=&(free_area.free_list)) {
      if(p->property) {
 p->property += base->property;
 base->property = 0;
 break;
      }
      le = list_prev(le);
      p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
    }
  }

  (free_area.nr_free) += n;
  return;
}

size_t default_nr_free_pages(void) {
  return (free_area.nr_free);
}

void basic_check() {
  struct Page *p0, *p1, *p2, *p;
  struct list_entry free_list_store;
  uint nr_free_store;

  p0 = p1 = p2 = ((void *)0);
  assert((p0 = alloc_pages(1)) != ((void *)0));
  assert((p1 = alloc_pages(1)) != ((void *)0));
  assert((p2 = alloc_pages(1)) != ((void *)0));

  assert(p0 != p1 && p0 != p2 && p1 != p2);
  assert(page_ref(p0) == 0 && page_ref(p1) == 0 && page_ref(p2) == 0);

  assert(page2pa(p0) < npage * 4096);
  assert(page2pa(p1) < npage * 4096);
  assert(page2pa(p2) < npage * 4096);

  memcpy(&free_list_store, &(free_area.free_list), sizeof(struct list_entry));
  list_init(&(free_area.free_list));
  assert(list_empty(&(free_area.free_list)));

  nr_free_store = (free_area.nr_free);
  (free_area.nr_free) = 0;

  assert(alloc_pages(1) == ((void *)0));

  free_pages(p0, 1);
  free_pages(p1, 1);
  free_pages(p2, 1);
  assert((free_area.nr_free) == 3);

  assert((p0 = alloc_pages(1)) != ((void *)0));
  assert((p1 = alloc_pages(1)) != ((void *)0));
  assert((p2 = alloc_pages(1)) != ((void *)0));

  assert(alloc_pages(1) == ((void *)0));

  free_pages(p0, 1);
  assert(!list_empty(&(free_area.free_list)));

  assert((p = alloc_pages(1)) == p0);
  assert(alloc_pages(1) == ((void *)0));

  assert((free_area.nr_free) == 0);

  memcpy(&(free_area.free_list), &free_list_store, sizeof(struct list_entry));
  (free_area.nr_free) = nr_free_store;

  free_pages(p, 1);
  free_pages(p1, 1);
  free_pages(p2, 1);
}

void
default_check(void) {
  struct Page * p0, *p1, *p2, *p;
  struct list_entry *le = &(free_area.free_list);
  struct list_entry free_list_store;
  uint nr_free_store;
  int count, total;
  count = 0;
  total = 0;
  while ((le = list_next(le)) != &(free_area.free_list)) {
    p = KADDR(((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link)))));
    assert(test_bit(1, &((p)->flags)));
    count++, total += p->property;
  }
  assert(total == nr_free_pages());

  basic_check();

  p0 = alloc_pages(5);
  assert(p0 != ((void *)0));
  assert(!test_bit(1, &((p0)->flags)));

  memcpy(&free_list_store, &(free_area.free_list), sizeof(struct list_entry));
  list_init(&(free_area.free_list));
  assert(list_empty(&(free_area.free_list)));
  assert(alloc_pages(1) == ((void *)0));

  nr_free_store = (free_area.nr_free);
  (free_area.nr_free) = 0;

  free_pages(p0 + 2, 3);
  assert(alloc_pages(4) == ((void *)0));
  assert(test_bit(1, &((p0 + 2)->flags)) && p0[2].property == 3);
  assert((p1 = alloc_pages(3)) != ((void *)0));
  assert(alloc_pages(1) == ((void *)0));
  assert(p0 + 2 == p1);

  p2 = p0 + 1;
  free_pages(p0, 1);
  free_pages(p1, 3);
  assert(test_bit(1, &((p0)->flags)) && p0->property == 1);
  assert(test_bit(1, &((p1)->flags)) && p1->property == 3);

  assert((p0 = alloc_pages(1)) == p2 - 1);
  free_pages(p0, 1);
  assert((p0 = alloc_pages(2)) == p2 + 1);

  free_pages(p0, 2);
  free_pages(p2, 1);

  assert((p0 = alloc_pages(5)) != ((void *)0));
  assert(alloc_pages(1) == ((void *)0));

  assert((free_area.nr_free) == 0);
  (free_area.nr_free) = nr_free_store;

  memcpy(&(free_area.free_list), &free_list_store, sizeof(struct list_entry));
  free_pages(p0, 5);

  le = &(free_area.free_list);
  while ((le = list_next(le)) != &(free_area.free_list)) {
    p = KADDR(((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link)))));
    count--, total -= p->property;
  }
  assert(count == 0);
  assert(total == 0);
}

void load_default_pmm_manager() {
  default_pmm_manager.name = "default_pmm_manager";
  default_pmm_manager.init = default_init;
  default_pmm_manager.init_memmap = default_init_memmap;
  default_pmm_manager.alloc_pages = default_alloc_pages;
  default_pmm_manager.free_pages = default_free_pages;
  default_pmm_manager.nr_free_pages = default_nr_free_pages;
  default_pmm_manager.check = default_check;
}

enum {
  FMEM,
  FTIMER,
  FKEYBD,
  FPRIV,
  FINST,
  FSYS,
  FARITH,
  FIPAGE,
  FWPAGE,
  FRPAGE,
  USER=16
};

uint ticks;

struct pushregs {
  int sp, pad1;
  double g;
  double f;
  int c, pad2;
  int b, pad3;
  int a, pad4;
};

struct trapframe {
  struct pushregs tf_regs;
  int fc, pad5;
  int pc, pad6;
};

void print_regs(struct pushregs *regs) {
  printf("  SP : 0x%x\n", regs->sp);

  printf("  REG_A : 0x%x\n", regs->a);
  printf("  REG_B : 0x%x\n", regs->b);
  printf("  REG_C : 0x%x\n", regs->c);
}

void print_trapframe(struct trapframe *tf) {

  printf("PC : 0x%x\n", tf->pc);
  printf("\n");
}

bool
trap_in_kernel(struct trapframe *tf) {
  return (tf->fc < USER);
}

uint32_t
hash32(uint32_t val, unsigned int bits) {
  uint32_t hash = val * 0x9e370001UL;
  return (hash >> (32 - bits));
}

unsigned int next = 1;

int
rand(void) {
  unsigned int result, p;
  next = (next * 1592583800 + 11) & ((1 << 32) - 1);
  p = 2147483647 + 1;
  result = next % p;
  return result;
}

void
srand(unsigned int seed) {
  next = seed;
}

struct skew_heap_entry {
  struct skew_heap_entry *parent, *left, *right;
};

typedef struct skew_heap_entry skew_heap_entry_t;
typedef void* compare_f;

void
skew_heap_init(skew_heap_entry_t *a)
{
  a->left = a->right = a->parent = ((void *)0);
}

skew_heap_entry_t *
skew_heap_merge(skew_heap_entry_t *a, skew_heap_entry_t *b,
                compare_f comp)
{
  skew_heap_entry_t *l, *r;
  int cmp;

  if (a == ((void *)0)) return b;
  else if (b == ((void *)0)) return a;

  cmp = call2(a, b, comp);

  if (cmp == -1)
  {
    r = a->left;
    l = skew_heap_merge(a->right, b, comp);

    a->left = l;
    a->right = r;
    if (l) l->parent = a;

    return a;
  }
  else
  {
    r = b->left;
    l = skew_heap_merge(a, b->right, comp);

    b->left = l;
    b->right = r;
    if (l) l->parent = b;

    return b;
  }
}

skew_heap_entry_t *
skew_heap_insert(skew_heap_entry_t *a, skew_heap_entry_t *b,
                 compare_f comp)
{
  skew_heap_init(b);
  return skew_heap_merge(a, b, comp);
}

skew_heap_entry_t *
skew_heap_remove(skew_heap_entry_t *a, skew_heap_entry_t *b,
                 compare_f comp)
{
  skew_heap_entry_t *p = b->parent;
  skew_heap_entry_t *rep = skew_heap_merge(b->left, b->right, comp);
  if (rep) rep->parent = p;

  if (p)
  {
    if (p->left == b)
      p->left = rep;
    else p->right = rep;
    return a;
  }
  else return rep;
}

enum proc_state {
  PROC_UNINIT = 0,
  PROC_SLEEPING,
  PROC_RUNNABLE,
  PROC_ZOMBIE,
};

struct context {
  uint32_t pc;
  uint32_t sp;
  uint32_t b;
  uint32_t c;
};

struct proc_struct {
  enum proc_state state;
  int pid;
  int runs;
  uintptr_t kstack;
  bool need_resched;
  struct proc_struct *parent;
  struct mm_struct *mm;
  struct context context;
  struct trapframe *tf;
  uintptr_t cr3;
  uint32_t flags;
  char name[15 + 1];
  list_entry_t list_link;
  list_entry_t hash_link;
  int exit_code;
  uint32_t wait_state;
  struct proc_struct *cptr, *yptr, *optr;
  struct run_queue *rq;
  list_entry_t run_link;
  int time_slice;
  skew_heap_entry_t lab6_run_pool;
  uint32_t lab6_stride;
  uint32_t lab6_priority;
};

void proc_init(void);
void proc_run(struct proc_struct *proc);
int kernel_thread(funcptr_t fn, void *arg, uint32_t clone_flags);

char *set_proc_name(struct proc_struct *proc, char *name);
char *get_proc_name(struct proc_struct *proc);

int do_fork(uint32_t clone_flags, uintptr_t stack, struct trapframe *tf);
int do_exit(int error_code);

list_entry_t proc_list;

static list_entry_t hash_list[(1 << 10)];

struct proc_struct *idleproc = ((void *)0);

struct proc_struct *initproc = ((void *)0);

struct proc_struct *current = ((void *)0);

static int nr_process = 0;

struct sched_class {

  char* name;

  void* init;

  void* enqueue;

  void* dequeue;

  void* pick_next;

  void* proc_tick;

};

struct run_queue {
  list_entry_t run_list;
  unsigned int proc_num;
  int max_time_slice;

  skew_heap_entry_t* lab6_run_pool;
};

int
proc_stride_comp_f(void *a, void *b)
{
  struct proc_struct *p = ((struct proc_struct *)((char *)((a)) - ((size_t)(&((struct proc_struct *)0)->lab6_run_pool))));
  struct proc_struct *q = ((struct proc_struct *)((char *)((b)) - ((size_t)(&((struct proc_struct *)0)->lab6_run_pool))));
  int32_t c = p->lab6_stride - q->lab6_stride;
  if (c > 0) return 1;
  else if (c == 0) return 0;
  else return -1;
}

void
stride_init(struct run_queue *rq) {

  list_init(&(rq->run_list));
  rq->lab6_run_pool = ((void *)0);
  rq->proc_num = 0;
}

void
stride_enqueue(struct run_queue *rq, struct proc_struct *proc) {

  rq->lab6_run_pool =
    skew_heap_insert(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f);
  if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice) {
    proc->time_slice = rq->max_time_slice;
  }
  proc->rq = rq;
  rq->proc_num++;
}

void
stride_dequeue(struct run_queue *rq, struct proc_struct *proc) {

  rq->lab6_run_pool =
    skew_heap_remove(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f);
  rq->proc_num--;
}

struct proc_struct *
stride_pick_next(struct run_queue *rq) {

  struct proc_struct *p;
  if (rq->lab6_run_pool == ((void *)0)) return ((void *)0);
  p = ((struct proc_struct *)((char *)((rq->lab6_run_pool)) - ((size_t)(&((struct proc_struct *)0)->lab6_run_pool))));
  if (p->lab6_priority == 0)
    p->lab6_stride += 0x7FFFFFFF;
  else p->lab6_stride += 0x7FFFFFFF / p->lab6_priority;
  return p;
}

void
stride_proc_tick(struct run_queue *rq, struct proc_struct *proc) {

  if (proc->time_slice > 0)
    proc->time_slice--;
  if (proc->time_slice == 0)
    proc->need_resched = 1;
}

struct sched_class default_sched_class;

void load_default_sched_class() {
  default_sched_class.name = "stride_scheduler";
  default_sched_class.init = stride_init;
  default_sched_class.enqueue = stride_enqueue;
  default_sched_class.dequeue = stride_dequeue;
  default_sched_class.pick_next = stride_pick_next;
  default_sched_class.proc_tick = stride_proc_tick;
}

list_entry_t timer_list;

struct sched_class *sched_class;

struct run_queue *rq;

void sched_class_enqueue(struct proc_struct *proc) {
  if (proc != idleproc) {
    call2(rq, proc,sched_class->enqueue);
  }
}

void sched_class_dequeue(struct proc_struct *proc) {
  call2(rq, proc, sched_class->dequeue);
}

struct proc_struct* sched_class_pick_next(void) {
  return call1(rq, sched_class->pick_next);
}

void sched_class_proc_tick(struct proc_struct *proc) {
  if (proc != idleproc) {
    call2(rq, proc, sched_class->proc_tick);
  }
  else {
    proc->need_resched = 1;
  }
}

struct run_queue __rq;

void sched_init(void) {
  list_init(&timer_list);

  load_default_sched_class();
  sched_class = &default_sched_class;

  rq = &__rq;
  rq->max_time_slice = 5;
  call1(rq, sched_class->init);

  printf("sched class: %s\n", sched_class->name);
}

void wakeup_proc(struct proc_struct *proc) {
  bool intr_flag;
  assert(proc->state != PROC_ZOMBIE);
  do { intr_flag = __intr_save(); } while (0);
  {
    if (proc->state != PROC_RUNNABLE) {
      proc->state = PROC_RUNNABLE;
      proc->wait_state = 0;
      if (proc != current) {
 sched_class_enqueue(proc);
      }
    }
    else {
      printf("wakeup runnable process.\n");
    }
  }
  __intr_restore(intr_flag);;
}

void schedule(void) {
  bool intr_flag;
  struct proc_struct *next;
  do { intr_flag = __intr_save(); } while (0);
  {
    current->need_resched = 0;
    if (current->state == PROC_RUNNABLE) {
      sched_class_enqueue(current);
    }
    if ((next = sched_class_pick_next()) != ((void *)0)) {
      sched_class_dequeue(next);
    }
    if (next == ((void *)0)) {
      next = idleproc;
    }
    next->runs++;
    if (next != current) {
      proc_run(next);
    }
  }
  __intr_restore(intr_flag);;
}

typedef unsigned int gfp_t;

struct slob_block {
  int units;
  struct slob_block *next;
};
typedef struct slob_block slob_t;

struct bigblock {
  int order;
  void *pages;
  struct bigblock *next;
};
typedef struct bigblock bigblock_t;

static slob_t arena;
static slob_t *slobfree;
static bigblock_t *bigblocks;

void init_kmalloc(void) {
  arena.next = &arena;
  arena.units = 1;
  slobfree = &arena;
}

void* __slob_get_free_pages(gfp_t gfp, int order)
{
  struct Page * page = alloc_pages(1 << order);
  if(!page)
    return ((void *)0);
  return page2kva(page);
}

void __slob_free_pages(unsigned long kva, int order)
{
  free_pages(kva2page(kva), 1 << order);
}

void slob_free(void *block, int size)
{
  slob_t *cur, *b = (slob_t *)block;
  unsigned long flags;

  if (!block)
    return;

  if (size)
    b->units = (((size) + sizeof(slob_t) - 1)/sizeof(slob_t));

  do { flags = __intr_save(); } while (0);
  for (cur = slobfree; !(b > cur && b < cur->next); cur = cur->next)
    if (cur >= cur->next && (b > cur || b < cur->next))
      break;

  if (b + b->units == cur->next) {
    b->units += cur->next->units;
    b->next = cur->next->next;
  } else
    b->next = cur->next;

  if (cur + cur->units == b) {
    cur->units += b->units;
    cur->next = b->next;
  } else
    cur->next = b;

  slobfree = cur;

  __intr_restore(flags);;
}

void* slob_alloc(size_t size, gfp_t gfp, int align)
{

  slob_t *prev, *cur, *aligned = 0;
  int delta = 0, units = (((size) + sizeof(slob_t) - 1)/sizeof(slob_t));
  unsigned long flags;

  assert( (size + sizeof(slob_t)) < 4096 );

  do { flags = __intr_save(); } while (0);
  prev = slobfree;
  for (cur = prev->next;; prev = cur, cur = cur->next) {
    if (align) {
      aligned = (slob_t *)((((unsigned long)cur)+(align)-1)&(~((align)-1)));
      delta = aligned - cur;
    }
    if (cur->units >= units + delta) {
      if (delta) {
 aligned->units = cur->units - delta;
 aligned->next = cur->next;
 cur->next = aligned;
 cur->units = delta;
 prev = cur;
 cur = aligned;
      }

      if (cur->units == units)
 prev->next = cur->next;
      else {
 prev->next = cur + units;
 prev->next->units = cur->units - units;
 prev->next->next = cur->next;
 cur->units = units;
      }

      slobfree = prev;
      __intr_restore(flags);;
      return cur;
    }
    if (cur == slobfree) {
      __intr_restore(flags);;

      if (size == 4096)
 return 0;

      cur = (slob_t *)__slob_get_free_pages(gfp, 0);
      if (!cur)
 return 0;

      slob_free(cur, 4096);
      do { flags = __intr_save(); } while (0);
      cur = slobfree;
    }
  }
}

void check_slab(void) {
  printf("check_slab() success\n");
}

void
slab_init(void) {
  printf("use SLOB allocator\n");
  check_slab();
}

void
kmalloc_init(void) {
  init_kmalloc();
  slab_init();
  printf("kmalloc_init() succeeded!\n");
}

size_t
slab_allocated(void) {
  return 0;
}

size_t
kallocated(void) {
  return slab_allocated();
}

int find_order(int size)
{
  int order = 0;
  for (; size > 4096; size >>=1)
    order++;
  return order;
}

void *__kmalloc(size_t size, gfp_t gfp)
{
  slob_t *m;
  bigblock_t *bb;
  unsigned long flags;

  if (size < 4096 - sizeof(slob_t)) {
    m = slob_alloc(size + sizeof(slob_t), gfp, 0);
    return m ? (void *)(m + 1) : 0;
  }

  bb = slob_alloc(sizeof(bigblock_t), gfp, 0);
  if (!bb)
    return 0;

  bb->order = find_order(size);
  bb->pages = (void *)__slob_get_free_pages(gfp, bb->order);

  if (bb->pages) {
    do { flags = __intr_save(); } while (0);
    bb->next = bigblocks;
    bigblocks = bb;
    __intr_restore(flags);;
    return bb->pages;
  }

  slob_free(bb, sizeof(bigblock_t));
  return 0;
}

void*
kmalloc(size_t size)
{
  return __kmalloc(size, 0);
}

void kfree(void *block)
{
  bigblock_t *bb, **last = &bigblocks;
  unsigned long flags;

  if (!block)
    return;

  if (!((unsigned long)block & (4096 -1))) {

    do { flags = __intr_save(); } while (0);
    for (bb = bigblocks; bb; last = &bb->next, bb = bb->next) {
      if (bb->pages == block) {
 *last = bb->next;
 __intr_restore(flags);;
 __slob_free_pages((unsigned long)block, bb->order);
 slob_free(bb, sizeof(bigblock_t));
 return;
      }
    }
    __intr_restore(flags);;
  }

  slob_free((slob_t *)block - 1, 0);
  return;
}

unsigned int ksize(void *block)
{
  bigblock_t *bb;
  unsigned long flags;

  if (!block)
    return 0;

  if (!((unsigned long)block & (4096 -1))) {
    do { flags = __intr_save(); } while (0);
    for (bb = bigblocks; bb; bb = bb->next)
      if (bb->pages == block) {
 __intr_restore(flags);;
 return 4096 << bb->order;
      }
    __intr_restore(flags);;
  }

  return ((slob_t *)block - 1)->units * sizeof(slob_t);
}

int
mm_count(struct mm_struct *mm) {
  return mm->mm_count;
}

void
set_mm_count(struct mm_struct *mm, int val) {
  mm->mm_count = val;
}

int
mm_count_inc(struct mm_struct *mm) {
  mm->mm_count += 1;
  return mm->mm_count;
}

int
mm_count_dec(struct mm_struct *mm) {
  mm->mm_count -= 1;
  return mm->mm_count;
}

void
lock_mm(struct mm_struct *mm) {
  if (mm != ((void *)0)) {
    lock(&(mm->mm_lock));
  }
}

void
unlock_mm(struct mm_struct *mm) {
  if (mm != ((void *)0)) {
    unlock(&(mm->mm_lock));
  }
}

int swap_init_mm(struct mm_struct *mm);

struct mm_struct *
mm_create(void) {
  struct mm_struct *mm;
  mm = kmalloc(sizeof(struct mm_struct));
  if (mm != ((void *)0)) {
    list_init(&(mm->mmap_list));
    mm->mmap_cache = ((void *)0);
    mm->pgdir = ((void *)0);
    mm->map_count = 0;
    if (swap_init_ok)
      swap_init_mm(mm);
    else
      mm->sm_priv = ((void *)0);

    set_mm_count(mm, 0);
    lock_init(&(mm->mm_lock));
  }
  return mm;
}

struct vma_struct *
vma_create(uintptr_t vm_start, uintptr_t vm_end, uint32_t vm_flags) {
  struct vma_struct *vma;
  vma = kmalloc(sizeof(struct vma_struct));
  if (vma != ((void *)0)) {
    vma->vm_start = vm_start;
    vma->vm_end = vm_end;
    vma->vm_flags = vm_flags;
  }
  return vma;
}

struct vma_struct *
find_vma(struct mm_struct *mm, uintptr_t addr) {
  bool found;
  struct vma_struct *vma = ((void *)0);
  list_entry_t *list, *le;
  if (mm != ((void *)0)) {
    vma = mm->mmap_cache;
    if (!(vma != ((void *)0) && vma->vm_start <= addr && vma->vm_end > addr)) {
      found = 0;
      list = &(mm->mmap_list);
      le = list;
      while ((le = list_next(le)) != list) {
 vma = ((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link))));
 if (vma->vm_start<=addr && addr < vma->vm_end) {
   found = 1;
   break;
 }
      }
      if (!found) {
 vma = ((void *)0);
      }
    }
    if (vma != ((void *)0)) {
      mm->mmap_cache = vma;
    }
  }
  return vma;
}

void check_vma_overlap(struct vma_struct *prev, struct vma_struct *next) {
  assert(prev->vm_start < prev->vm_end);
  assert(prev->vm_end <= next->vm_start);
  assert(next->vm_start < next->vm_end);
}

void
insert_vma_struct(struct mm_struct *mm, struct vma_struct *vma) {
  list_entry_t *le;
  list_entry_t *list;
  list_entry_t *le_prev, *le_next;
  struct vma_struct *mmap_prev;
  assert(vma->vm_start < vma->vm_end);
  list = &(mm->mmap_list);
  le_prev = list;

  le = list;
  while ((le = list_next(le)) != list) {
    mmap_prev = ((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link))));
    if (mmap_prev->vm_start > vma->vm_start) {
      break;
    }
    le_prev = le;
  }

  le_next = list_next(le_prev);

  if (le_prev != list) {
    check_vma_overlap(((struct vma_struct *)((char *)((le_prev)) - ((size_t)(&((struct vma_struct *)0)->list_link)))), vma);
  }
  if (le_next != list) {
    check_vma_overlap(vma, ((struct vma_struct *)((char *)((le_next)) - ((size_t)(&((struct vma_struct *)0)->list_link)))));
  }

  vma->vm_mm = mm;
  list_add_after(le_prev, &(vma->list_link));

  mm->map_count++;
}

void
mm_destroy(struct mm_struct *mm) {
  list_entry_t *list = &(mm->mmap_list), *le;
  assert(mm_count(mm) == 0);
  while ((le = list_next(list)) != list) {
    list_del(le);
    kfree(((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link)))));
  }
  kfree(mm);
  mm=((void *)0);
}

void
check_vma_struct(void) {
  size_t nr_free_pages_store = nr_free_pages();
  int step1 = 10;
  int step2 = step1 * 10;
  int i;
  struct mm_struct *mm = mm_create();
  struct vma_struct *vma;
  struct vma_struct *mmap;
  struct vma_struct *vma1, *vma2, *vma3, *vma4, *vma5;
  struct vma_struct *vma_below_5;
  list_entry_t *le;

  assert(mm != ((void *)0));

  for (i = step1; i >= 1; i--) {
    vma = vma_create(i * 5, i * 5 + 2, 0);
    assert(vma != ((void *)0));
    insert_vma_struct(mm, vma);
  }

  for (i = step1 + 1; i <= step2; i++) {
    vma = vma_create(i * 5, i * 5 + 2, 0);
    assert(vma != ((void *)0));
    insert_vma_struct(mm, vma);
  }

  le = list_next(&(mm->mmap_list));

  for (i = 1; i <= step2; i++) {
    assert(le != &(mm->mmap_list));
    mmap = ((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link))));
    assert(mmap->vm_start == i * 5 && mmap->vm_end == i * 5 + 2);
    le = list_next(le);
  }

  for (i = 5; i <= 5 * step2; i +=5) {
    vma1 = find_vma(mm, i);
    assert(vma1 != ((void *)0));
    vma2 = find_vma(mm, i+1);
    assert(vma2 != ((void *)0));
    vma3 = find_vma(mm, i+2);
    assert(vma3 == ((void *)0));
    vma4 = find_vma(mm, i+3);
    assert(vma4 == ((void *)0));
    vma5 = find_vma(mm, i+4);
    assert(vma5 == ((void *)0));

    assert(vma1->vm_start == i && vma1->vm_end == i + 2);
    assert(vma2->vm_start == i && vma2->vm_end == i + 2);
  }

  for (i =4; i>=0; i--) {
    vma_below_5 = find_vma(mm,i);
    if (vma_below_5 != ((void *)0) ) {
      printf("vma_below_5: i %x, start %x, end %x\n",i, vma_below_5->vm_start, vma_below_5->vm_end);
    }
    assert(vma_below_5 == ((void *)0));
  }

  mm_destroy(mm);

  printf("check_vma_struct() succeeded!\n");
}

void check_pgfault(void) {
  size_t nr_free_pages_store = nr_free_pages();
  struct mm_struct *mm;
  struct vma_struct *vma;
  pde_t *pgdir;
  uintptr_t addr;
  int i, sum;

  check_mm_struct = mm_create();
  assert(check_mm_struct != ((void *)0));

  mm = check_mm_struct;
  pgdir = mm->pgdir = boot_pgdir;
  assert(pgdir[0] == 0);

  vma = vma_create(0, (4096 * 1024), 0x00000002);
  assert(vma != ((void *)0));

  insert_vma_struct(mm, vma);

  addr = 0x100;

  assert(find_vma(mm, addr) == vma);

  sum = 0;

  for (i = 0; i < 100; i++) {
    *(char *)(addr + i) = i;
    sum += i;
  }

  for (i = 0; i < 100; i++) {
    sum -= *(char *)(addr + i);
  }

  assert(sum == 0);

  page_remove(pgdir, ROUNDDOWN(addr, 4096));

  free_pages(pde2page(pgdir[0]), 1);

  pgdir[0] = 0;

  mm->pgdir = ((void *)0);
  mm_destroy(mm);
  check_mm_struct = ((void *)0);

  assert(nr_free_pages_store == nr_free_pages());

  printf("check_pgfault() succeeded!\n");
}

void check_vmm(void) {
  size_t nr_free_pages_store = nr_free_pages();

  check_vma_struct();
  check_pgfault();

  printf("check_vmm() succeeded.\n");
}

int
mm_map(struct mm_struct *mm, uintptr_t addr, size_t len, uint32_t vm_flags,
       struct vma_struct **vma_store) {

  uintptr_t start = ROUNDDOWN(addr, 4096), end = ROUNDUP(addr + len, 4096);
  int ret;
  struct vma_struct *vma;

  if (!(0x00200000 <= (start) && (start) < (end) && (end) <= 0xB0000000)) {
    return -3;
  }

  assert(mm != ((void *)0));

  ret = -3;

  if ((vma = find_vma(mm, start)) != ((void *)0) && end > vma->vm_start) {
    return ret;
  }
  ret = -4;

  if ((vma = vma_create(start, end, vm_flags)) == ((void *)0)) {
    return ret;
  }
  insert_vma_struct(mm, vma);
  if (vma_store != ((void *)0)) {
    *vma_store = vma;
  }
  ret = 0;

  return ret;
}

int
dup_mmap(struct mm_struct *to, struct mm_struct *from) {
  list_entry_t *list = &(from->mmap_list), *le = list;
  struct vma_struct *vma, *nvma;
  bool share;

  assert(to != ((void *)0) && from != ((void *)0));

  while ((le = list_prev(le)) != list) {

    vma = ((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link))));
    nvma = vma_create(vma->vm_start, vma->vm_end, vma->vm_flags);
    if (nvma == ((void *)0)) {
      return -4;
    }

    insert_vma_struct(to, nvma);
    share = 0;
    if (copy_range(to->pgdir, from->pgdir, vma->vm_start, vma->vm_end, share) != 0) {
      return -4;
    }
  }
  return 0;
}

void
exit_mmap(struct mm_struct *mm) {
  pde_t *pgdir;
  list_entry_t *list, *le;
  struct vma_struct *vma;

  assert(mm != ((void *)0) && mm_count(mm) == 0);

  pgdir = mm->pgdir;
  list = &(mm->mmap_list);
  le = list;
  while ((le = list_next(le)) != list) {
    vma = ((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link))));
    unmap_range(pgdir, vma->vm_start, vma->vm_end);
  }
  while ((le = list_next(le)) != list) {
    vma = ((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link))));
    exit_range(pgdir, vma->vm_start, vma->vm_end);
  }
}

bool
user_mem_check(struct mm_struct *mm, uintptr_t addr, size_t len, bool write) {
  struct vma_struct * vma;
  uintptr_t start, end;

  if (mm != ((void *)0)) {
    if (!(0x00200000 <= (addr) && (addr) < (addr + len) && (addr + len) <= 0xB0000000)) {
      return 0;
    }

    start = addr;
    end = addr + len;

    while (start < end) {
      if ((vma = find_vma(mm, start)) == ((void *)0) || start < vma->vm_start) {
 return 0;
      }
      if (!(vma->vm_flags & ((write) ? 0x00000002 : 0x00000001))) {
 return 0;
      }
      if (write && (vma->vm_flags & 0x00000008)) {
 if (start < vma->vm_start + 4096) {
   return 0;
 }
      }
      start = vma->vm_end;
    }
    return 1;
  }
  return (0xC0000000 <= (addr) && (addr) < (addr + len) && (addr + len) <= (0xC0000000 + 0x38000000));
}

bool
copy_from_user(struct mm_struct *mm, void *dst, void *src, size_t len, bool writable) {
  if (!user_mem_check(mm, (uintptr_t)src, len, writable)) {
    return 0;
  }
  memcpy(dst, src, len);
  return 1;
}

bool
copy_to_user(struct mm_struct *mm, void *dst, void *src, size_t len) {
  if (!user_mem_check(mm, (uintptr_t)dst, len, 1)) {
    return 0;
  }
  memcpy(dst, src, len);
  return 1;
}

void
vmm_init(void) {
  check_vmm();
}

unsigned int pgfault_num=0;

int swap_in(struct mm_struct *mm, uintptr_t addr, struct Page **ptr_result);

int
do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr) {
  int ret = -3;

  struct vma_struct *vma = find_vma(mm, addr);
  struct Page * page;
  pte_t *ptep;
  uint32_t perm;

  pgfault_num++;

  if (vma == ((void *)0) || vma->vm_start > addr) {
    printf("not valid addr %x, and  can not find it in vma\n", addr);

    return ret;
  }

  perm = 0x004;
  if (vma->vm_flags & 0x00000002) {
    perm |= 0x002;
  }
  addr = ROUNDDOWN(addr, 4096);

  ret = -4;

  ptep = ((void *)0);

  if ((ptep = get_pte(mm->pgdir, addr, 1)) == ((void *)0)) {
    printf("get_pte in do_pgfault failed\n");
    return ret;
  }

  if (*ptep == 0) {
    if (pgdir_alloc_page(mm->pgdir, addr, perm) == ((void *)0)) {
      printf("pgdir_alloc_page in do_pgfault failed\n");
      return ret;
    }
  }
  else {

    page = ((void *)0);
    printf("do pgfault: ptep %x, pte %x\n",ptep, *ptep);
    if (*ptep & 0x001) {

      panic("error write a non-writable pte");

    } else{

      if(swap_init_ok) {
 if ((ret = swap_in(mm, addr, &page)) != 0) {
   printf("swap_in in do_pgfault failed\n");
   return ret;
 }
      }
      else {
 printf("no swap_init_ok but ptep is %x, failed\n",*ptep);
 return ret;
      }
    }
    page_insert(mm->pgdir, page, addr, perm);
    swap_map_swappable(mm, addr, page, 1);
    page->pra_vaddr = addr;
  }
  ret = 0;
  return ret;
}

int syscall();

static struct proc_struct *
alloc_proc(void) {
  struct proc_struct *proc = kmalloc(sizeof(struct proc_struct));
  if (proc != ((void *)0)) {

    proc->state = PROC_UNINIT;
    proc->pid = -1;
    proc->runs = 0;
    proc->kstack = 0;
    proc->need_resched = 0;
    proc->parent = ((void *)0);
    proc->mm = ((void *)0);
    memset(&(proc->context), 0, sizeof(struct context));
    proc->tf = ((void *)0);
    proc->cr3 = boot_cr3;
    proc->flags = 0;
    memset(proc->name, 0, 15);
    proc->wait_state = 0;
    proc->cptr = proc->optr = proc->yptr = ((void *)0);
    proc->rq = ((void *)0);
    list_init(&(proc->run_link));
    proc->time_slice = 0;
    proc->lab6_run_pool.left = proc->lab6_run_pool.right = proc->lab6_run_pool.parent = ((void *)0);
    proc->lab6_stride = 0;
    proc->lab6_priority = 0;
  }
  return proc;
}

char *
set_proc_name(struct proc_struct *proc, char *name) {
  memset(proc->name, 0, sizeof(proc->name));
  return memcpy(proc->name, name, 15);
}

char *
get_proc_name(struct proc_struct *proc) {
  static char name[15 + 1];
  memset(name, 0, sizeof(name));
  return memcpy(name, proc->name, 15);
}

uint getb() {
  asm (PSHB);
  asm (POPA);
}

uint getc() {
  asm (PSHC);
  asm (POPA);
}

void setb(int con) {
  asm (LL, 8);
  asm (LBA);
}

void setc(int con) {
  asm (LL, 8);
  asm (LCA);
}

void seta(int con) {
  asm (LL, 8);
}

void kernel_thread_entry() {
  asm (PSHC);
  asm (PSHB);
  asm (LEV);
  asm (PSHA);
  do_exit();
}

void
forkret(void) {
  asm (POPA); asm (SUSP);
  asm (POPG);
  asm (POPF);
  asm (POPC);
  asm (POPB);
  asm (POPA);
  asm (RTI);
}

void switch_to(struct context *oldc, struct context *newc)
{
  oldc->pc = *(uint*)(getsp());
  oldc->sp = getsp() + 8;
  oldc->b = getb();
  oldc->c = getc();

  setb(newc->b);
  setc(newc->c);
  *(uint*)((uint)(newc->sp) - 8) = newc->pc;
  seta((uint)(newc->sp) - 8);
  asm (SSP);
  asm (LEV);
}

void proc_run(struct proc_struct *proc) {
  int intr_flag;
  struct proc_struct *prev, *next;
  if (proc != current) {
    prev = current;
    next = proc;
    do { intr_flag = __intr_save(); } while (0);
    {
      current = proc;
      pdir(next->cr3);
      spage(1);
      switch_to(&(prev->context), &(next->context));
    }
    __intr_restore(intr_flag);;
  }
}

static void
set_links(struct proc_struct *proc) {
  list_add(&proc_list, &(proc->list_link));
  proc->yptr = ((void *)0);
  if ((proc->optr = proc->parent->cptr) != ((void *)0)) {
    proc->optr->yptr = proc;
  }
  proc->parent->cptr = proc;
  nr_process++;
}

static void
remove_links(struct proc_struct *proc) {
  list_del(&(proc->list_link));
  if (proc->optr != ((void *)0)) {
    proc->optr->yptr = proc->yptr;
  }
  if (proc->yptr != ((void *)0)) {
    proc->yptr->optr = proc->optr;
  }
  else {
    proc->parent->cptr = proc->optr;
  }
  nr_process--;
}

static int
get_pid(void) {
  struct proc_struct *proc;
  list_entry_t *list = &proc_list, *le;
  static int next_safe = (4096 * 2), last_pid = (4096 * 2);
  if (++last_pid >= (4096 * 2)) {
    last_pid = 1;
    goto inside;
  }
  if (last_pid >= next_safe) {
inside:
    next_safe = (4096 * 2);
repeat:
    le = list;
    while ((le = list_next(le)) != list) {
      proc = ((struct proc_struct *)((char *)((le)) - ((size_t)(&((struct proc_struct *)0)->list_link))));
      if (proc->pid == last_pid) {
 if (++last_pid >= next_safe) {
   if (last_pid >= (4096 * 2)) {
     last_pid = 1;
   }
   next_safe = (4096 * 2);
   goto repeat;
 }
      }
      else if (proc->pid > last_pid && next_safe > proc->pid) {
 next_safe = proc->pid;
      }
    }
  }
  return last_pid;
}

static void
hash_proc(struct proc_struct *proc) {
  list_add(hash_list + (hash32(proc->pid, 10)), &(proc->hash_link));
}

static void
unhash_proc(struct proc_struct *proc) {
  list_del(&(proc->hash_link));
}

struct proc_struct *
find_proc(int pid) {
  list_entry_t *list, *le;
  struct proc_struct *proc;
  if (0 < pid && pid < (4096 * 2)) {
    list = hash_list + (hash32(pid, 10));
    le = list;
    while ((le = list_next(le)) != list) {
      proc = ((struct proc_struct *)((char *)((le)) - ((size_t)(&((struct proc_struct *)0)->hash_link))));
      if (proc->pid == pid) {
 return proc;
      }
    }
  }
  return ((void *)0);
}

int kernel_thread(funcptr_t fn, void *arg, uint32_t clone_flags) {
  struct trapframe tf;
  memset(&tf, 0, sizeof(struct trapframe));
  tf.tf_regs.b = fn;
  tf.tf_regs.c = arg;
  tf.pc = kernel_thread_entry;
  return do_fork(clone_flags | 0x00000100, 0, &tf);
}

static int
setup_kstack(struct proc_struct *proc) {
  struct Page *page = alloc_pages(2);
  if (page != ((void *)0)) {
    proc->kstack = (uintptr_t)page2kva(page);
    return 0;
  }
  return -4;
}

static void
put_kstack(struct proc_struct *proc) {
  free_pages(kva2page((void *)(proc->kstack)), 2);
}

static int
setup_pgdir(struct mm_struct *mm) {
  struct Page *page;
  pde_t *pgdir;
  if ((page = alloc_pages(1)) == ((void *)0)) {
    return -4;
  }
  pgdir = page2kva(page);
  memcpy(pgdir, boot_pgdir, 4096);
  pgdir[((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)] = PADDR(pgdir) | 0x001 | 0x002;
  mm->pgdir = pgdir;
  return 0;
}

static void
put_pgdir(struct mm_struct *mm) {
  free_pages(kva2page(mm->pgdir), 1);
}

static int
copy_mm(uint32_t clone_flags, struct proc_struct *proc) {
  struct mm_struct *mm, *oldmm = current->mm;
  int ret;

  if (oldmm == ((void *)0)) {
    return 0;
  }
  if (clone_flags & 0x00000100) {
    mm = oldmm;
    goto good_mm;
  }

  ret = -4;
  if ((mm = mm_create()) == ((void *)0)) {
    goto bad_mm;
  }
  if (setup_pgdir(mm) != 0) {
    goto bad_pgdir_cleanup_mm;
  }

  lock_mm(oldmm);
  {
    ret = dup_mmap(mm, oldmm);
  }
  unlock_mm(oldmm);

  if (ret != 0) {
    goto bad_dup_cleanup_mmap;
  }

good_mm:
  mm_count_inc(mm);
  proc->mm = mm;
  proc->cr3 = PADDR(mm->pgdir);
  return 0;
bad_dup_cleanup_mmap:
  exit_mmap(mm);
  put_pgdir(mm);
bad_pgdir_cleanup_mm:
  mm_destroy(mm);
bad_mm:
  return ret;
}

static void
copy_thread(struct proc_struct *proc, uintptr_t esp, struct trapframe *tf) {

  proc->tf = (struct trapframe *)(proc->kstack + (2 * 4096)) - 1;
  memcpy(proc->tf, tf, sizeof(struct trapframe));
  proc->tf->tf_regs.a = 0;
  proc->tf->tf_regs.sp = esp;
  proc->tf->fc = (proc->tf->fc);

  proc->context.pc = (uintptr_t)(forkret);
  proc->context.sp = (uintptr_t)(proc->tf);
}

int
do_fork(uint32_t clone_flags, uintptr_t stack, struct trapframe *tf) {
  int ret = -5;
  int intr_flag;

  struct proc_struct *proc;
  if (nr_process >= 4096) {
    goto fork_out;
  }
  ret = -4;

  if ((proc = alloc_proc()) == ((void *)0)) {
    goto fork_out;
  }

  proc->parent = current;
  assert(current->wait_state == 0);

  if (setup_kstack(proc) != 0) {
    goto bad_fork_cleanup_proc;
  }
  if (copy_mm(clone_flags, proc) != 0) {
    goto bad_fork_cleanup_kstack;
  }
  copy_thread(proc, stack, tf);

  do { intr_flag = __intr_save(); } while (0);
  {
    proc->pid = get_pid();
    hash_proc(proc);
    set_links(proc);

  }
  __intr_restore(intr_flag);;

  wakeup_proc(proc);

  ret = proc->pid;

fork_out:
  return ret;

bad_fork_cleanup_kstack:
  put_kstack(proc);
bad_fork_cleanup_proc:
  kfree(proc);
  goto fork_out;
}

int
do_exit(int error_code) {
  struct mm_struct *mm;
  int intr_flag;
  struct proc_struct *proc;

  if (current == idleproc) {
    panic("idleproc exit.\n");
  }
  if (current == initproc) {
    panic("initproc exit.\n");
  }

  mm = current->mm;
  if (mm != ((void *)0)) {
    pdir(boot_cr3);
    spage(1);
    if (mm_count_dec(mm) == 0) {
      exit_mmap(mm);
      put_pgdir(mm);
      mm_destroy(mm);
    }
    current->mm = ((void *)0);
  }
  current->state = PROC_ZOMBIE;
  current->exit_code = error_code;

  do { intr_flag = __intr_save(); } while (0);
  {
    proc = current->parent;
    if (proc->wait_state == (0x00000001 | 0x80000000)) {
      wakeup_proc(proc);
    }
    while (current->cptr != ((void *)0)) {
      proc = current->cptr;
      current->cptr = proc->optr;

      proc->yptr = ((void *)0);
      if ((proc->optr = initproc->cptr) != ((void *)0)) {
 initproc->cptr->yptr = proc;
      }
      proc->parent = initproc;
      initproc->cptr = proc;
      if (proc->state == PROC_ZOMBIE) {
 if (initproc->wait_state == (0x00000001 | 0x80000000)) {
   wakeup_proc(initproc);
 }
      }
    }
  }
  __intr_restore(intr_flag);;

  schedule();
  panic("do_exit will not return!! %d.\n", current->pid);
}

struct elfhdr {
  uint magic, size, bss, entry, flags;
};

uint ELF_MAGIC = 0xC0DEF00D;

static int
load_icode(unsigned char *binary, size_t bsize) {

  int ret = -4;
  struct mm_struct *mm;

  struct Page *page;

  uint32_t vm_flags, perm;
  unsigned char *from;
  size_t off, size;
  uintptr_t start, end, la;
  struct trapframe *tf;
  struct elfhdr *hdr = binary;

  if (hdr->magic != ELF_MAGIC) {
    ret = -8;
    goto bad_elf_cleanup_pgdir;
  }

  if (current->mm != ((void *)0)) {
    panic("load_icode: current->mm must be empty.\n");
  }

  if ((mm = mm_create()) == ((void *)0)) {
    goto bad_mm;
  }

  if (setup_pgdir(mm) != 0) {
    goto bad_pgdir_cleanup_mm;
  }

  vm_flags = 0, perm = 0x004;
  vm_flags = 0x00000004 | 0x00000002 | 0x00000001;
  if (vm_flags & 0x00000002) perm |= 0x002;

  if ((ret = mm_map(mm, 0x00200000, hdr->size + hdr->bss, vm_flags, ((void *)0))) != 0) {
    goto bad_cleanup_mmap;
  }

  from = (uint)(binary)+sizeof(struct elfhdr);
  start = 0x00200000;
  la = ROUNDDOWN(start, 4096);
  ret = -4;

  end = 0x00200000 + hdr->size;

  while (start < end) {
    if ((page = pgdir_alloc_page(mm->pgdir, la, perm)) == ((void *)0)) {
      goto bad_cleanup_mmap;
    }
    off = start - la, size = 4096 - off, la += 4096;
    if (end < la) {
      size -= la - end;
    }
    memcpy(page2kva(page) + off, from, size);
    start += size, from += size;
  }

  end = 0x00200000 + hdr->size + hdr->bss;
  if (start < la) {

    if (start == end) {
      continue;
    }
    off = start + 4096 - la, size = 4096 - off;
    if (end < la) {
      size -= la - end;
    }
    memset(page2kva(page) + off, 0, size);
    start += size;
    assert((end < la && start == end) || (end >= la && start == la));
  }
  while (start < end) {
    if ((page = pgdir_alloc_page(mm->pgdir, la, perm)) == ((void *)0)) {
      goto bad_cleanup_mmap;
    }
    off = start - la, size = 4096 - off, la += 4096;
    if (end < la) {
      size -= la - end;
    }
    memset(page2kva(page) + off, 0, size);
    start += size;
  }

  vm_flags = 0x00000001 | 0x00000002 | 0x00000008;
  if ((ret = mm_map(mm, 0xB0000000 - (256 * 4096), (256 * 4096), vm_flags, ((void *)0))) != 0) {
    goto bad_cleanup_mmap;
  }

  assert(pgdir_alloc_page(mm->pgdir, 0xB0000000 -4096, (0x004 | 0x002 | 0x001)) != ((void *)0));
  assert(pgdir_alloc_page(mm->pgdir, 0xB0000000 -2*4096, (0x004 | 0x002 | 0x001)) != ((void *)0));
  assert(pgdir_alloc_page(mm->pgdir, 0xB0000000 -3*4096, (0x004 | 0x002 | 0x001)) != ((void *)0));
  assert(pgdir_alloc_page(mm->pgdir, 0xB0000000 -4*4096, (0x004 | 0x002 | 0x001)) != ((void *)0));

  mm_count_inc(mm);
  current->mm = mm;
  current->cr3 = PADDR(mm->pgdir);
  pdir(PADDR(mm->pgdir));

  tf = current->tf;
  memset(tf, 0, sizeof(struct trapframe));

  tf->tf_regs.sp = 0xB0000000 - 32;
  tf->fc = tf->fc | 16;
  tf->pc = 0x00200000 + hdr->entry;
  ret = 0;

output:
  return ret;
bad_cleanup_mmap:
  exit_mmap(mm);
bad_elf_cleanup_pgdir:
  put_pgdir(mm);
bad_pgdir_cleanup_mm:
  mm_destroy(mm);
bad_mm:
  goto output;
}

static int exitfunc() {
  while(1) {}
}

static int exitend() {

}

int
do_execve(char *name, size_t len, unsigned char *binary, size_t size) {
  struct mm_struct *mm = current->mm;
  char local_name[15 + 1];
  int ret;
  if (!user_mem_check(mm, (uintptr_t)name, len, 0)) {
    return -3;
  }
  if (len > 15) {
    len = 15;
  }
  memset(local_name, 0, sizeof(local_name));
  memcpy(local_name, name, len);

  if (mm != ((void *)0)) {
    pdir(boot_cr3);
    spage(1);
    if (mm_count_dec(mm) == 0) {
      exit_mmap(mm);
      put_pgdir(mm);
      mm_destroy(mm);
    }
    current->mm = ((void *)0);
  }
  if ((ret = load_icode(binary, size)) != 0) {
    goto execve_exit;
  }
  set_proc_name(current, local_name);
  return 0;

execve_exit:
  do_exit(ret);
  panic("already exit: %e.\n", ret);
}

int
do_yield(void) {
  current->need_resched = 1;
  return 0;
}

int
do_wait(int pid, int *code_store) {
  struct mm_struct *mm = current->mm;
  struct proc_struct *proc;
  bool intr_flag, haskid;
  if (code_store != ((void *)0)) {
    if (!user_mem_check(mm, (uintptr_t)code_store, sizeof(int), 1)) {
      return -3;
    }
  }

repeat:
  haskid = 0;
  if (pid != 0) {
    proc = find_proc(pid);
    if (proc != ((void *)0) && proc->parent == current) {
      haskid = 1;
      if (proc->state == PROC_ZOMBIE) {
 goto found;
      }
    }
  }
  else {
    proc = current->cptr;
    for (; proc != ((void *)0); proc = proc->optr) {
      haskid = 1;
      if (proc->state == PROC_ZOMBIE) {
 goto found;
      }
    }
  }
  if (haskid) {
    current->state = PROC_SLEEPING;
    current->wait_state = (0x00000001 | 0x80000000);
    schedule();
    if (current->flags & 0x00000001) {
      do_exit(-9);
    }
    goto repeat;
  }
  return -2;

found:
  if (proc == idleproc || proc == initproc) {
    panic("wait idleproc or initproc.\n");
  }
  if (code_store != ((void *)0)) {
    *code_store = proc->exit_code;
  }
  do { intr_flag = __intr_save(); } while (0);
  {
    unhash_proc(proc);
    remove_links(proc);
  }
  __intr_restore(intr_flag);;
  put_kstack(proc);
  kfree(proc);
  return 0;
}

int
do_kill(int pid) {
  struct proc_struct *proc;
  if ((proc = find_proc(pid)) != ((void *)0)) {
    if (!(proc->flags & 0x00000001)) {
      proc->flags |= 0x00000001;
      if (proc->wait_state & 0x80000000) {
 wakeup_proc(proc);
      }
      return 0;
    }
    return -9;
  }
  return -3;
}

static int
__kernel_execve(char *name, uint* args) {
  asm (LI, 4);
  asm (LBL, 8);
  asm (LCL, 16);
  asm (TRAP);
}

static int
kernel_execve(char *name, unsigned char *binary, size_t size) {
  uint args[3];
  args[0] = strlen(name);
  args[1] = binary;
  args[2] = size;
  return __kernel_execve(name, args);
}

static int
user_main(void *arg) {
  kernel_execve("exit", memdisk, 0);
}

static int
init_main(void *arg) {
  size_t nr_free_pages_store = nr_free_pages();
  size_t kernel_allocated_store = kallocated();

  int pid;

  pid = kernel_thread(user_main, ((void *)0), 0);

  if (pid <= 0) {
    panic("create user_main failed.\n");
  }

  while (do_wait(0, ((void *)0)) == 0) {
    schedule();
  }

  printf("all user-mode processes have quit.\n");
  assert(initproc->cptr == ((void *)0) && initproc->yptr == ((void *)0) && initproc->optr == ((void *)0));
  assert(nr_process == 2);
  assert(list_next(&proc_list) == &(initproc->list_link));
  assert(list_prev(&proc_list) == &(initproc->list_link));
  assert(nr_free_pages_store == nr_free_pages());
  assert(kernel_allocated_store == kallocated());
  printf("init check memory pass.\n");
  return 0;
}

void
proc_init(void) {
  int i, pid;
  static char staspace[(2 * 4096) + 4096];
  char *bootstack = staspace;
  bootstack = bootstack - (uint32_t)bootstack % 4096 + 4096;

  list_init(&proc_list);
  for (i = 0; i < (1 << 10); i++) {
    list_init(hash_list + i);
  }

  if ((idleproc = alloc_proc()) == ((void *)0)) {
    panic("cannot alloc idleproc.\n");
  }

  idleproc->pid = 0;
  idleproc->state = PROC_RUNNABLE;
  idleproc->kstack = bootstack;
  idleproc->need_resched = 1;
  set_proc_name(idleproc, "idle");
  nr_process++;

  current = idleproc;
  pid = kernel_thread(init_main, ((void *)0), 0);

  if (pid <= 0) {
    panic("create init_main failed.\n");
  }

  initproc = find_proc(pid);
  set_proc_name(initproc, "init");

  assert(idleproc != ((void *)0) && idleproc->pid == 0);
  assert(initproc != ((void *)0) && initproc->pid == 1);
}

void
cpu_idle(void) {
  while (1) {
    if (current->need_resched) {
      schedule();
    }
  }
}

void
lab6_set_priority(uint32_t priority)
{
  if (priority == 0)
    current->lab6_priority = 1;
  else current->lab6_priority = priority;
}

static int
sys_exit(uint32_t arg[]) {
  int error_code = (int)arg[0];
  return do_exit(error_code);
}

static int
sys_fork(uint32_t arg[]) {
  struct trapframe *tf = current->tf;
  uintptr_t stack = tf->tf_regs.sp;
  return do_fork(0, stack, tf);
}

static int
sys_wait(uint32_t arg[]) {
  int pid = (int)arg[0];
  int *store = (int *)arg[1];
  return do_wait(pid, store);
}

static int
sys_exec(uint32_t arg[]) {
  char *name = (char *)arg[0];
  uint32_t *args = arg[1];
  size_t len = (size_t)args[0];
  unsigned char *binary = (unsigned char *)args[1];
  size_t size = (size_t)args[2];
  return do_execve(name, len, binary, size);
}

static int
sys_yield(uint32_t arg[]) {
  return do_yield();
}

static int
sys_kill(uint32_t arg[]) {
  int pid = (int)arg[0];
  return do_kill(pid);
}

static int
sys_getpid(uint32_t arg[]) {
  return current->pid;
}

static int
sys_putc(uint32_t arg[]) {
  int c = (int)arg[0];
  cputchar(c);
  return 0;
}

static int
sys_pgdir(uint32_t arg[]) {
  print_pgdir();
  return 0;
}

static int
sys_gettime(uint32_t arg[]) {
  return (int)ticks;
}

static int
sys_lab6_set_priority(uint32_t arg[])
{
  uint32_t priority = (uint32_t)arg[0];
  lab6_set_priority(priority);
  return 0;
}

int
syscall() {
  struct trapframe *tf = current->tf;
  uint32_t arg[2];
  int num = tf->tf_regs.a;
  arg[0] = tf->tf_regs.b;
  arg[1] = tf->tf_regs.c;
  switch (num) {
  case 1:
    return sys_exit(arg);
  case 2:
    return sys_fork(arg);
  case 3:
    return sys_wait(arg);
  case 4:
    return sys_exec(arg);
  case 10:
    return sys_yield(arg);
  case 12:
    return sys_kill(arg);
  case 18:
    return sys_getpid(arg);
  case 30:
    return sys_putc(arg);
  case 31:
    return sys_pgdir(arg);
  case 17:
    return sys_gettime(arg);
  case 255:
    return sys_lab6_set_priority(arg);
  }

  print_trapframe(tf);
  panic("undefined syscall %d, pid = %d, name = %s.\n",
        num, current->pid, current->name);
}

void trapname(int fc) {
  switch (fc) {
  case FMEM:
  case FMEM + USER:
    printf("bad physical address\n");
    return;
  case FTIMER:
  case FTIMER + USER:
    printf("timer interrupt\n");
    return;
  case FKEYBD:
  case FKEYBD + USER:
    printf("keyboard interrupt");
    return;
  case FPRIV:
  case FPRIV + USER:
    printf("privileged instruction");
    return;
  case FINST:
  case FINST + USER:
    printf("illegal instruction");
    return;
  case FSYS:
  case FSYS + USER:
    printf("software trap");
    return;
  case FARITH:
  case FARITH + USER:
    printf("arithmetic trap");
    return;
  case FIPAGE:
  case FIPAGE + USER:
    printf("page fault on opcode fetch");
    return;
  case FWPAGE:
  case FWPAGE + USER:
    printf("page fault on write");
    return;
  case FRPAGE:
  case FRPAGE + USER:
    printf("page fault on read");
    return;
  default:
    printf("Unkonw!\n");
  }
}

void print_pgfault(struct trapframe *tf) {

  printf("page fault at 0x%x: %c/%c [%s].\n", lvadr(),
         (tf->fc >= USER) ? 'U' : 'K',
         (tf->fc == FWPAGE || tf->fc == USER + FWPAGE) ? 'W' : 'R',
         (tf->tf_regs.b & 1) ? "protection fault" : "no page found");
}

int pgfault_handler(struct trapframe *tf) {
  struct mm_struct *mm;
  if(check_mm_struct != ((void *)0)) {
    print_pgfault(tf);
  }
  if (check_mm_struct != ((void *)0)) {
    assert(current == idleproc);
    mm = check_mm_struct;
  }
  else {
    if (current == ((void *)0)) {
      print_trapframe(tf);
      print_pgfault(tf);
      panic("unhandled page fault.\n");
    }
    mm = current->mm;
  }
  return do_pgfault(check_mm_struct, tf->fc, lvadr());
}

void trap_dispatch(struct trapframe *tf)
{
  uint va;
  uint ret;

  switch (tf->fc) {
  case FSYS:

  case FSYS + USER:
    tf->tf_regs.a = syscall();
    return;
  case FMEM:
    panic("FMEM from kernel");
  case FMEM + USER:
    printf("FMEM + USER\n");
    return;

  case FPRIV:
    panic("FPRIV from kernel");
  case FPRIV + USER:
    printf("FPRIV + USER\n");
    return;

  case FINST:
    panic("FINST from kernel");
  case FINST + USER:
    printf("FINST + USER\n");
    return;

  case FARITH:
    panic("FARITH from kernel");
  case FARITH + USER:
    printf("FARITH + USER\n");
    return;

  case FIPAGE:
    printf("FIPAGE from kernel [0x%x]", lvadr());
    panic("!\n");
  case FIPAGE + USER:
    printf("FIPAGE + USER [0x%x]", lvadr());
    return;

  case FWPAGE:
  case FWPAGE + USER:
  case FRPAGE:
  case FRPAGE + USER:
    if ((ret = pgfault_handler(tf)) != 0) {
      print_trapframe(tf);
      if (current == ((void *)0)) {
 panic("handle pgfault failed. ret=%d\n", ret);
      }
      else {
 if (trap_in_kernel(tf)) {
   panic("handle pgfault failed in kernel mode. ret=%d\n", ret);
 }
 printf("killed by kernel.\n");
 panic("handle user mode pgfault failed. ret=%d\n", ret);
      }
    }
    tf->pc = (uint *)(tf->pc) - 1;
    spage(1);
    return;
  case FTIMER:
  case FTIMER + USER:

    ticks++;
    assert(current != ((void *)0));
    sched_class_proc_tick(current);
    return;
  case FKEYBD:
  case FKEYBD + USER:

    return;
  }
}

void trap(struct trapframe *tf) {

  struct trapframe *otf;
  bool in_kernel;

  if (current == ((void *)0)) {
    trap_dispatch(tf);
  }
  else {

    otf = current->tf;
    current->tf = tf;
    in_kernel = trap_in_kernel(tf);

    trap_dispatch(tf);

    current->tf = otf;
    if (!in_kernel) {
      if (current->flags & 0x00000001) {
 do_exit(-9);
      }
      if (current->need_resched) {
 schedule();
      }
    }
  }

}

void trap_before(uint *sp, double g, double f, int c, int b, int a, int fc, uint *pc) {
  trap(&sp);
}

void alltraps()
{
  asm (PSHA);
  asm (PSHB);
  asm (PSHC);
  asm (PSHF);
  asm (PSHG);
  asm (LUSP); asm (PSHA);
  trap_before();
  asm (POPA); asm (SUSP);
  asm (POPG);
  asm (POPF);
  asm (POPC);
  asm (POPB);
  asm (POPA);
  asm (RTI);
}

void idt_init() {
  ivec(alltraps);
}

list_entry_t pra_list_head;

int _fifo_init_mm(struct mm_struct *mm)
{
  list_init(&pra_list_head);
  mm->sm_priv = &pra_list_head;

  return 0;
}

int _fifo_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in)
{
  list_entry_t *head=(list_entry_t*) mm->sm_priv;
  list_entry_t *entry=&(page->pra_page_link);

  assert(entry != ((void *)0) && head != ((void *)0));

  list_add(head, entry);
  return 0;
}

int
_fifo_swap_out_victim(struct mm_struct *mm, struct Page ** ptr_page, int in_tick)
{
  list_entry_t *le;
  struct Page *p;
  list_entry_t *head = (list_entry_t*) mm->sm_priv;
  assert(head != ((void *)0));
  assert(in_tick==0);

  le = head->prev;
  assert(head!=le);
  p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->pra_page_link))));
  list_del(le);
  assert(p !=((void *)0));
  *ptr_page = p;
  return 0;
}

int
_fifo_check_swap(void) {
  int tmp;
  printf("write Virt Page c in fifo_check_swap\n");
  *(unsigned char *)0x3000 = 0x0c;
  assert(pgfault_num==4);
  printf("write Virt Page a in fifo_check_swap\n");
  *(unsigned char *)0x1000 = 0x0a;
  assert(pgfault_num==4);
  printf("write Virt Page d in fifo_check_swap\n");
  *(unsigned char *)0x4000 = 0x0d;
  assert(pgfault_num==4);
  printf("write Virt Page b in fifo_check_swap\n");
  *(unsigned char *)0x2000 = 0x0b;
  assert(pgfault_num==4);
  printf("write Virt Page e in fifo_check_swap\n");
  *(unsigned char *)0x5000 = 0x0e;
  assert(pgfault_num==5);
  printf("write Virt Page b in fifo_check_swap\n");
  *(unsigned char *)0x2000 = 0x0b;
  assert(pgfault_num==5);
  printf("write Virt Page a in fifo_check_swap\n");
  *(unsigned char *)0x1000 = 0x0a;
  assert(pgfault_num==6);
  printf("write Virt Page b in fifo_check_swap\n");
  *(unsigned char *)0x2000 = 0x0b;
  assert(pgfault_num==7);
  printf("write Virt Page c in fifo_check_swap\n");
  *(unsigned char *)0x3000 = 0x0c;
  assert(pgfault_num==8);
  printf("write Virt Page d in fifo_check_swap\n");
  *(unsigned char *)0x4000 = 0x0d;
  assert(pgfault_num==9);
  printf("write Virt Page e in fifo_check_swap\n");
  *(unsigned char *)0x5000 = 0x0e;
  assert(pgfault_num==10);
  printf("write Virt Page a in fifo_check_swap\n");
  assert(*(unsigned char *)0x1000 == 0x0a);
  *(unsigned char *)0x1000 = 0x0a;
  assert(pgfault_num==11);
  return 0;
}

int _fifo_init(void) {
  return 0;
}

int _fifo_set_unswappable(struct mm_struct *mm, uintptr_t addr) {
  return 0;
}

int _fifo_tick_event(struct mm_struct *mm) {
  return 0;
}

static char disk[1 * 1024 * 8 * 512];

void
ide_init(void) {
  memset(disk, sizeof(disk), 0);
}

size_t
ide_device_size(unsigned short ideno) {
  return 1 * 1024 * 8;
}

int
ide_read_secs(unsigned short ideno, uint32_t secno, void *dst, size_t nsecs) {

  int ret = 0;
  int i;

  secno = secno * 512;

  for (; nsecs > 0; nsecs--, dst += 512, secno += 512) {
    for (i = 0; i < 512; i++) {

      *((char *)(dst) + i) = disk[secno + i];
    }

  }

  return ret;
}

int
ide_write_secs(unsigned short ideno, uint32_t secno, uint32_t *src, size_t nsecs) {

  int ret = 0;
  int i;

  secno = secno * 512;

  for (; nsecs > 0; nsecs--, src += 512, secno += 512) {
    for (i = 0; i < 512; i++) {

      disk[secno + i] = *((char *)(src) + i);
    }

  }

  return ret;

}

size_t max_swap_offset;

uint swap_offset(uint entry) {
  size_t __offset = (entry >> 8);
  if (!(__offset > 0 && __offset < max_swap_offset)) {
    panic("invalid swap_entry_t = %x.\n", entry);
  }
  return __offset;
}

void
swapfs_init(void) {
  assert((4096 % 512) == 0);
  max_swap_offset = ide_device_size(1) / (4096 / 512);
}

int
swapfs_read(swap_entry_t entry, struct Page *page) {
  return ide_read_secs(1, swap_offset(entry) * (4096 / 512), page2kva(page), (4096 / 512));
}

int
swapfs_write(swap_entry_t entry, struct Page *page) {
  return ide_write_secs(1, swap_offset(entry) * (4096 / 512), page2kva(page), (4096 / 512));
}

unsigned int swap_page[5];

unsigned int swap_in_seq_no[10],swap_out_seq_no[10];

struct swap_manager {
  char* name;

  void* init;

  void* init_mm;

  void* tick_event;

  void* map_swappable;

  void* set_unswappable;

  void* swap_out_victim;

  void* check_swap;
};

struct swap_manager *sm;

struct swap_manager swap_manager_fifo;

void load_swap_manager() {
  swap_manager_fifo.name = "fifo swap manager";
  swap_manager_fifo.init = _fifo_init;
  swap_manager_fifo.init_mm = _fifo_init_mm;
  swap_manager_fifo.tick_event = _fifo_tick_event;
  swap_manager_fifo.map_swappable = _fifo_map_swappable;
  swap_manager_fifo.set_unswappable = _fifo_set_unswappable;
  swap_manager_fifo.swap_out_victim = _fifo_swap_out_victim;
  swap_manager_fifo.check_swap = _fifo_check_swap;
}

void check_swap(void);

int
swap_init(void)
{
  int r;
  swapfs_init();
  if (!(1024 <= max_swap_offset && max_swap_offset < (1 << 24)))
  {
    panic("bad max_swap_offset %08x.\n", max_swap_offset);
  }

  load_swap_manager();
  sm = &swap_manager_fifo;
  r = call0(sm->init);
  if (r == 0) {
    swap_init_ok = 1;
    printf("SWAP: manager = %s\n", sm->name);
    check_swap();
  }

  return r;
}

int
swap_init_mm(struct mm_struct *mm)
{
  return call1(mm, sm->init_mm);
}

int
swap_tick_event(struct mm_struct *mm)
{
  return call1(mm, sm->tick_event);
}

int
swap_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in)
{
  return call4(mm, addr, page, swap_in, sm->map_swappable);
}

int
swap_set_unswappable(struct mm_struct *mm, uintptr_t addr)
{
  return call2(mm, addr, sm->set_unswappable);
}

unsigned int swap_out_num = 0;

int
swap_out(struct mm_struct *mm, int n, int in_tick)
{
  int i;
  uintptr_t v;
  struct Page *page;
  pte_t *ptep;
  int r;

  for (i = 0; i != n; ++i)
  {

    r = call3(mm, &page, in_tick, sm->swap_out_victim);

    if (r != 0) {
      printf("i %d, swap_out: call swap_out_victim failed\n",i);
      break;
    }

    v = page->pra_vaddr;
    ptep = get_pte(mm->pgdir, v, 0);
    assert((*ptep & 0x001) != 0);
    if (swapfs_write( (page->pra_vaddr/4096 +1)<<8, page) != 0) {
      printf("SWAP: failed to save\n");
      call4(mm, v, page, 0, sm->map_swappable);
      continue;
    }
    else {
      printf("swap_out: i %d, store page in vaddr %x to disk swap entry %d\n", i, v, page->pra_vaddr/4096 +1);
      *ptep = (page->pra_vaddr/4096 +1)<<8;
      free_pages(page, 1);
    }

    spage(1);
  }

  return i;
}

int
swap_in(struct mm_struct *mm, uintptr_t addr, struct Page **ptr_result)
{
  struct Page *result = alloc_pages(1);
  pte_t *ptep;
  int r;

  assert(result!=((void *)0));

  ptep = get_pte(mm->pgdir, addr, 0);
  printf("SWAP: load ptep %x swap entry %d to vaddr 0x%x, page %x, No %d\n", ptep, (*ptep)>>8, addr, result, (result-pages));

  if ((r = swapfs_read((*ptep), result)) != 0)
  {
    assert(r!=0);
  }
  printf("swap_in: load disk swap entry %d with swap_page in vadr 0x%x\n", (*ptep)>>8, addr);
  *ptr_result = result;
  return 0;
}

void
check_content_set(void)
{
  *(unsigned char *)0x1000 = 0x0a;
  assert(pgfault_num==1);
  *(unsigned char *)0x1010 = 0x0a;
  assert(pgfault_num==1);
  *(unsigned char *)0x2000 = 0x0b;
  assert(pgfault_num==2);
  *(unsigned char *)0x2010 = 0x0b;
  assert(pgfault_num==2);
  *(unsigned char *)0x3000 = 0x0c;
  assert(pgfault_num==3);
  *(unsigned char *)0x3010 = 0x0c;
  assert(pgfault_num==3);
  *(unsigned char *)0x4000 = 0x0d;
  assert(pgfault_num==4);
  *(unsigned char *)0x4010 = 0x0d;
  assert(pgfault_num==4);
}

int check_content_access(void)
{
  int ret = call0(sm->check_swap);
  return ret;
}

struct Page * check_rp[4];
pte_t * check_ptep[4];
unsigned int check_swap_addr[5];

void check_swap(void)
{

  int ret, count, total, i;
  unsigned int nr_free_store;
  struct Page* p;
  struct mm_struct* mm;
  struct vma_struct* vma;
  list_entry_t* le;
  pde_t* pgdir;
  pte_t* temp_ptep;
  list_entry_t free_list_store;

  le = &(free_area.free_list);
  count = 0;
  total = 0;
  while ((le = list_next(le)) != &(free_area.free_list)) {
    p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
    assert(test_bit(1, &((p)->flags)));
    count++;
    total += p->property;
  }
  assert(total == nr_free_pages());
  printf("BEGIN check_swap: count %d, total %d\n", count, total);

  mm = mm_create();
  assert(mm != ((void *)0));
  assert(check_mm_struct == ((void *)0));
  check_mm_struct = mm;

  pgdir = mm->pgdir = boot_pgdir;
  assert(pgdir[0] == 0);

  vma = vma_create(0X1000, (5 +1)*0x1000, 0x00000002 | 0x00000001);
  assert(vma != ((void *)0));
  insert_vma_struct(mm, vma);

  printf("setup Page Table for vaddr 0X1000, so alloc a page\n");
  temp_ptep = ((void *)0);
  temp_ptep = get_pte(mm->pgdir, 0X1000, 1);
  assert(temp_ptep!= ((void *)0));
  printf("setup Page Table vaddr 0~4MB OVER!\n");

  for (i=0; i<4; i++) {
    check_rp[i] = alloc_pages(1);
    assert(check_rp[i] != ((void *)0) );
    assert(!test_bit(1, &((check_rp[i])->flags)));
  }

  memcpy(&free_list_store, &(free_area.free_list), sizeof(struct list_entry));
  list_init(&(free_area.free_list));
  assert(list_empty(&(free_area.free_list)));

  nr_free_store = (free_area.nr_free);
  (free_area.nr_free) = 0;
  for (i=0; i<4; i++) {
    free_pages(check_rp[i], 1);
  }
  assert((free_area.nr_free)==4);
  printf("set up init env for check_swap begin!\n");

  pgfault_num=0;

  check_content_set();
  assert( (free_area.nr_free) == 0);
  for(i = 0; i<10; i++)
    swap_out_seq_no[i]=swap_in_seq_no[i]=-1;

  for (i= 0; i<4; i++) {
    check_ptep[i] = 0;
    check_ptep[i] = get_pte(pgdir, (i+1)*0x1000, 0);
    assert(check_ptep[i] != ((void *)0));
    assert(pte2page(*check_ptep[i]) == check_rp[i]);
    assert((*check_ptep[i] & 0x001));
  }
  printf("set up init env for check_swap over!\n");

  ret = check_content_access();
  assert(ret==0);

  for (i=0; i<4; i++) {
    free_pages(check_rp[i],1);
  }

  free_pages(pde2page(pgdir[0]), 1);
  pgdir[0] = 0;
  mm->pgdir = ((void *)0);

  mm_destroy(mm);

  check_mm_struct = ((void *)0);

  (free_area.nr_free) = nr_free_store;
  memcpy(&(free_area.free_list), &free_list_store, sizeof(struct list_entry));

  printf("count is %d, total is %d\n",count,total);
  le = &(free_area.free_list);
  while ((le = list_next(le)) != &(free_area.free_list)) {
    p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
    count--;
    total -= p->property;
  }
  printf("count is %d, total is %d\n",count,total);

  printf("check_swap() succeeded!\n");
}

void kern_init() {
  printf("(THU.CST) os is running ...\n");
  pmm_init();
  idt_init();
  asm (STI);
  spage(1);
  vmm_init();
  sched_init();
  swap_init();
  proc_init();
  stmr(128*1024);
  cpu_idle();
}

main() {
  int *ksp;

  static int bss;
  endbss = &bss;

  ksp = ((uint)kstack + sizeof(kstack) - 8) & -8;
  asm (LL, 4);
  asm (SSP);

  kern_init();

  asm (HALT);
}
