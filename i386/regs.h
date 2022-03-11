#ifndef _REG_H_
#define _REG_H_

struct regs {
	u_int32_t	r_gs;
	u_int32_t	r_fs;
	u_int32_t	r_es;
	u_int32_t	r_ds;
	u_int32_t	r_edi;
	u_int32_t	r_esi;
	u_int32_t	r_ebp;
	u_int32_t	r_esp;
	u_int32_t	r_ebx;
	u_int32_t	r_edx;
	u_int32_t	r_ecx;
	u_int32_t	r_eax;
	u_int32_t	r_intrno;
	u_int32_t	r_err;
	u_int32_t	r_eip;
	u_int32_t	r_cs;
	u_int32_t	r_eflags;
	u_int32_t	r_uesp;
	u_int32_t	r_ss;
};

#endif /* _REG_H_ */
