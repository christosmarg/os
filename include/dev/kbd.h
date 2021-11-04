#ifndef _KERNEL_KBD_H_
#define _KERNEL_KBD_H_

#define KBD_PRESSED	0x80
#define KBD_LSHIFT	0x2a
#define KBD_RSHIFT	0x36
#define KBD_LSHIFT_REL	0xaa
#define KBD_RSHIFT_REL	0xb6

void kbd_init(void);

#endif /* _KERNEL_KBD_H_ */
