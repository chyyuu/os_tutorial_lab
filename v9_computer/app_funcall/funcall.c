int gk=8;
int gi=9;
int gj=10;
int bi,bj;
int f2(int i, int j) {
  return i-j;
}

int f1(int i) {
   f2(i,2);
   return i+3;
}

void  main(void) {
       int li;
       if(gk>8) 
           bi=gi+1;
       else
           bj=gj+5;
       li=bi+2;
      li=f1(li);
}