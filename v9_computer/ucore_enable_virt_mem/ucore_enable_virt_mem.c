enum {
  HALT,ENT ,LEV ,JMP ,JMPI,JSR ,JSRA,LEA ,LEAG,CYC ,MCPY,MCMP,MCHR,MSET,
  LL ,LLS ,LLH ,LLC ,LLB ,LLD ,LLF ,LG ,LGS ,LGH ,LGC ,LGB ,LGD ,LGF ,
  LX ,LXS ,LXH ,LXC ,LXB ,LXD ,LXF ,LI ,LHI ,LIF ,
  LBL ,LBLS,LBLH,LBLC,LBLB,LBLD,LBLF,LBG ,LBGS,LBGH,LBGC,LBGB,LBGD,LBGF,
  LBX ,LBXS,LBXH,LBXC,LBXB,LBXD,LBXF,LBI ,LBHI,LBIF,LBA ,LBAD,
  SL ,SLH ,SLB ,SLD ,SLF ,SG ,SGH ,SGB ,SGD ,SGF ,
  SX ,SXH ,SXB ,SXD ,SXF ,
  ADDF,SUBF,MULF,DIVF,
  ADD ,ADDI,ADDL,SUB ,SUBI,SUBL,MUL ,MULI,MULL,DIV ,DIVI,DIVL,
  DVU ,DVUI,DVUL,MOD ,MODI,MODL,MDU ,MDUI,MDUL,AND ,ANDI,ANDL,
  OR ,ORI ,ORL ,XOR ,XORI,XORL,SHL ,SHLI,SHLL,SHR ,SHRI,SHRL,
  SRU ,SRUI,SRUL,EQ ,EQF ,NE ,NEF ,LT ,LTU ,LTF ,GE ,GEU ,GEF ,
  BZ ,BZF ,BNZ ,BNZF,BE ,BEF ,BNE ,BNEF,BLT ,BLTU,BLTF,BGE ,BGEU,BGEF,
  CID ,CUD ,CDI ,CDU ,
  CLI ,STI ,RTI ,BIN ,BOUT,NOP ,SSP ,PSHA,PSHI,PSHF,PSHB,POPB,POPF,POPA,
  IVEC,PDIR,SPAG,TIME,LVAD,TRAP,LUSP,SUSP,LCL ,LCA ,PSHC,POPC,MSIZ,
  PSHG,POPG,NET1,NET2,NET3,NET4,NET5,NET6,NET7,NET8,NET9,
  POW ,ATN2,FABS,ATAN,LOG ,LOGT,EXP ,FLOR,CEIL,HYPO,SIN ,COS ,TAN ,ASIN,
  ACOS,SINH,COSH,TANH,SQRT,FMOD,
  IDLE
};

enum {
  S_fork=1, S_exit, S_wait, S_pipe, S_write, S_read, S_close, S_kill,
  S_exec, S_open, S_mknod, S_unlink, S_fstat, S_link, S_mkdir, S_chdir,
  S_dup2, S_getpid, S_sbrk, S_sleep, S_uptime, S_lseek, S_mount, S_umount,
  S_socket, S_bind, S_listen, S_poll, S_accept, S_connect,
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

set_bit(int nr, uint* addr) {
  *addr = (*addr) | (1 << nr);
}

clear_bit(int nr, uint* addr) {
  *addr = ((*addr) | (1 << nr)) ^ (1 << nr);
}

change_bit(int nr, uint* addr) {
  *addr = (*addr) ^ (1 << nr);
}

int test_bit(int nr, uint* addr) {
  if (((*addr) & (1 << nr)) == 0)
    return 0;
  return 1;
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
printnum(funcptr_t putch, int fd, void *putdat,
         unsigned int num, unsigned int base, int width, int padc) {
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

static void cout(char c) {
  out(1, c);
}

static void cputch(int c, int *cnt, int unused) {
  cout(c);
  (*cnt)++;
}

static int
vkprintf(char *fmt, va_list ap) {
  int cnt = 0;
  int NO_FD = -0x9527;
  vprintfmt(cputch, NO_FD, &cnt, fmt, ap);
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

struct Page *pages;

size_t npage = 0;

pde_t *boot_pgdir = ((void *)0);

uintptr_t boot_cr3;

uint endbss;
uint page_enable;

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
  char *name;
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
  maxpa = msiz();
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

tlb_clear_enable = 0;

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
      swap_map_swappable(check_mm_struct, la, page, 0);
      page->pra_vaddr=la;
      assert(page_ref(page) == 1);
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
}

uint getsp() {
  asm (LEA, 8);
}

static char kstack[4096];

void
pmm_init() {
  int *ksp;

  page_enable = 0;

  init_pmm_manager();

  page_init();

  check_alloc_page();

  boot_pgdir = boot_alloc_page();
  memset(boot_pgdir, 0, 4096);
  boot_cr3 = boot_pgdir;

  check_pgdir();

  boot_pgdir[((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)] = (uint)(boot_pgdir) | 0x001 | 0x002;

  boot_map_segment(boot_pgdir, 0xC0000000, 0x38000000, 0, 0x002);

  boot_pgdir[0] = boot_pgdir[((((uintptr_t)(0xC0000000)) >> 22) & 0x3FF)];

  page_enable = 1;

  pdir(boot_cr3);
  spage(1);

  ksp = 0xC0000000 + getsp();
  asm (LL, 4);
  asm (SSP);
  *(uint *)(getsp()+8) += 0xC0000000;
  *(uint *)(getsp()+16) += 0xC0000000;

  ksp = (uint)(check_and_return) + 0xC0000000;
  asm (LL, 4);
  asm (JSRA);
}

void *
kmalloc(size_t n) {
  void * ptr=((void *)0);
  struct Page *base=((void *)0);
  int num_pages=(n+4096 -1)/4096;
  assert(n > 0 && n < 1024*0124);
  base = alloc_pages(num_pages);
  assert(base != ((void *)0));
  ptr=page2kva(base);
  return ptr;
}

void
kfree(void *ptr, size_t n) {
  struct Page *base=((void *)0);
  int num_pages=(n+4096 -1)/4096;
  assert(n > 0 && n < 1024*0124);
  assert(ptr != ((void *)0));
  base = kva2page(ptr);
  free_pages(base, num_pages);
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

uint trap_in_kernel(struct trapframe *tf) {
  if (tf->fc >= USER)
    return 0;
  else
    return 1;
}

void print_regs(struct pushregs *regs) {
    printf("  SP : 0x%x\n", regs -> sp);

    printf("  REG_A : 0x%x\n", regs -> a);
    printf("  REG_B : 0x%x\n", regs -> b);
    printf("  REG_C : 0x%x\n", regs -> c);
}

void print_trapframe(struct trapframe *tf) {

    printf("PC : 0x%x\n", tf->pc);
    printf("\n");
}

void print_pgfault(struct trapframe *tf) {

    printf("page fault at 0x%x: %c/%c [%s].\n", lvadr(),
            (tf->fc >= USER) ? 'U' : 'K',
            (tf->fc == FWPAGE || tf->fc == USER + FWPAGE) ? 'W' : 'R',
            (tf->tf_regs.b & 1) ? "protection fault" : "no page found");
}

int do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr);

int pgfault_handler(struct trapframe *tf) {
    print_pgfault(tf);
    if (check_mm_struct != ((void *)0)) {
        return do_pgfault(check_mm_struct, tf->fc, lvadr());
    }
    panic("unhandled page fault.\n");
}

void trap_dispatch(struct trapframe *tf)
{
  uint va;
  uint ret;
  print_trapframe(tf);
  switch (tf -> fc) {
    case FSYS: panic("FSYS from kernel");
    case FSYS + USER:
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
            panic("handle pgfault failed. %e\n", ret);
      }
      tf->pc = (uint *)(tf->pc) - 1;
      spage(1);
      return;
    case FTIMER:
    case FTIMER + USER:
      ticks = ticks + 1;
      printf("%x\n", ticks);
      return;
    case FKEYBD:
    case FKEYBD + USER:

      return;
  }
}

void trap(uint *sp, double g, double f, int c, int b, int a, int fc, uint *pc) {
  trap_dispatch(&sp);
}

void alltraps()
{
  asm(PSHA);
  asm(PSHB);
  asm(PSHC);
  asm(PSHF);
  asm(PSHG);
  asm(LUSP); asm(PSHA);
  trap();
  asm(POPA); asm(SUSP);
  asm(POPG);
  asm(POPF);
  asm(POPC);
  asm(POPB);
  asm(POPA);
  asm(RTI);
}

void idt_init() {
  ivec(alltraps);
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
  while ((le = list_next(list)) != list) {
    list_del(le);
    kfree(((struct vma_struct *)((char *)((le)) - ((size_t)(&((struct vma_struct *)0)->list_link)))), sizeof(struct vma_struct));
  }
  kfree(mm, sizeof(struct mm_struct));
  mm=((void *)0);
}

void check_vma_struct(void) {
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

  assert(nr_free_pages_store == nr_free_pages());

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

  assert(nr_free_pages_store == nr_free_pages());

  printf("check_vmm() succeeded.\n");
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

    if(swap_init_ok) {
      page=((void *)0);
      if ((ret = swap_in(mm, addr, &page)) != 0) {
 printf("swap_in in do_pgfault failed\n");
 return ret;
      }
      spage(1);
      page_insert(mm->pgdir, page, addr, perm);
      swap_map_swappable(mm, addr, page, 1);
      page->pra_vaddr = addr;
    }
    else {
      printf("no swap_init_ok but ptep is %x, failed\n",*ptep);
      return ret;
    }
  }
  ret = 0;
  return ret;
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

  mm_destroy(mm);

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
  tlb_clear_enable = 1;
  spage(1);
  vmm_init();
  swap_init();
  stmr(128*1024*1000);
  while (1) {
  }
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
