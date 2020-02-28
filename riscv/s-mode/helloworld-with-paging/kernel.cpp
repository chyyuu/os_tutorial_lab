extern "C" void sbi_putchar(int c);
extern "C" void start();

namespace {

  using mword_t = unsigned long;
  static_assert(sizeof(mword_t) == sizeof(void *));

  constexpr unsigned long PAGE_SHIFT {12};
  constexpr unsigned long PAGE_SIZE  {1UL << PAGE_SHIFT};

  void print(const char *str)
  {
    for (;*str;str++) {
      sbi_putchar(*str);
    }
  }

  enum {
	CSR_SATP = 0x180U,
  };

  enum {
	SATP_MODE_SV39 = 8UL << 60,
  };

  template <mword_t CSR>
  void write_csr(mword_t value)
  {
    asm volatile ("csrw %[csr], %[val]"
		  :
		  : [csr] "i" (CSR), [val] "r" (value)
		  : "memory");
  }

  void sfence_vma()
  {
    // This flushes everything for now.
    asm volatile ("sfence.vma x0, x0");
  }

  alignas(PAGE_SIZE) mword_t root_pt[512];

  enum {
	PTE_V = 1UL << 0,
	PTE_R = 1UL << 1,
	PTE_W = 1UL << 2,
	PTE_X = 1UL << 3,
	PTE_U = 1UL << 4,
	PTE_G = 1UL << 5,
  };
}



void start()
{
  print("\nHello World!\n");

  // An identity mapped 1 GB page at 2 GB
  root_pt[2] = PTE_V | PTE_R | PTE_W | PTE_X | (2UL << 28); // page frames do not start at PAGE_SHIFT

  sfence_vma();
  write_csr<CSR_SATP>(SATP_MODE_SV39 | (reinterpret_cast<mword_t>(root_pt) >> PAGE_SHIFT));

  print("Paging enabled\n");
}
