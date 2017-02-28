These info is base and from https://github.com/rswier/swieros

A tiny and fast Unix-ish kernel (based on xv6), compiler, and userland for fun, education, and research.
Virtual CPU with full user/supervisor and virtual memory support, fast enough to support self-emulation.
Fast C-subset compiler allowing on-the-fly compile-and-go of all applications.

In each DIR，
```
make clean
make 
make run
# then you will see the os examples/demos running

``` 

In xv6_os
```
clean.sh
boot.sh
# then you will see the xv6 os running
```


# The lab lists:

## tiny&simple labs 
- [os_helloworld](os_helloworld) : print string
- [os_timer_interrupt ](os_timer_interrupt ) : setup&enable timer interrupt 
- [os_bad_phys_addr](os_bad_phys_addr) : check exception: apps access bad phys addr
- [os_divid_by_zero](os_divid_by_zero) : check exception: apps divid by zero
- [os_invalid_intruction](os_invalid_intruction) : check exception: apps execute invalid instruction 
- [os_page_fault](os_page_fault) : check exception: apps access fault page
- [os_task_switch](os_task_switch) : two kernel tasks switch each other.
- [os_user_task_switch](os_user_task_switch) : two user tasks switch each other.
- [os_user_task_syscall](os_user_task_syscall) : user tasks invoke syscall

## simple xv6 os
- [xv6_os](xv6_os) : a complete xv6 os

## ucore os
- [ucore_helloworld](ucore_helloworld) : print string.
- [ucore_enable_interrupt](ucore_enable_interrupt) : setup&enable interrupt 
- [ucore_enable_paging](ucore_enable_paging) : setup&enable page table 
- [ucore_enable_virt_mem](ucore_enable_virt_mem) :  setup virtual mem management
- [ucore_enable_usrapp](ucore_enable_usrapp) : setup&run user apps
- [ucore_enable_scheduling](ucore_enable_scheduling) : add scheduling algorithms
- [ucore_enable_mutexsync](ucore_enable_mutexsync) : semaphore & monitor mechanism
- [ucore_enable_filesystem](ucore_enable_filesystem) : sfs filesystem 

