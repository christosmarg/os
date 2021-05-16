#ifndef _KERNEL_PORT_H
#define _KERNEL_PORT_H

#define P_PIC1_CMD	0x20
#define P_PIC2_CMD	0xa0
#define P_PIC1_DATA	(P_PIC1_CMD + 1)
#define P_PIC2_DATA	(P_PIC2_CMD + 1)
#define P_TIMER_CMD	0x43
#define P_TIMER_DATA	0x40
#define P_CURS_CMD	0x3d4
#define P_CURS_DATA	0x3d5
#define P_KBD		0x60

#endif /* _KERNEL_PORT_H */
