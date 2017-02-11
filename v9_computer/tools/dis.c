#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char ops[] =
  "HALT,ENT ,LEV ,JMP ,JMPI,JSR ,JSRA,LEA ,LEAG,CYC ,MCPY,MCMP,MCHR,MSET," // system
  "LL  ,LLS ,LLH ,LLC ,LLB ,LLD ,LLF ,LG  ,LGS ,LGH ,LGC ,LGB ,LGD ,LGF ," // load a
  "LX  ,LXS ,LXH ,LXC ,LXB ,LXD ,LXF ,LI  ,LHI ,LIF ,"
  "LBL ,LBLS,LBLH,LBLC,LBLB,LBLD,LBLF,LBG ,LBGS,LBGH,LBGC,LBGB,LBGD,LBGF," // load b
  "LBX ,LBXS,LBXH,LBXC,LBXB,LBXD,LBXF,LBI ,LBHI,LBIF,LBA ,LBAD,"
  "SL  ,SLH ,SLB ,SLD ,SLF ,SG  ,SGH ,SGB ,SGD ,SGF ,"                     // store
  "SX  ,SXH ,SXB ,SXD ,SXF ,"
  "ADDF,SUBF,MULF,DIVF,"                                                   // arithmetic
  "ADD ,ADDI,ADDL,SUB ,SUBI,SUBL,MUL ,MULI,MULL,DIV ,DIVI,DIVL,"
  "DVU ,DVUI,DVUL,MOD ,MODI,MODL,MDU ,MDUI,MDUL,AND ,ANDI,ANDL,"
  "OR  ,ORI ,ORL ,XOR ,XORI,XORL,SHL ,SHLI,SHLL,SHR ,SHRI,SHRL,"
  "SRU ,SRUI,SRUL,EQ  ,EQF ,NE  ,NEF ,LT  ,LTU ,LTF ,GE  ,GEU ,GEF ,"      // logical
  "BZ  ,BZF ,BNZ ,BNZF,BE  ,BEF ,BNE ,BNEF,BLT ,BLTU,BLTF,BGE ,BGEU,BGEF," // conditional
  "CID ,CUD ,CDI ,CDU ,"                                                   // conversion
  "CLI ,STI ,RTI ,BIN ,BOUT,NOP ,SSP ,PSHA,PSHI,PSHF,PSHB,POPB,POPF,POPA," // misc
  "IVEC,PDIR,SPAG,TIME,LVAD,TRAP,LUSP,SUSP,LCL ,LCA ,PSHC,POPC,MSIZ,"
  "PSHG,POPG,NET1,NET2,NET3,NET4,NET5,NET6,NET7,NET8,NET9,"
  "POW ,ATN2,FABS,ATAN,LOG ,LOGT,EXP ,FLOR,CEIL,HYPO,SIN ,COS ,TAN ,ASIN," // math
  "ACOS,SINH,COSH,TANH,SQRT,FMOD,"
  "IDLE,";
 
struct Header {
	int magic;
	int bss;
	int entry; // the entry point of execution
	int flags; // the data segment base addr
};
struct Header hdr;

int dat[65536];
int label[65536];
bool cmt = false;

void usage() {
	printf("disasm Exec_File -o Assemble_File [-c]\n");
	printf("  -c : print comments for each instrs\n");
}

char showChar(int c) {
	if (c >= 32 && c <= 126)
		return (char)(c);
	else
		return '.';
}

int main(int argc, char** argv) {
	char *file_i, *file_o;
	int i, j;
	size_t len;
	
	if (argc<=1) {
            usage();
	        exit(1);
	}
	
	for (i = 1 ; i < argc ; i++) {
		if (!strcmp(argv[i], "-help") || !strcmp(argv[i], "-h")){
            usage();
		} else if (!strcmp(argv[i], "-c"))
			cmt = true;
		else if (!strcmp(argv[i], "-o"))
			file_o = argv[++i];
		else
			file_i = argv[i];
	}
	FILE* fi = fopen(file_i, "rb");
    if (fi==NULL) {
            printf("Can not open input file %s\n",file_i);
            exit(-1);
    }
    
	FILE* fo = fopen(file_o, "wb");
    if (fo==NULL) {
            printf("Can not open output file %s\n",file_o);
            exit(-1);
    }
        	
	len=fread(&hdr, 1, sizeof(hdr), fi);
	if(len !=  sizeof(hdr)) {
		printf("can not read hdr of %s correctly\n",file_i);
		exit(-1);
	}
	if(hdr.magic!=0xc0def00d) {
            printf("%s is not a legal v9-cpu exec file\n",file_i);
            exit(-1);
    }    	
	int d_sz = fread(&dat, 1, sizeof(dat), fi)>>2;
	int c_sz = (hdr.flags-sizeof(hdr))>>2;
	int label_sz = 0, label_ct = 0;
	for (i = 0 ; i < c_sz ; i++) {
		int inst = dat[i]&255;
		int imme = dat[i]>>8;
		if (ops[inst*5] == 'B' && ops[inst*5+1] != 'I' && ops[inst*5+1] != 'O') {
			//printf("b to %d\n", i+(imme>>2)+1);
			label[i+(imme>>2)+1] = ++label_sz;
		}
		if (ops[inst*5] == 'J') {
			//printf("j to %d\n", i+(imme>>2)+1);
			label[i+(imme>>2)+1] = ++label_sz;
		}
	}
	fprintf(fo, "No.:   Addr:    Value:  Intr  Operand  (Dec Format) #Label_id #Meaning\n");
	fprintf(fo, "=======================================================================\n");	 
	for (i = 0 ; i < c_sz ; i++) {
		int inst = dat[i]&255;
		int imme = (int)(((unsigned int)(dat[i]))>>8);
		if((unsigned char)(inst)<=0xd1) { //IDLE=0xd1
			fprintf(fo, "%3d: %08x: %08x: %c%c%c%c 0x%06x (D%4d)",
					i + 1, i * 4, dat[i],
					ops[inst * 5], ops[inst * 5 + 1], ops[inst * 5 + 2], ops[inst * 5 + 3],
					imme, dat[i] >> 8
			);
		}else{ //illegal instr
			fprintf(fo, "%3d: %08x: %08x: %s \n",
					i + 1, i * 4, dat[i],"illegal instr");
			continue;
		}
		if (i == hdr.entry>>2)
			fprintf(fo, " # <=ENTRY");
		if (label[i])
			fprintf(fo, " # label%d ", label[i]);
		else
			fprintf(fo, "          ");
		if (ops[inst*5] == 'B' && ops[inst*5+1] != 'I' && ops[inst*5+1] != 'O')
			fprintf(fo, " # Cond goto label%d", ++label_ct);
		else if (ops[inst*5] == 'J') {
		        if(ops[inst*5+1] == 'M') {
			        fprintf(fo, " # Jmp label%d 0x%x=pc+%d", ++label_ct, (i<<2)+imme, dat[i]>>8);
			    } else if(ops[inst*5+1] == 'S') {
			        fprintf(fo, " # Call label%d 0x%x=pc+%d", ++label_ct, (i<<2)+imme, dat[i]>>8);
			    }
		}	    
		if (!cmt) {
			fprintf(fo, "\n"); continue;
		}

		if (ops[inst*5] == 'E' && ops[inst*5+1] == 'N') {
			fprintf(fo, " # sp += 0x%x=%d\n", imme, dat[i] >> 8); continue;
		}
		if (ops[inst*5] == 'H' && ops[inst*5+1] == 'A' && ops[inst*5+2] == 'L' && ops[inst*5+3] == 'T') {
			fprintf(fo, " # System Halt\n"); continue;
		}

		if (ops[inst*5] == 'I' && ops[inst*5+1] == 'D' && ops[inst*5+2] == 'L' && ops[inst*5+3] == 'E') {
			fprintf(fo, " # Idle, wait interrupt\n"); continue;
		}

		if (ops[inst*5] == 'P' && ops[inst*5+1] == 'D' && ops[inst*5+2] == 'I' && ops[inst*5+3] == 'R') {
			fprintf(fo, " # Page_Directory_Addr=ra\n"); continue;
		}
		if (ops[inst*5] == 'R' && ops[inst*5+1] == 'T' && ops[inst*5+2] == 'I') {
			fprintf(fo, " # POP fault code|pc|sp\n"); continue;
		}
		if (ops[inst*5] == 'S' && ops[inst*5+1] == 'P' && ops[inst*5+2] == 'A' && ops[inst*5+3] == 'G') {
			fprintf(fo, " # Paging=ra set/unset paging\n"); continue;
		}
		if (ops[inst*5] == 'S' && ops[inst*5+1] == 'S' && ops[inst*5+2] == 'P') {
			fprintf(fo, " # kenel_sp=ra\n"); continue;
		}
		if (ops[inst*5] == 'S' && ops[inst*5+1] == 'T' && ops[inst*5+2] == 'I') {
			fprintf(fo, " # Enable Interrupt\n"); continue;
		}
		if (ops[inst*5] == 'S' && ops[inst*5+1] == 'S' && ops[inst*5+2] == 'P') {
			fprintf(fo, " # kernel_sp = ra\n"); continue;
		}
		if (ops[inst*5] == 'T' && ops[inst*5+1] == 'I' && ops[inst*5+2] == 'M' && ops[inst*5+3] == 'E') {
			fprintf(fo, " # timer's timeout =ra\n"); continue;
		}
		if (ops[inst*5] == 'T' && ops[inst*5+1] == 'R' && ops[inst*5+2] == 'A' && ops[inst*5+3] == 'P') {
			fprintf(fo, " #trap = FSYS for syscall\n"); continue;
		}
		if (ops[inst*5] == 'M' && ops[inst*5+1] == 'C' && ops[inst*5+2] == 'P' && ops[inst*5+3] == 'Y') {
			fprintf(fo, " # memcpy(char *ra, char *rb, len=rc)\n"); continue;
		}
		if (ops[inst*5] == 'M' && ops[inst*5+1] == 'C' && ops[inst*5+2] == 'M' && ops[inst*5+3] == 'P') {
			fprintf(fo, " # memcmp(char *ra, char *rb, len=rc)\n"); continue;
		}
		if (ops[inst*5] == 'M' && ops[inst*5+1] == 'C' && ops[inst*5+2] == 'H'&& ops[inst*5+3] == 'R') {
			fprintf(fo, " # memchr(char *ra, char rb, len=rc)\n"); continue;
		}
		if (ops[inst*5] == 'M' && ops[inst*5+1] == 'S' && ops[inst*5+2] == 'E' && ops[inst*5+3] == 'T') {
			fprintf(fo, " # memset(char *ra, char rb, len=rc)\n"); continue;
		}
		if (inst >= 0x4f && inst <= 0x83) {
			if (inst-0x4f < 4) {
				switch (inst-0x4f) {
					case 0 : fprintf(fo, " # rf = rf + rg"); break;
					case 1 : fprintf(fo, " # rf = rf - rg"); break;
					case 2 : fprintf(fo, " # rf = rf * rg"); break;
					case 3 : fprintf(fo, " # rf = rf / rg"); break;
				}
			} else if ((inst-0x54)/3 < 13) {
				switch ((inst-0x54)/3) {
					case 0 : fprintf(fo, " # ra = ra +"); break;
					case 1 : fprintf(fo, " # ra = ra -"); break;
					case 2 : fprintf(fo, " # ra = int(ra) * "); break;
					case 3 : fprintf(fo, " # ra = int(ra) /"); break;
					case 4 : fprintf(fo, " # ra = uint(ra) /"); break;
					case 5 : fprintf(fo, " # ra = int(ra) mod"); break;
					case 6 : fprintf(fo, " # ra = uint(ra) mod"); break;
					case 7 : fprintf(fo, " # ra = ra &"); break;
					case 8 : fprintf(fo, " # ra = ra |"); break;
					case 9 : fprintf(fo, " # ra = ra ^"); break;
					case 10: fprintf(fo, " # ra = ra <<"); break;
					case 11: fprintf(fo, " # ra = int(ra) >>"); break;
					case 12: fprintf(fo, " # ra = uint(ra) >>"); break;
				}
				switch ((inst-0x54)%3) {
					case 0 : fprintf(fo, " rb"); break;
					case 1 : fprintf(fo, " %d", imme); break;
					case 2 : fprintf(fo, " sp[%d]", imme); break;
				}
			} else {
				switch (inst-0x7a) {
					case 0 : fprintf(fo, " # ra = (ra == rb)"); break;
					case 1 : fprintf(fo, " # ra = (rf == rg)"); break;
					case 2 : fprintf(fo, " # ra = (ra != rb)"); break;
					case 3 : fprintf(fo, " # ra = (rf != rg)"); break;
					case 4 : fprintf(fo, " # ra = (ra == rb)"); break;
					case 5 : fprintf(fo, " # ra = ((int)ra < (int)rb)"); break;
					case 6 : fprintf(fo, " # ra = ((uint)ra < (uint)rb)"); break;
					case 7 : fprintf(fo, " # ra = (f < g)"); break;
					case 8 : fprintf(fo, " # ra = ((int)ra < (int)rb)"); break;
					case 9 : fprintf(fo, " # ra = ((uint)ra < (uint)rb)"); break;
					case 10: fprintf(fo, " # ra = (f > g)"); break;
				}
			}
		}
		if (ops[inst*5] == 'B') {
			if (ops[inst*5+1] == 'I')
				fprintf(fo, " # ra = kbchar, kbchar is the value from outside IO\n");
			if (ops[inst*5+1] == 'O') 
				fprintf(fo, " # write(ra, &rb, 1)");
		}
		if (ops[inst*5] == 'C') {
			if (ops[inst*5+1] == 'L' && ops[inst*5+2] == 'I') {
				fprintf(fo, " # ra=iena, Disable Interrupt\n"); continue;
			}
			if (ops[inst*5+2] == 'D') {
				fprintf(fo, " # rf = ");
				if (ops[inst*5+1] == 'I') 
					fprintf(fo, "double(int(ra))");
				else
					fprintf(fo, "double(uint(ra))");
			}
			if (ops[inst*5+1] == 'D') {
				fprintf(fo, " # ra = ");
				if (ops[inst*5+2] == 'I') 
					fprintf(fo, "int(rf)");
				else
					fprintf(fo, "uint(ra)");
			}
		}
		if (ops[inst*5] == 'I') {
			if (ops[inst*5+1] == 'V')
				fprintf(fo, " # intr vec addr = ra\n");
			if (ops[inst*5+1] == 'D')
				fprintf(fo, " # idle, wait interrupt\n");
		}

		if (ops[inst*5] == 'L') {
			if (ops[inst*5+2] == 'V') {
				fprintf(fo, " # pc=*sp, sp += %d + 8 (return) \n", imme);
				continue;
			}
			if (ops[inst*5+1] == 'B' && ops[inst*5+2] == 'A') {
				if (ops[inst*5+3] == 'D')
					fprintf(fo, " # rg=rf \n");
				else
					fprintf(fo, " # rb=ra \n");
				continue;
			}
			if (ops[inst*5+1] == 'E') {
				if (ops[inst*5+3] == 'G') 
					fprintf(fo, " # ra = pc+%d\n", imme);
				else
					fprintf(fo, " # ra = sp+%d\n", imme);
				continue;
			}
			if (ops[inst*5+1] == 'C' && ops[inst*5+2] == 'L') {
				fprintf(fo, " # rc = *(uint *)(sp + imme)\n");
				continue;
			}
			if (ops[inst*5+1] == 'C' && ops[inst*5+2] == 'A') {
				fprintf(fo, " # rc = ra\n");
				continue;
			}
			if (ops[inst*5+1] == 'V' && ops[inst*5+2] == 'A' && ops[inst*5+3] == 'D') {
				fprintf(fo, " # ra= bad virtual address\n");
				continue;
			}
			if (ops[inst*5+1] == 'U' && ops[inst*5+2] == 'S' && ops[inst*5+3] == 'P') {
				fprintf(fo, " # ra= user_sp \n");
				continue;
			}
			fprintf(fo, " # ");
			int opi = inst*5+1;
			if (ops[opi] == 'B') {
				fprintf(fo, "rb = ");
				opi++;
			} else
				fprintf(fo, "ra = ");
			switch (ops[opi+1]) {
				case ' ' : fprintf(fo, "uint(");  break;
				case 'S' : fprintf(fo, "short("); break;
				case 'H' : fprintf(fo, "ushort(");break;
				case 'C' : fprintf(fo, "char(");  break;
				case 'B' : fprintf(fo, "uchar("); break;
				case 'D' : fprintf(fo, "double(");break;
				case 'F' : fprintf(fo, "float("); break;
			}
			switch (ops[opi]) {
				case 'L' : fprintf(fo, "sp[0x%x=%d])",  imme,  imme); break;
				case 'G' : fprintf(fo, "gaddr[0x%x=%d=pc+%d])", (i<<2)+imme, (i<<2)+imme, imme); break;
				case 'X' : 
					if (opi == inst*5+1)
						fprintf(fo, "ra[0x%x=%d])", imme, imme);
					else
						fprintf(fo, "rb[0x%x=%d])", imme, imme);
					break;break;
				case 'I' : fprintf(fo, "0x%x=%d)",  imme, imme); break;
			}
		}
		if (ops[inst*5] == 'S') {
			if (ops[inst*5+1] == 'U' && ops[inst*5+2] == 'S' && ops[inst*5+3] == 'P') {
				fprintf(fo, " # user_sp=ra \n");
				continue;
			}
			if (ops[inst*5+1] == 'U') {
				fprintf(fo, "\n"); continue;
			}
			fprintf(fo, " # ");
			int opi = inst*5+1;
			if (ops[opi] == 'B') opi++;
			switch (ops[opi+1]) {
				case ' ' : fprintf(fo, "uint(");  break;
				case 'S' : fprintf(fo, "short("); break;
				case 'H' : fprintf(fo, "ushort(");break;
				case 'C' : fprintf(fo, "char(");  break;
				case 'B' : fprintf(fo, "uchar("); break;
				case 'D' : fprintf(fo, "double(");break;
				case 'F' : fprintf(fo, "float("); break;
			}
			switch (ops[opi]) {
				case 'L' : fprintf(fo, "sp[0x%x=%d])",   imme, imme); break;
				case 'G' : fprintf(fo, "gaddr[0x%x=%d=pc+%d])", (i<<2)+imme, (i<<2)+imme, imme); break;
				case 'X' : 
					if (opi == inst*5+1)
						fprintf(fo, "ra[0x%x=%d])", imme, imme);
					else
						fprintf(fo, "rb[0x%x=%d])", imme, imme);
					break;
			}
			if (opi == inst*5+2)
				fprintf(fo, " = rb");
			else
				fprintf(fo, " = ra");
		}
		if (ops[inst*5] == 'P') {
			if (ops[inst*5+1] == 'S' && ops[inst*5+2] == 'H') {
				switch (ops[inst*5+3]) {
					case 'A' : fprintf(fo, " # push ra"); break;
					case 'I' : fprintf(fo, " # push 0x%x=%d", imme, imme);break;
					case 'F' : fprintf(fo, " # push rf"); break;
					case 'B' : fprintf(fo, " # push rb"); break;
					case 'C' : fprintf(fo, " # push rc"); break;
					case 'G' : fprintf(fo, " # push rg"); break;
				}
			}
			if (ops[inst*5+1] == 'O' && ops[inst*5+2] == 'P') {
				switch (ops[inst*5+3]) {
					case 'A' : fprintf(fo, " # pop ra"); break;
					case 'F' : fprintf(fo, " # pop rf"); break;
					case 'B' : fprintf(fo, " # pop rb"); break;
					case 'C' : fprintf(fo, " # pop rc"); break;
					case 'G' : fprintf(fo, " # pop rg"); break;
				}
			}
		}
		fprintf(fo, "\n");


	}
	fprintf(fo, "=======================================================================\n");
	fprintf(fo, "Data Segment\n");
	fprintf(fo, "Address     Hex									 | Char\n");
	for (i = c_sz ; i < d_sz ; i += 4) {
		fprintf(fo, "0x%08x	", i<<2);
		for (j = i ; j < i+4 ; j++)
			if (j < d_sz) 
				fprintf(fo, "%02x %02x %02x %02x	", 
					dat[j]&255, (dat[j]>>8)&255, (dat[j]>>16)&255, dat[j]>>24) ;
			else
				fprintf(fo, "                   	");
		fprintf(fo, " | ");
		for (j = i ; j < i+4 ; j++)
			if (j < d_sz)
				fprintf(fo, "%c%c%c%c", 
					showChar(dat[j]&255),
					showChar((dat[j]>>8)&255),
					showChar((dat[j]>>16)&255),
					showChar(dat[j]>>24)
				);
			else
				fprintf(fo, "    ");
		fprintf(fo, "\n");
	}
	return 0;
}


