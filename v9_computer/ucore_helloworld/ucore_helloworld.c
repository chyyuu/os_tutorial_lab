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
typedef int int32_t;
typedef uint uint32_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
typedef uintptr_t size_t;

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

uint msiz()
{
    asm(MSIZ);
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

static char kstack[4096];

void kern_init()
{
    printf("(THU.CST) ucore os is running ...\n");
    printf("Hello World\n");
    while (1)
    {
    }
}

uint endbss;
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
