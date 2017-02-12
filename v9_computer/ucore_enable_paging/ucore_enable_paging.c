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

uint ROUNDDOWN(uint a, uint n)
{
    return a - a % n;

}

uint ROUNDUP(uint a, uint n)
{
    return ROUNDDOWN(a + n - 1, n);
}

struct list_entry
{
    struct list_entry *prev, *next;
};

typedef struct list_entry list_entry_t;

void __list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next)
{
    prev->next = next->prev = elm;
    elm->next = next;
    elm->prev = prev;
}

void __list_del(list_entry_t *prev, list_entry_t *next)
{
    prev->next = next;
    next->prev = prev;
}

void list_init(list_entry_t *elm)
{
    elm->prev = elm->next = elm;
}

void list_add_after(list_entry_t *listelm, list_entry_t *elm)
{
    __list_add(elm, listelm, listelm->next);
}


void list_add_before(list_entry_t *listelm, list_entry_t *elm)
{
    __list_add(elm, listelm->prev, listelm);
}

void list_del(list_entry_t *listelm)
{
    __list_del(listelm->prev, listelm->next);
}

void list_del_init(list_entry_t *listelm)
{
    list_del(listelm);
    list_init(listelm);
}

bool list_empty(list_entry_t *list)
{
    return list->next == list;
}

list_entry_t * list_next(list_entry_t *listelm)
{
    return listelm->next;
}

list_entry_t * list_prev(list_entry_t *listelm)
{
    return listelm->prev;
}

set_bit(int nr, uint* addr)
{
    *addr = (*addr) | (1 << nr);
}

clear_bit(int nr, uint* addr)
{
    *addr = ((*addr) | (1 << nr)) ^ (1 << nr);
}

change_bit(int nr, uint* addr)
{
    *addr = (*addr) ^ (1 << nr);
}

int test_bit(int nr, uint* addr)
{
    if (((*addr) & (1 << nr)) == 0)
        return 0;
    return 1;
}

void *memcpy(void *d, void *s, uint n)
{
    asm (LL,8);
    asm (LBL, 16);
    asm (LCL,24);
    asm (MCPY);
    asm (LL,8);
}

void *memset(void *d, uint c, uint n)
{
    asm (LL,8);
    asm (LBLB,16);
    asm (LCL,24);
    asm (MSET);
    asm (LL,8);
}

size_t strlen(char *s)
{
    size_t cnt = 0;
    while (*s != '\0')
    {
        s++;
        cnt++;
    }
    return cnt;
}

size_t strnlen(char *s, size_t len)
{
    size_t cnt = 0;
    while (cnt < len && *s++ != '\0')
    {
        cnt++;
    }
    return cnt;
}

char *strcpy(char *dst, char *src)
{
    char *p = dst;
    while ((*p++ = *src++) != '\0')
        ;
    return dst;
}



int strcmp(char *s1, char *s2)
{
    while (*s1 != '\0' && *s1 == *s2)
    {
        s1++, s2++;
    }
    return (int)((unsigned char)*s1 - (unsigned char)*s2);

}

typedef unsigned int funcptr_t;

void call0(funcptr_t func)
{
    asm (LL, 8);    asm (PSHA);    asm (LEV);
}

void call1(int arg0, funcptr_t func)
{
    asm (LL, 16);    asm (PSHA);    asm (LEV);
}

void call2(int arg0, int arg1, funcptr_t func)
{
    asm (LL, 24);    asm (PSHA);    asm (LEV);
}

void call3(int arg0, int arg1, int arg2, funcptr_t func)
{
    asm (LL, 32);    asm (PSHA);    asm (LEV);
}

static char * error_string[24 + 1];

void printfmt(void (*putch)(int, void*, int), int fd, void *putdat, char *fmt, ...);

void vprintfmt(funcptr_t putch, int fd, void *putdat, char *fmt, va_list ap);

static uint32_t getuint(va_list *ap, int lflag)
{
    return va_arg(*ap, unsigned int);
}

static int32_t getint(va_list *ap, int lflag)
{
    return va_arg(*ap, int);
}

static void printnum(funcptr_t putch, int fd, void *putdat,
                     unsigned int num, unsigned int base, int width, int padc)
{
    unsigned int result = num;
    unsigned mod;
    mod = result % base;
    result /= base;


    if (num >= base)
    {
        printnum(putch, fd, putdat, result, base, width - 1, padc);
    }
    else
    {
        while (--width > 0)
            call3(padc, putdat, fd, putch);
    }
    call3("0123456789abcdef"[mod], putdat, fd, putch);
}

void vprintfmt(funcptr_t putch, int fd, void *putdat, char *fmt, va_list ap)
{
    char *p;
    int ch, err;
    int num;
    int base, width, precision, lflag, altflag;
    char padc;
    while (1)
    {
        while ((ch = *(unsigned char *)fmt++) != '%')
        {
            if (ch == '\0')
            {
                return;
            }
            call3(ch, putdat, fd, putch);
        }
        padc = ' ';
        width = precision = -1;
        lflag = altflag = 0;

reswitch:
        switch (ch = *(unsigned char *)fmt++)
        {
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
            for (precision = 0;; ++fmt)
            {
                precision = precision * 10 + ch - '0';
                ch = *fmt;
                if (ch < '0' || ch > '9')
                {
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
            if (err < 0)
            {
                err = -err;
            }
            if (err > 24 || (p = error_string[err]) == ((void *)0))
            {
                printfmt(putch, fd, putdat, "error %d", err);
            }
            else
            {
                printfmt(putch, fd, putdat, "%s", p);
            }
            break;
        case 's':
            if ((p = va_arg(ap, char *)) == ((void *)0))
            {
                p = "(null)";
            }
            if (width > 0 && padc != '-')
            {
                for (width -= strnlen(p, precision); width > 0; width--)
                {
                    call3(padc, putdat, fd, putch);
                }
            }
            for (; (ch = *p++) != '\0' && (precision < 0 || --precision >= 0); width--)
            {
                if (altflag && (ch < ' ' || ch > '~'))
                {
                    call3('?', putdat, fd, putch);
                }
                else
                {
                    call3(ch, putdat, fd, putch);
                }
            }
            for (; width > 0; width--)
            {
                call3(' ', putdat, fd, putch);
            }
            break;

        case 'd':
            num = getint(&ap, lflag);
            if (num < 0)
            {
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
printfmt(void (*putch)(int, void*, int), int fd, void *putdat, char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintfmt(putch, fd, putdat, fmt, ap);

}

out(port, val)
{
    asm(LL,8);
    asm(LBL,16);
    asm(BOUT);
}
ivec(void *isr)
{
    asm(LL,8);
    asm(IVEC);
}
lvadr()
{
    asm(LVAD);
}
uint msiz()
{
    asm(MSIZ);
}
stmr(val)
{
    asm(LL,8);
    asm(TIME);
}
pdir(val)
{
    asm(LL,8);
    asm(PDIR);
}
spage(val)
{
    asm(LL,8);
    asm(SPAG);
}
splhi()
{
    asm(CLI);
}
splx(int e)
{
    if (e) asm(STI);
}

static void cout(char c)
{
    out(1, c);
}

static void cputch(int c, int *cnt, int unused)
{
    cout(c);
    (*cnt)++;
}

static int vkprintf(char *fmt, va_list ap)
{
    int cnt = 0;
    int NO_FD = -0x9527;
    vprintfmt(cputch, NO_FD, &cnt, fmt, ap);
    return cnt;
}

int printf(char *fmt, ...)
{
    va_list ap;
    int cnt;
    va_start(ap, fmt);
    cnt = vkprintf(fmt, ap);
    return cnt;
}

panic(char *s)
{
    asm (CLI);
    out(1,'p');
    out(1,'a');
    out(1,'n');
    out(1,'i');
    out(1,'c');
    out(1,':');
    out(1,' ');
    while (*s) out(1,*s++);
    out(1,'\n');
    asm (HALT);
}

assert(int x)
{
    if (x == 0)
        panic("assertion failed: %s", x);
}

bool __intr_save(void)
{
    return splhi();
}

void __intr_restore(bool flag)
{
    splx(flag);
}

struct pushregs
{
    int sp, pad1;
    double g;
    double f;
    int c, pad2;
    int b, pad3;
    int a, pad4;
};

struct trapframe
{
    struct pushregs tf_regs;
    int fc, pad5;
    int pc, pad6;
};

enum
{
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

uint trap_in_kernel(struct trapframe *tf)
{
    if (tf->fc >= USER)
        return 0;
    else
        return 1;
}

void print_regs(struct pushregs *regs)
{
    printf("  SP : 0x%x\n", regs -> sp);
    printf("  REG_G : %f\n", regs -> g);
    printf("  REG_F : %f\n", regs -> f);
    printf("  REG_A : 0x%x\n", regs -> a);
    printf("  REG_B : 0x%x\n", regs -> b);
    printf("  REG_C : 0x%x\n", regs -> c);
}

void print_trapframe(struct trapframe *tf)
{
    printf("trapframe at %x\n", tf);
    print_regs(&tf->tf_regs);
    if (!trap_in_kernel(tf))
    {
        printf("Trap in usermode!\n");
    }
    else
    {
        printf("Trap in kernel!\n");
    }
    printf("Error Code: %d\n", tf->fc);
    printf("PC : 0x%x\n", tf->pc);
    printf("\n");
}

void trap_dispatch(struct trapframe *tf)
{
    uint va;
    print_trapframe(tf);
    switch (tf -> fc)
    {
    case FSYS:
        panic("FSYS from kernel");
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

void trap(uint *sp, double g, double f, int c, int b, int a, int fc, uint *pc)
{
    trap_dispatch(&sp);
}

void alltraps()
{
    asm(PSHA);
    asm(PSHB);
    asm(PSHC);
    asm(PSHF);
    asm(PSHG);
    asm(LUSP);
    asm(PSHA);
    trap();
    asm(POPA);
    asm(SUSP);
    asm(POPG);
    asm(POPF);
    asm(POPC);
    asm(POPB);
    asm(POPA);
    asm(RTI);
}

void idt_init()
{
    ivec(alltraps);
}

typedef uintptr_t pte_t;
typedef uintptr_t pde_t;

struct Page
{
    int ref;
    uint32_t flags;
    unsigned int property;
    list_entry_t page_link;
};

typedef struct
{
    list_entry_t free_list;
    unsigned int nr_free;
} free_area_t;

struct Page *pages;

size_t npage = 0;

pde_t *boot_pgdir = ((void *)0);

uintptr_t boot_cr3;

uint endbss;
uint page_enable;

uint PADDR(uint kva)
{
    uint __m_kva;
    __m_kva = kva;



    if (page_enable == 1)
        return __m_kva - 0xC0000000;
    else
        return __m_kva;
}

uint KADDR(uint pa)
{
    uint __m_pa;
    uint __m_ppn;
    __m_pa = pa;
    __m_ppn = (((uintptr_t)(__m_pa)) >> 12);



    if (page_enable == 1)
        return __m_pa + 0xC0000000;
    else
        return __m_pa;
}

ppn_t page2ppn(struct Page *page)
{
    return page - pages;
}

uintptr_t page2pa(struct Page *page)
{
    return page2ppn(page) << 12;
}

struct Page * pa2page(uintptr_t pa)
{
    if ((((uintptr_t)(pa)) >> 12) >= npage)
    {
        panic("pa2page called with invalid pa");
    }
    return &pages[(((uintptr_t)(pa)) >> 12)];
}

void * page2kva(struct Page *page)
{
    return KADDR(page2pa(page));
}

struct Page * pte2page(pte_t pte)
{
    if (!(pte & 0x001))
    {
        panic("pte2page called with invalid pte");
    }
    return pa2page(((uintptr_t)(pte) & ~0xFFF));
}

struct Page * pde2page(pde_t pde)
{
    return pa2page(((uintptr_t)(pde) & ~0xFFF));
}

int page_ref(struct Page *page)
{
    return page->ref;
}

void set_page_ref(struct Page *page, int val)
{
    page->ref = val;
}

int page_ref_inc(struct Page *page)
{
    page->ref += 1;
    return page->ref;
}

int page_ref_dec(struct Page *page)
{
    page->ref -= 1;
    return page->ref;
}

pte_t * vpt = (pte_t *)(0xFAC00000);
pde_t * vpd = (pde_t *)((uintptr_t)((((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)) << 22 | (((((uintptr_t)(0xFAC00000)) >> 22) & 0x3FF)) << 12 | (0)));





struct pmm_manager
{
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


void init_pmm_manager(void)
{
    load_default_pmm_manager();
    pmm_manager = &default_pmm_manager;
    printf("memory management: %s\n", pmm_manager->name);
    call0(pmm_manager->init);
}


void init_memmap(struct Page *base, size_t n)
{
    call2(base, n, pmm_manager->init_memmap);
}


struct Page * alloc_pages(size_t n)
{
    struct Page *page=((void *)0);
    bool intr_flag;
    do
    {
        intr_flag = __intr_save();
    }
    while (0);
    {
        page = call1(n, pmm_manager->alloc_pages);
    }
    __intr_restore(intr_flag);;
    return page;
}


void free_pages(struct Page *base, size_t n)
{
    bool intr_flag;
    do
    {
        intr_flag = __intr_save();
    }
    while (0);
    {
        call2(base, n, pmm_manager->free_pages);
    }
    __intr_restore(intr_flag);;
}


size_t nr_free_pages(void)
{
    size_t ret;
    bool intr_flag;
    do
    {
        intr_flag = __intr_save();
    }
    while (0);
    {
        ret = call0(default_pmm_manager.nr_free_pages);
    }
    __intr_restore(intr_flag);;
    return ret;
}


static void page_init(void)
{
    uintptr_t freemem;
    uint32_t maxpa, begin, end;
    int i;
    maxpa = msiz();
    if (maxpa > 0x38000000)
    {
        maxpa = 0x38000000;
    }
    npage = maxpa / 4096;
    pages = (struct Page *)ROUNDUP((uint)(endbss) + 4096 + 3, 4096);
    for (i = 0; i < npage; i++)
    {
        set_bit(0, &((pages + i)->flags));
    }
    freemem = ((uintptr_t)pages + sizeof(struct Page) * npage);
    begin = freemem;
    end = maxpa;
    if (begin < end)
    {
        begin = ROUNDUP(begin, 4096);
        end = ROUNDDOWN(end, 4096);
        if (begin < end)
            init_memmap(pa2page(begin), (end - begin) / 4096);
    }
}

static void * boot_alloc_page(void)
{
    struct Page *p = alloc_pages(1);
    if (p == ((void *)0))
    {
        panic("boot_alloc_page failed.\n");
    }
    return page2pa(p);
}

static pte_t * get_pte(pde_t *pgdir, uintptr_t la, bool create)
{
    pde_t *pdep = &pgdir[((((uintptr_t)(la)) >> 22) & 0x3FF)];
    struct Page *page;
    uintptr_t pa;

    if (!(*pdep & 0x001))
    {
        if (!create || (page = alloc_pages(1)) == ((void *)0))
        {
            return ((void *)0);
        }
        set_page_ref(page, 1);
        pa = page2pa(page);
        memset(KADDR(pa), 0, 4096);
        *pdep = pa | 0x004 | 0x002 | 0x001;
    }
    return &((pte_t *)KADDR(((uintptr_t)(*pdep) & ~0xFFF)))[((((uintptr_t)(la)) >> 12) & 0x3FF)];
}

static struct Page * get_page(pde_t *pgdir, uintptr_t la, pte_t **ptep_store)
{
    pte_t *ptep = get_pte(pgdir, la, 0);
    if (ptep_store != ((void *)0))
    {
        *ptep_store = ptep;
    }
    if (ptep != ((void *)0) && *ptep & 0x001)
    {
        return pte2page(*ptep);
    }
    return ((void *)0);
}

tlb_clear_enable = 0;

void tlb_invalidate(pde_t *pgdir, uintptr_t la)
{
    if (tlb_clear_enable)
        spage(1);
}

void
page_remove_pte(pde_t *pgdir, uintptr_t la, pte_t *ptep)
{

    struct Page * page;
    if (*ptep & 0x001)
    {
        page = pte2page(*ptep);
        if (page_ref_dec(page) == 0)
        {
            free_pages(page, 1);
        }
        *ptep = 0;
        tlb_invalidate(pgdir, la);
    }
}

void
page_remove(pde_t *pgdir, uintptr_t la)
{
    pte_t *ptep = get_pte(pgdir, la, 0);
    if (ptep != ((void *)0))
    {
        page_remove_pte(pgdir, la, ptep);
    }
}

int
page_insert(pde_t *pgdir, struct Page *page, uintptr_t la, uint32_t perm)
{
    pte_t *ptep = get_pte(pgdir, la, 1);
    struct Page *p;
    if (ptep == ((void *)0))
    {

    }

    page_ref_inc(page);
    if (*ptep & 0x001)
    {
        p = pte2page(*ptep);
        if (p == page)
        {
            page_ref_dec(page);
        }
        else
        {
            page_remove_pte(pgdir, la, ptep);
        }
    }
    *ptep = page2pa(page) | 0x001 | perm;
    tlb_invalidate(pgdir, la);
    return 0;
}

static void
check_alloc_page(void)
{
    call0(pmm_manager->check);
    printf("check_alloc_page() succeeded!\n");
}

static void
check_pgdir(void)
{
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

static void
boot_map_segment(pde_t *pgdir, uintptr_t la, size_t size, uintptr_t pa, uint32_t perm)
{
    size_t n = ROUNDUP(size + (((uintptr_t)(la)) & 0xFFF), 4096) / 4096;
    pte_t * ptep;
    assert((((uintptr_t)(la)) & 0xFFF) == (((uintptr_t)(pa)) & 0xFFF));
    la = ROUNDDOWN(la, 4096);
    pa = ROUNDDOWN(pa, 4096);
    for (; n > 0; n--, la += 4096, pa += 4096)
    {
        ptep = get_pte(pgdir, la, 1);
        assert(ptep != ((void *)0));
        *ptep = pa | 0x001 | perm;
    }
}

static void
check_boot_pgdir(void)
{
    pte_t *ptep;
    int i;
    struct Page *p;
    char *str = "ucore: Hello world!!";

    for (i = 0; i < npage; i += 4096)
    {
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
perm2str(int perm)
{
    char str[4];
    str[0] = (perm & 0x004) ? 'u' : '-';
    str[1] = 'r';
    str[2] = (perm & 0x002) ? 'w' : '-';
    str[3] = '\0';
    return str;
}

static int
get_pgtable_items(size_t left, size_t right, size_t start, uintptr_t *table, size_t *left_store, size_t *right_store)
{
    int perm;
    if (start >= right)
    {
        return 0;
    }
    while (start < right && !(table[start] & 0x001))
    {
        start++;
    }
    if (start < right)
    {
        if (left_store != ((void *)0))
        {
            *left_store = start;
        }
        perm = (table[start++] & (0x004 | 0x002 | 0x001));
        while (start < right && (table[start] & (0x004 | 0x002 | 0x001)) == perm)
        {
            start++;
        }
        if (right_store != ((void *)0))
        {
            *right_store = start;
        }
        return perm;
    }
    return 0;
}

uint tmp;
free_area_t free_area;


void print_pgdir(void)
{
    size_t left, right = 0, perm;
    size_t l, r;

    printf("-------------------- BEGIN --------------------\n");
    while ((perm = get_pgtable_items(0, 1024, right, vpd, &left, &right)) != 0)
    {
        printf("PDE(%x) %x-%x %x ", right - left,
               left * (4096 * 1024), right * (4096 * 1024), (right - left) * (4096 * 1024));
        printf("%s\n", perm2str(perm));
        r = left * 1024;
        while ((perm = get_pgtable_items(left * 1024, right * 1024, r, vpt, &l, &r)) != 0)
        {
            printf("  |-- PTE(%x) %x-%x %x ", r - l,
                   l * 4096, r * 4096, (r - l) * 4096);
            printf("%s\n", perm2str(perm));
        }
    }
    printf("--------------------- END ---------------------\n");
}

void check_and_return(void)
{
    list_entry_t *head = &free_area.free_list;

    boot_pgdir = (uint)(boot_pgdir) + 0xC0000000;
    boot_pgdir[0] = 0;

    load_default_pmm_manager();
    pmm_manager = &default_pmm_manager;

    pages = (uint)(pages) + 0xC0000000;
    while (1)
    {
        head->prev = (uint)(head->prev) + 0xC0000000;
        head->next = (uint)(head->next) + 0xC0000000;
        if (head->next == &free_area.free_list) break;
        head = list_next(head);
    }

    check_boot_pgdir();
    print_pgdir();

}

uint getsp()
{
    asm (LEA, 8);
}

static char kstack[4096];

void pmm_init(void)
{
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

static void
default_init(void)
{
    list_init(&(free_area.free_list));
    (free_area.nr_free) = 0;
}

static void
default_init_memmap(struct Page *base, size_t n)
{
    struct Page *p;
    assert(n > 0);
    p = base;
    for (; p != base + n; p++)
    {
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

static struct Page * default_alloc_pages(size_t n)
{
    list_entry_t *le, *len;
    struct Page *p, *pp, *ppp;
    int i;
    assert(n > 0);
    if (n > (free_area.nr_free))
    {
        return ((void *)0);
    }
    le = &(free_area.free_list);
    while((le=list_next(le)) != &(free_area.free_list))
    {
        p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
        if(p->property >= n)
        {
            for(i=0; i<n; i++)
            {
                len = list_next(le);
                pp = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
                set_bit(0, &((pp)->flags));
                clear_bit(1, &((pp)->flags));
                list_del(le);
                le = len;
            }
            if(p->property>n)
            {
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

static void default_free_pages(struct Page *base, size_t n)
{
    list_entry_t *le;
    struct Page * p;
    assert(n > 0);
    assert(test_bit(0, &((base)->flags)));

    le = &(free_area.free_list);
    while((le=list_next(le)) != &(free_area.free_list))
    {
        p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
        if(p>base)
        {
            break;
        }
    }

    for(p=base; p<base+n; p++)
    {
        list_add_before(le, &(p->page_link));
    }
    base->flags = 0;
    set_page_ref(base, 0);
    clear_bit(1, &((base)->flags));
    set_bit(1, &((base)->flags));
    base->property = n;

    p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
    if( base+n == p )
    {
        base->property += p->property;
        p->property = 0;
    }
    le = list_prev(&(base->page_link));
    p = ((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link))));
    if(le!=&(free_area.free_list) && p==base-1)
    {
        while(le!=&(free_area.free_list))
        {
            if(p->property)
            {
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

static size_t default_nr_free_pages(void)
{
    return (free_area.nr_free);
}

void basic_check()
{
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

void default_check(void)
{
    struct Page * p0, *p1, *p2, *p;
    struct list_entry *le = &(free_area.free_list);
    struct list_entry free_list_store;
    uint nr_free_store;
    int count, total;
    count = 0;
    total = 0;
    while ((le = list_next(le)) != &(free_area.free_list))
    {
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
    while ((le = list_next(le)) != &(free_area.free_list))
    {
        p = KADDR(((struct Page *)((char *)((le)) - ((size_t)(&((struct Page *)0)->page_link)))));
        count--, total -= p->property;
    }
    assert(count == 0);
    assert(total == 0);
}

void load_default_pmm_manager()
{
    default_pmm_manager.name = "default_pmm_manager";
    default_pmm_manager.init = default_init;
    default_pmm_manager.init_memmap = default_init_memmap;
    default_pmm_manager.alloc_pages = default_alloc_pages;
    default_pmm_manager.free_pages = default_free_pages;
    default_pmm_manager.nr_free_pages = default_nr_free_pages;
    default_pmm_manager.check = default_check;
}

void kern_init()
{
    printf("(THU.CST) ucore os is running ...\n");
    pmm_init();
    idt_init();
    stmr(128*1024*1000);
    asm (STI);
    while (1)
    {
    }
}

main()
{
    int *ksp;

    static int bss;
    endbss = &bss;

    ksp = ((uint)kstack + sizeof(kstack) - 8) & -8;
    asm (LL, 4);
    asm (SSP);

    kern_init();

    asm (HALT);
}
