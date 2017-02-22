# v9 Computer （v9计算机）

## v9 Computer概述
v9 computer是一个简化的计算机系统，包含一个简化的32-bit RISC CPU--v9，一块ram内存，时钟/键盘/屏幕三种外设。可在此基础上编写操作系统，编译器等，用于简化操作系统教学，原理讲解，实验和练习．

## v9  CPU概述

v9 cpu是一个简化的32-bit RISC CPU，只包含了支持操作系统教学的最基本的组成部分：ALU，其中有通用寄存器（用于访存和计算）和算术/逻辑运算单元。CU（Control Unit），其中涉及栈寄存器（sp），程序计数器寄存器（pc），状态寄存器flags等，也涉及与执行异常和外设中断相关的现场状态缓存和跳转机制等。还包括与内存访问相关的TLB(Translation Lookaside Buffer)，其中涉及对在内存中建立的页表的缓冲内容。

对于计算机原理中讲到的cache, bus, disk等没有建立相应的内容，其原因是对于支持OS基本功能，这些硬件内容可以暂时忽略。

## v9  CPU寄存器
总共有 9 个寄存器,其中 7 个为 32 位,2 个为 64 位(浮点寄存器)。本文档只针对 CPU
进行描述,对于相关的硬件配套外设(例如中断控制器等)在此不做介绍。其中：

### 通用寄存器

- a, b, c : 三个32-bit通用寄存器
- f, g 两个64-bit浮点寄存器,是用来进行各种指令操作的


### 控制寄存器

- sp 为当前栈底指针，按64-bit(8字节)对齐
   - usp: user stack，在用户态,sp是usp
   - ssp: kernel stack，在内核态，sp是ksp,用户态应用不可见ssp
- pc 为32-bit程序计数器（指向下一条指令），按32-bit(4字节)对齐，其指向的内存内容（即具体的指令值）会放到ir中，给CPU解码并执行
- tsp 为栈顶指针(本 CPU 的栈是从顶部往底部增长),按64-bit(8字节)对齐


### 状态寄存器

- flags 为内部状态寄存器(包括当前的运行模式,是否中断使能,是否有自陷,以及是否使用虚拟地址等)，可通过特定指令访问相关bit



#### flags寄存器标志位
- user   : 1; 用户态或内核态(user mode or kernel mode.)
- iena   : 1; 中断使能/失效 (interrupt flag.)
- trap   : 1; 异常/错误编码 (fault code.)
- paging : 1; 页模式使能/失效（ virtual memory enabled or not.）


####  内核态与用户态

v9 CPU提供user mode/kernel mode两种级别的运行模式。内核态kernel mode的特权级别最高，用户态user mode的特权级别最低。其中v9 CPU的内核态运行模式是预留给操作系统使用的，可确保操作系统不受任何的限制地自由访问任何有效内存地址，执行特权（系统）指令，对外设直接访问。而v9 CPU的用户态运行模式是预留给给普通的应用程序使用的，运行于用户态的代码会受到更多的v9 CPU的安全保护机制的检查，它们只能操作系统约束的虚拟内存空间，不能执行特权（系统）指令，不能直接访问物理内存空间和外设。

## v9  CPU指令集

总共有209条指令,一条指令大小为32bit, 具体的命令编码（opcode）在指令的低8位,高24位为操作数(operand）或立即数（immediate )。
但很多指令只是某一类指令的多种形式，很容易触类旁通。

整体来看,指令分为如下几类:

- 运算指令：如ADD, SUB等
- 跳转指令：如JMP, JSR，LEV等
- 访存（Load/Store）指令：如LL, LBL, SL等
- 系统命令:如HALT, RTI, IDLE，SSP, USP,IVEC, PDIR，目的是为了操作系统设计
- 扩展函数库命令：如MCPY/MCMP/MCHR/MSET, MATH类，NET类，目的为了简化编译器设计

按照指令对条件的需求，指令可分类如下：

- HALT, RTI, IDLE等，不需要立即数来参与，也不需要读取或修改寄存器值。
- ADD, MCPY，PSHA等，不需要立即数来参与，但需要读取或修改寄存器值。
- ADDI, S/L系列，B系列等，需要立即数和寄存器来参与。



v9-cpu的指令集如下：


### 显示格式
```
指令名字　  指令编码　　指令含义
```

### 指令编码格式
```
0xiiiiiicc OR 0x......cc
i --> 操作数　immediate OR operand0 (简称imme)
. --> CPU解码不会用到高24位
c --> 指令编码
```

### 系统和扩展类指令--system extended
```
//系统停止
HALT	0xiiiiii00	halt system

//可用于分配函数中的局部变量
ENT		0xiiiiii01  sp += imme

//用于函数返回
LEV		0xiiiiii02  pc =  *sp,	sp += imme+8

//绝对跳转指令
JMP		0xiiiiii03	pc += imme
JMPI	0xiiiiii04	pc += imme+ra>>2

//用于函数调用，把函数调用返回地址保存在sp指向的栈中，跳到目的地址
JSR		0xiiiiii05	*sp = pc,	sp -= 8,	pc += imme
JSRA	0x......06	*sp = pc,	sp -= 8,	pc += ra

//保存sp地址，pc地址等到ra寄存器中
LEA		0xiiiiii07	ra = sp+imme
LEAG	0xiiiiii08	ra = pc+imme
CYC		0x......09	ra = current cycle related with pc


//内存块操作
// MCPY： while (c) { *a = *b; a++; b++; c--; }
MCPY	0x......0a	memcpy(ra, rb, rc)   
// MCMP： 
// for (;;) { 
//   if (!c) {
//      a = 0; break; 
//   } 
//   if (*b != *a) { 
//     a = *b - *a; 
//     b += c; 
//     c = 0; 
//     break; 
//   } 
//   a++; b++; c--; 
// } /* end for */
MCMP	0x......0b	memcmp(ra, rb, rc) 
// MCHR: 
// for (;;) { 
//    if (!c) { 
//         a = 0; break; 
//    } 
//    if (*a == b) { 
//         c = 0; break; 
//    } 
//    a++; c--; 
// } /* end for */
MCHR	0x......0c  memchr(ra, rb, rc)
//MSET: while (c) { *a = b; a++; c--; }
MSET	0x......0d	memset(ra, rb, rc)
```

### 访存指令--load/store

#### load to register a

```
LL		0xiiiiii0e	ra = *(*uint)  (sp+imme)
LLS		0xiiiiii0f	ra = *(*short) (sp+imme)
LLH		0xiiiiii10	ra = *(*ushort)(sp+imme)
LLC		0xiiiiii11	ra = *(*char)  (sp+imme)
LLB		0xiiiiii12	ra = *(*uchar) (sp+imme)
LLD		0xiiiiii13	ra = *(*double)(sp+imme)
LLF		0xiiiiii14	ra = *(*float) (sp+imme)
LG		0xiiiiii15	ra = *(*uint)  (pc+imme)
LGS		0xiiiiii16	ra = *(*short) (pc+imme)
LGH		0xiiiiii17	ra = *(*ushort)(pc+imme)
LGC		0xiiiiii18	ra = *(*char)  (pc+imme)
LGB		0xiiiiii19	ra = *(*uchar) (pc+imme)
LGD		0xiiiiii1a	ra = *(*double)(pc+imme)
LGF		0xiiiiii1b	ra = *(*float) (pc+imme)
LX		0xiiiiii1c	ra = *(*uint)  global_addr(ra+imme)
LXS		0xiiiiii1d	ra = *(*short) global_addr(ra+imme)
LXH		0xiiiiii1e	ra = *(*ushort)global_addr(ra+imme)
LXC		0xiiiiii1f	ra = *(*char)  global_addr(ra+imme)
LXB		0xiiiiii20	ra = *(*uchar) global_addr(ra+imme)
LXD		0xiiiiii21	ra = *(*double)global_addr(ra+imme)
LXF		0xiiiiii22	ra = *(*float) global_addr(ra+imme)
LI		0xiiiiii23	ra = imme
LHI		0xiiiiii24	ra = (ra<<24)|(imme>>8)
LIF		0xiiiiii25	rf = double(imme)
```

#### load to register b
```
LBL		0xiiiiii26	rb = *(*uint)  (sp+imme)
LBLS	0xiiiiii27	rb = *(*short) (sp+imme)
LBLH	0xiiiiii28	rb = *(*ushort)(sp+imme)
LBLC	0xiiiiii29	rb = *(*char)  (sp+imme)
LBLB	0xiiiiii2a	rb = *(*uchar) (sp+imme)
LBLD	0xiiiiii2b	rb = *(*double)(sp+imme)
LBLF	0xiiiiii2c	rb = *(*float) (sp+imme)
LBG		0xiiiiii2d	rb = *(*uint)  (pc+imme)
LBGS	0xiiiiii2e	rb = *(*short) (pc+imme)
LBGH	0xiiiiii2f	rb = *(*ushort)(pc+imme)
LBGC	0xiiiiii30	rb = *(*char)  (pc+imme)
LBGB	0xiiiiii31	rb = *(*uchar) (pc+imme)
LBGD	0xiiiiii32	rb = *(*double)(pc+imme)
LBGF	0xiiiiii33	rb = *(*float) (pc+imme)
LBX		0xiiiiii34	rb = *(*uint)  global_addr(rb+imme)
LBXS	0xiiiiii35	rb = *(*short) global_addr(rb+imme)
LBXH	0xiiiiii36	rb = *(*ushort)global_addr(rb+imme)
LBXC	0xiiiiii37	rb = *(*char)  global_addr(rb+imme)
LBXB	0xiiiiii38	rb = *(*uchar) global_addr(rb+imme)
LBXD	0xiiiiii39	rb = *(*double)global_addr(rb+imme)
LBXF	0xiiiiii3a	rb = *(*float) global_addr(rb+imme)
LBI		0xiiiiii3b  rb = imme
LBHI	0xiiiiii3c  rb = (rb<<24)|(imme>>8)
LBIF	0xiiiiii3d  rb = double(imme)
LBA		0x......3e  rb = ra
LBAD    0x......3f  rg = rf
```

#### store register a to memory 
```
SL		0xiiiiii40	*(*uint)  (sp+imme) = (uint)  (ra)
SLH		0xiiiiii41	*(*ushort)(sp+imme) = (ushort)(ra)
SLB		0xiiiiii42	*(*uchar) (sp+imme) = (ushort)(ra)
SLD		0xiiiiii43	*(*double)(sp+imme) = (double)(ra)
SLF		0xiiiiii44	*(*float) (sp+imme) = (float) (ra)
SG		0xiiiiii45	*(*uint)  (pc+imme) = (uint)  (ra)
SGH		0xiiiiii46	*(*ushort)(pc+imme) = (ushort)(ra)
SGB		0xiiiiii47	*(*uchar) (pc+imme) = (ushort)(ra)
SGD		0xiiiiii48	*(*double)(pc+imme) = (double)(ra)
SGF		0xiiiiii49	*(*float) (pc+imme) = (float) (ra)
SX		0xiiiiii4a  *(*uint)  global_addr(rb+imme) = (uint)  (ra)
SXH		0xiiiiii4b	*(*ushort)global_addr(rb+imme) = (ushort)(ra)
SXB		0xiiiiii4c	*(*uchar) global_addr(rb+imme) = (ushort)(ra)
SXD		0xiiiiii4d	*(*double)global_addr(rb+imme) = (double)(ra)
SXF		0xiiiiii4e	*(*float) global_addr(rb+imme) = (float) (ra)
```

### 运算指令--arithmetic
```
ADDF	0x......4f	rf = rf+rg
SUBF	0x......50	rf = rf-rg
MULF	0x......51	rf = rf*rg
DIVF	0x......52	rf = rf/rg
ADD		0x......53  ra = ra+rb
ADDI	0xiiiiii54	ra = ra+imme
ADDL	0xiiiiii55	ra = ra+(*(int*)(sp+imme))
SUB		0x......56  ra = ra-rb
SUBI	0xiiiiii57	ra = ra-imme
SUBL	0xiiiiii58	ra = ra-(sp+imme)
MUL		0x......59	ra = (int)(ra)*(int)(rb)
MULI	0xiiiiii5a	ra = (int)(ra)*(int)(imme)
MULL	0xiiiiii5b	ra = (int)(ra)*(*(int*)(sp+imme))
DIV		0x......5c	ra = (int)(ra)/(int)(rb)
DIVI	0xiiiiii5d	ra = (int)(ra)/(int)(imme)
DIVL	0xiiiiii5e	ra = (int)(ra)/(*(int*)(sp+imme))
DVU		0x......5f	ra = (uint)(ra)/(uint)(rb)
DVUI	0xiiiiii60	ra = (uint)(ra)/(uint)(imme)
DVUL	0xiiiiii61	ra = (uint)(ra)/(*(uint*)(sp+imme))
MOD		0x......62	ra = (int)(ra)%(int)(rb)
MODI	0xiiiiii63	ra = (int)(ra)%(int)(imme)
MODL	0xiiiiii64	ra = (int)(ra)%(*(int*)(sp+imme))
MDU		0x......65	ra = (uint)(ra)%(uint)(rb)
MDUI	0xiiiiii66	ra = (uint)(ra)%(uint)(imme)
MDUL	0xiiiiii67	ra = (uint)(ra)%(*(uint*)(sp+imme))
AND		0x......68	ra = ra&rb
ANDI	0xiiiiii69	ra = ra&imme
ANDL	0xiiiiii6a	ra = ra&(*(int*)(sp+imme))
OR		0x......6b	ra = ra|rb
ORI		0xiiiiii6c	ra = ra|imme
ORL		0xiiiiii6d	ra = ra|(*(int*)(sp+imme))
XOR		0x......6e	ra = ra^rb
XORI	0xiiiiii6f	ra = ra^imme
XORL	0xiiiiii70	ra = ra^(*(int*)(sp+imme))
SHL		0x......71	ra = ra<<(uint)(rb)
SHLI	0xiiiiii72	ra = ra<<(uint)(imme)
SHLL	0xiiiiii73	ra = ra<<(*(uint*)(sp+imme))
SHR		0x......74	ra = (int)(ra)>>(uint)(rb)
SHRI	0xiiiiii75	ra = (int)(ra)>>(uint)(imme)
SHRL	0xiiiiii76	ra = (int)(ra)>>(*(uint*)(sp+imme))
SRU		0x......77	ra = (uint)(ra)>>(uint)(rb)
SRUI	0xiiiiii78	ra = (uint)(ra)>>(uint)(imme)
SRUL	0xiiiiii79	ra = (uint)(ra)>>(*(uint*)(sp+imme))
EQ		0x......7a  ra = (ra == rb)
EQF		0x......7b	ra = (rf == rg)
NE		0x......7c	ra = (ra != rb)
NEF		0x......7d	ra = (rf != rg)
LT		0x......7e	ra = ((int)a < (int)b)
LTU		0x......7f	ra = ((uint)a < (uint)b)
LTF		0x......80	ra = (f < g)
GE		0x......81	ra = ((int)a > (int)b)
GEU		0x......82	ra = ((uint)a > (uint)b)
GEF		0x......83	ra = (f > g)
```
### 条件跳转指令--cond branch
```
BZ		0xiiiiii84	if (ra == 0)  pc = pc+imme
BZF		0xiiiiii85	if (rf == 0)  pc = pc+imme
BNZ		0xiiiiii86  if (ra != 0)  pc = pc+imme
BNZF	0xiiiiii87	if (rf != 0)  pc = pc+imme
BE		0xiiiiii88	if (ra == rb) pc = pc+imme
BEF		0xiiiiii89	if (rf == rg) pc = pc+imme
BNE		0xiiiiii8a  if (ra != rb) pc = pc+imme
BNEF	0xiiiiii8b	if (rf != rg) pc = pc+imme
BLT		0xiiiiii8c	if ((int)a < (int)b) pc = pc+imme
BLTU	0xiiiiii8d	if ((uint)a < (uint)b) pc = pc+imme
BLTF	0xiiiiii8e	if (f < g) pc = pc+imme
BGE		0xiiiiii8f	if ((int)a < (int)b) pc = pc+imme
BGEU	0xiiiiii90	if ((uint)a < (uint)b) pc = pc+imme
BGEF	0xiiiiii91	if (f < g) pc = pc+imme
```

### 格式转换指令--conversion
```
CID		0x......92  f = (double)((int)a)
CUD		0x......93	f = (double)((uint)a)
CDI		0x......94	a = (int)(f)
CDU		0x......95	a = (uint)(f)
```

#### misc
```
//屏蔽中断
CLI		0x......96	a = iena,	iena = 0
//使能中断
STI		0x......97	if generated by hardware: set trap, and process the interrupt; else: iena = 1
//中断返回
RTI		0x......98	return from interrupt, POP fault code, pc, sp,  if fault code== USER, then switch to user mode; if has pending interrupt, process the interrupt
//IO读写指令
BIN		0x......99	a = kbchar,	kbchar is the value from outside io(e.g. keyboard)
BOUT	0x......9a	a = write(a, &b, 1);
//空操作指令
NOP		0x......9b	no operation.

SSP		0x......9c	ksp = a -- ksp is kernel sp
PSHA	0x......9d	sp -= 8, *sp = a
PSHI	0x......9e	sp -= 8, *sp = imme
PSHF	0x......9f	sp -= 8, *(double *)sp = f
PSHB	0x......a0	sp -= 8, *sp = b
POPB	0x......a1	b = *sp, sp += 8
POPF	0x......a2	f = *(double *)sp, sp += 8
POPA	0x......a3	a = *sp, sp += 8

//设置中断向量起始地址
IVEC	0x......a4	ivec = a -- set interrupt vector by a
//设置页目录表起始地址
PDIR	0x......a5	pdir = a -- set page directory physical memory by a
//设置页机制使能/屏蔽
SPAG	0x......a6	paging = a -- enable/disable virtual memory feature by a
//设置时钟到时值
TIME	0xiiiiiia7	if operand0 is 0: timeout = a -- set current timeout from a; else: printk("timer%d=%u timeout=%u", operand0, timer, timeout)
//获取访问异常的地址
LVAD	0x......a8	a = vadr -- vadr is bad virtual address
//陷入指令，常用于系统调用
TRAP	0x......a9	trap = FSYS
//获取用户态的sp值
LUSP	0x......aa	a = usp	
//设置用户态的sp值
SUSP	0x......ab	usp = a -- usp is user stack pointer
LCL		0xiiiiiiac	c = *(uint *)(sp + imme)
LCA		0x......ad	c = a
PSHC	0x......ae	sp -= 8, *sp = c
POPC	0x......af	c = *sp, sp += 8
//获取内存大小
MSIZ 	0x......b0	a = memsz -- move physical memory to a.

PSHG	0x......b1	sp -= 8, *sp = g
POPG	0x......b2	g = *sp, sp += 8

NET1	0xiiiiiib3	No use
NET2	0xiiiiiib4	No use
NET3	0xiiiiiib5	No use
NET4	0xiiiiiib6	No use
NET5	0xiiiiiib7	No use
NET6	0xiiiiiib8	No use
NET7	0xiiiiiib9	No use
NET8	0xiiiiiiba	No use
NET9	0xiiiiiibb	No use
```

#### math 
```
POW		0x......bc	rf = power(rf, rg)
ATN2	0x......bd	rf = atan2(rf, rg)
FABS	0x......be	rf = fabs(rf, rg)
ATAN	0x......c0	rf = atan(rf)
LOG 	0x......c1	rf = log(rf)
LOGT	0x......c2	rf = log10(rf)
EXP 	0x......c3	rf = exp(rf)
FLOR	0x......c4	rf = floor(rf)
CEIL	0x......c5	rf = ceil(rf)
HYPO	0x......c6	rf = hypo(rf, rg)
SIN 	0x......c7	rf = sin(rf)
COS 	0x......c8	rf = cos(rf)
TAN 	0x......c9	rf = tan(rf)
ASIN	0x......ca	rf = asin(rf)
ACOS	0x......cb	rf = acos(rf)
SINH	0x......cc	rf = sinh(rf)
COSH	0x......cd	rf = cosh(rf)
TANH	0x......ce	rf = tanh(rf)
SQRT	0x......cf	rf = sqrt(rf)
FMOD	0x......d0	rf = fmod(rf, rg)
```

#### cpu idle
```
IDLE 	0x......d1	response hardware interrupt (include timer).
```



## v9 cpu TLB(页表缓冲)

TLB -- （Translation Lookaside Buffer），也称为快表，旁路快表缓冲，页表缓冲，地址变换高速缓存，页转换表等。TLB位于v9 CPU中。当v9 CPU要访问一个虚拟地址时，v9 CPU会首先根据虚拟地址的高20位在TLB中查找。如果是表中没有相应的表项，称为TLB miss。出现TLB miss后，v9 CPU需要以虚拟地址的内容为索引，通过访问v9 Computer中的RAM内存中的页表（由OS建立），查找并计算出虚拟地址相应的物理地址(页帧号)，并把此物理地址（页帧号）被存放在一个TLB表项中。这样v9 CPU后续对同一虚拟地址范围（只比较地址的高20位部分，即页号，page number）的访问，就可直接以页号为索引，从v9 CPU内的TLB表项中获取物理地址即可，称为TLB hit。在v9 CPU的TLB中，设置了4个1MB大小页转换表（page translation buffer array）

- kernel read page translation table
- kernel write page translation table
- user read page translation table
- user write page translation table

有两个指针tr/tw, tw指向内核态或用户态的read/write　page translation table．
```
tr/tw[page number]=phy page number //页帧号
```
还有一个tpage buffer array, 保存了所有tr/tw中的虚页号，这些虚页号是tr/tw数组中的index 
```
tpage[tpages++] = v //v是page number
```

## 计算机系统内存

v9 Computer中包括一块连续的物理内存（RAM），与v9 CPU直接相连，缺省内存大小为128MB，可以通过启动参数"-m XXX"，设置为XXX MB大小．其物理内存地址是连续的，从0~XXX。

### 分页机制

#### 相关操作
- PDIR --> page_directory base addr PTBR <--reg a
- SPAG --> enable_paging = a

#### 页表格式

```
                                                              PAGE FRAME
              +-----------+-----------+----------+         +---------------+
              | DIR 10bit |PAGE 10bit |OFF 12bit |         |               |
              +-----+-----+-----+-----+-----+----+         |               |
                    |           |           |              |               |
      +-------------+           |           +------------->|    PHYSICAL   |
      |                         |                          |    ADDRESS    |
      |   PAGE DIRECTORY        |      PAGE TABLE          |               |
      |  +---------------+      |   +---------------+      |               |
      |  |               |      |   |               |      +---------------+
      |  |               |      |   |---------------|              ^
      |  |               |      +-->| PG TBL ENTRY  |--------------+
      |  |---------------|          |---------------|
      +->|   DIR ENTRY   |--+       |               |
         |---------------|  |       |               |
         |               |  |       |               |
         +---------------+  |       +---------------+
                 ^          |               ^
+-------+        |          +---------------+
| PTBR  |--------+
+-------+
```
#### 页目录表项，页表项的属性
```
  PTE_P = 0x001, // present
  PTE_W = 0x002, // writeable
  PTE_U = 0x004, // user
  PTE_A = 0x020, // accessed
  PTE_D = 0x040, // dirty
```

#### 页目录表项，页表项的组成
```
DIR_ENTRY=　[高20位：二级页表地址的高20位（4KB对齐）][低12位属性]　

PT_ENTRY　=　[高20位：物理页帧地址的高20位（4KB对齐）][低12位属性]　
```

### 页访问异常
```
  FMEM,   // bad physical address
  FIPAGE, // page fault on opcode fetch
  FWPAGE, // page fault on write
  FRPAGE, // page fault on read
  USER=16 // user mode exception
```

通过LVAD指令可获得访问异常的虚地址并赋值给寄存器a　

###  刷新TLB表

v9的页大小是`4K(2^12)`，页表条目数是`1M(2^20)`，并分为两级。包括核心态读写页表与用户态读写页表（由页表项的标志位确定）。v9中有4个数组指针`trk/twk`和`tru/twu`， 分别指向内核态和用户态的读/写`TLB`，对地址进行虚实转换处理：

    tr/tw[page number]=phy page number //页帧号
tpages是一个array,保存了所有 tr/tw 中的虚页号,这些虚页号是tr/tw 数组中的 index。

当v9 cpu需要刷新整个TLB时，将执行如下过程：
​    

    flush()
    {
      uint v; 
      while (tpages) {
        v = tpage[--tpages];
        trk[v] = twk[v] = tru[v] = twu[v] = 0;    
      }
    }

### 设置TLB表项

当某次访问虚存地址，v9发现有对应的页表项，但没有对应的TLB项时，需要设置TLB表项，将执行如下过程：

```
uint setpage(uint v, uint p, uint writable, uint userable)
{
  if (p >= memsz) { trap = FMEM; vadr = v; return 0; } //物理地址p超过范围，则产生异常
  p = ((v ^ (mem + p)) & -4096) + 1; //更新p值（v和mem+p的位异或），这样将来通过p^v可以还原出mem+p(即物理地址)
  if (!trk[v >>= 12]) {              //如果v高20位为index的trk数组项为0,则添加一个TLB项目
    if (tpages >= TPAGES) flush();
    tpage[tpages++] = v;
  }
  trk[v] = p;
  twk[v] = writable ? p : 0;
  tru[v] = userable ? p : 0;
  twu[v] = (userable && writable) ? p : 0;
  return p;
}
```

### 硬件缓存TLB表项的过程

在访问`虚地址v`时，取其高20位作为index访问`tr/tw`数组（即访问v9 CPU内部的TLB），若不存在则调用`rlook/wlook`（即硬件访问页表项，并缓存到TLB中）

    if (!(p = tr[(v >> 12]) && !(p = rlook(v)))
`rlook`:（读访问中缓存TLB；写访问中缓存TLB的过程类似）

    uint rlook(uint v)
    {
      uint pde, *ppde, pte, *ppte, q, userable;
      if (!paging) return setpage(v, v, 1, 1); //未开启虚拟内存
      pde = *(ppde = (uint *)(pdir + (v>>22<<2))); // 取虚地址高10位作为page directory entry（第一级页表，即页目录表）
      if (pde & PTE_P) { //如果页目录项有效
        if (!(pde & PTE_A)) *ppde = pde | PTE_A; //置访问位(Access)
        if (pde >= memsz) { trap = FMEM; vadr = v; return 0; } //产生内存访问异常
        //取中间10位（第二级页表）
        pte = *(ppte = (uint *)(mem + (pde & -4096) + ((v >> 10) & 0xffc))); 
        if ((pte & PTE_P) && ((userable = (q = pte & pde) & PTE_U) || !user)) {
          if (!(pte & PTE_A)) *ppte = pte | PTE_A; //置访问位(Access)
          return setpage(v, pte, (pte & PTE_D) && (q & PTE_W), userable); //缓存TLB表项
        }
      }
      //产生内存页读访问异常
      trap = FRPAGE;
      vadr = v;
      return 0;
    }
## 计算机外设

v9 Computer只包含最基本的外设：timer，keyboard/串口, screen/串口，支持中断响应和相关的IO操作。

### 写外设（类似屏幕/串口写）的步骤
- 1 --> a          //把输出的字符个数给寄存器a
- 'char' --> b   //把字符的内容给寄存器b
- BOUT　　  //如果在内核态，在终端上输出一个字符'char', 1-->a，如果在用户态，产生FPRIV异常

### 读外设（类似键盘/串口读）的步骤
- BIN  //如果在内核态，kchar -->a  kchar是模拟器定期轮询获得的一个终端输入字符

### 中断响应

如果iena(中断使能)，则在获得一个终端输入字符后，会产生FKEYBD中断；时钟到了timeout值会产生时钟中断。

### 设置timer的timeout
- val --> a  //把timerout值给寄存器a
- TIME // 如果在内核态，设置timer的timeout阈值为寄存器a的值; 如果在用户态，产生FPRIV异常


> 注意： timeout置零时不产生时钟中断。


## 中断/异常机制
### 一些变量的含义：
- ivec: 中断向量的地址

### 中断/异常类型
```
- FMEM,          // bad physical address 
- FTIMER,        // timer interrupt
- FKEYBD,        // keyboard interrupt
- FPRIV,         // privileged instruction
- FINST,         // illegal instruction
- FSYS,          // software trap
- FARITH,        // arithmetic trap
- FIPAGE,        // page fault on opcode fetch
- FWPAGE,        // page fault on write
- FRPAGE,        // page fault on read
- USER 　　　　   // user mode exception 
```

### 设置中断向量
- val --> a
- IVEC // 如果在内核态，设置中断向量的地址ivec为a; 如果在用户态，产生FPRIV异常

### 中断/异常产生的处理
对于中断，CPU在执行指令前进行判断，看是否有中断：

- 如果终端产生了键盘输入，且iena=1，则ipend |= FKEYBD，0-->iena
- 如果timer产生了timeout，且iena=1，则ipend |= FTIMER，0-->iena

对于异常，即在执行某指令时出现了异常（非法访问内存，非法权限，算术异常等）,或者是陷入（trap）指令，则会有相应的处理

- 如果当前处于中断屏蔽状态(即iena=0)，则产生fatal错误；
- 对于异常，设置错误码为异常编码；对于陷入指令，设置错误码为FSYS

然后是统一的善后处理：

- 如果当前处于用户态(user=1)，则sp=kernel_sp, tr=kernel_tr, tw=kernel_tw,  trap|=USER
- PUSH当前的pc到当前内核栈
- PUSH 错误码(fault code)到当前内核栈
- pc跳转中断向量的地址ivec


### 时钟中断的过程

1. 通过'TIME'指令设置时钟的time out阈值（>0）

2. 通过'IVEC'指令设置到中断处理例程的起始地址

3. 通过‘STI’指令使能中断

4. 当v9 cpu执行时间超过了time out阈值，就产生中断，ipend |= FTIMER（设置中断类型），0-->iena（屏蔽中断）

5. 跳到中断处理例程进行处理

   ​

## CPU执行控制机制

### CPU跳转机制

v9包括无条件跳转和有条件跳转多种跳转令，若需要跳转，则修改pc寄存器的值，并在使能页机制的情况下：

- 若跳转后的指令与跳转前的指令在同一页表中，则直接执行下一条指令`(goto next)`。

- 若跳转后的位置与跳转前的指令不在同一页表中，则需要重新装填页表 (goto fixpc)。

CPU函数调用机制

v9包括J型指令`(JSR/JSRA)`，在函数调用时，将返回地址、调用参数等依序压栈。在函数调用和返回时再分别通过J型指令跳转到对应地址。

## 应用二进制接口（application binary interface，ABI）

ABI描述的内容包括：

- 数据类型的大小、布局和对齐;
- 调用约定（控制着函数的参数如何传送以及如何接受返回值），例如，是所有的参数都通过栈传递，还是部分参数通过寄存器传递；哪个寄存器用于哪个函数参数；通过栈传递的第一个函数参数是最先push到栈上还是最后；
- 系统调用的编码和一个应用如何向操作系统进行系统调用；
- 以及在一个完整的操作系统ABI中，执行文件的二进制格式、程序库等等。


### 基本数据表示
- char：8-bit有符号字符类型
- uchar：8-bit无符号字符类型
- short：16-bit有符号短整数类型
- ushort:16-bit无符号短整数类型
- int:32-bit有符号整数类型
- uint：32-bit无符号整数类型
- float:32-bit浮点类型
- double:64-bit浮点类型
- pointer:32-bit指针类型

### 对齐要求和字节序
上述的的数据类型，只有当自然对齐的情况，才可以用标准的v9指令直接处理。v9-cpu采用小端（Little-Endian）格式，就是低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。v9-cpu 要求栈对齐到8字节。


### 函数调用约定
####  常规参数传递
v9-cpu ABI (应用程序二进制接口)约定了用栈来传递函数调用参数，所有的参数的都是占用8字节的空间，在调用前，把参数按从右至左的顺序进行压栈。

#### 返回值传递
在返回基本数据类型的情况下，寄存器 a 约定为传递返回值。

### 执行文件的格式
执行文件由文件头和文件体组成。其中，文件头的结构为：

```
struct { 
 uint magic //0xC0DEF00D 文件的magic number 
 uint bss　 //在v9-cpu执行时没有用到
 uint entry //程序的执行入口地址
 uint flags;//程序的数据段起始地址 
} hdr;
```
> BSS是Block Started by Symbol的简称，指用来存放程序中未初始化的全局变量的一块内存区域

执行文件体有两部分组成：

- text段：紧接在文件头后，代码段（code segment/text segment）通常是指用来存放程序执行代码的一块内存区域。这部分区域的大小在程序运行前就已经确定，
- data段:紧接在代码段后，数据段（data segment）通常是指用来存放程序中已初始化的全局变量的一块内存区域。数据段属于静态内存分配。

## CPU执行过程
### 一些变量的含义：
主要集中在em.c的cpu()函数中

- a: a寄存器
- b: b寄存器
- c: c寄存器
- f: f浮点寄存器
- g: g浮点寄存器
- ir:　指令寄存器
- xpc: pc在host内存中的值
- fpc: pc在host内存中所在页的下一页的起始地址值
- tpc: pc在host内存中所在页的起始地址值
- xsp: sp在host内存中的值
- tsp: sp在host内存中所在页的起始地址值
- fsp: 辅助判断是否要经过tr/tw的分析
- ssp: 内核态的栈指针
- usp: 用户态的栈指针
- cycle: 指令执行周期计数
- xcycle:　用于判断外设中断的执行频度，和调整最终的指令执行周期计数（需进一步明确?）
- timer: 当前时钟计时（和时间时间中断有关）
- timeout: 时钟周期，当timer>timeout时，产生时钟中断 
- detla:　一次指令执行的时间，timer+=delta

 ###执行过程概述

 1. 以funcall程序为例，首先，读入执行文件的代码段＋数据段到内存的底部，并把pc放置到执行文件头中entry设置的内存位置， 设置可用sp为MEM_SZ-FS_SZ - 数据段和程序段（应该还包括BSS段）
 2. 然后从pc指向的起始地址开始执行
 3. 如果碰到异常或中断，则保存中断的地址，并跳到中断向量的地址ivec处执行

### v9加载OS和初始设置

v9模拟器模拟了一块连续的物理内存，缺省128MB。v9模拟器模拟开机后，将把OS文件内容读到它模拟的内存区域中（从零地址开始，顺序读出并写到模拟内存中）。如果有文件系统，则把文件系统（也是一个文件）放到物理内存的最高处，缺省大小为4MB。在文件系统之下是堆栈的栈顶位置，即SP的值。

```
   +---------------------------+ <--addr=128MB
   |     File System           |
   +---------------------------+ <--addr=124MB  <--SP
   |                           |
   |       ....                |
   +---------------------------+ <-- end of OS
   |         OS                |
   +---------------------------+ <-- addr=0

```



