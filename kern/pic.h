#ifndef _PIC_H_
#define _PIC_H_

#include <u.h>

#define PIC_MASTER_CMD	0x20
#define PIC_MASTER_DATA	(PIC_MASTER_CMD + 1)
#define PIC_MASTER_OFFT	0x20
#define PIC_SLAVE_CMD	0xa0
#define PIC_SLAVE_DATA	(PIC_SLAVE_CMD + 1)
#define PIC_SLAVE_OFFT	(PIC_MASTER_OFFT + 8)

#define PIC_SET_MASK	0
#define PIC_CLEAR_MASK	1
#define PIC_EOI		0x20	/* End of interrupt */

#define ICW1_ICW4	0x01	/* ICW4 not needed */
#define ICW1_SINGLE	0x02	/* Single cascade mode */
#define ICW1_INTVAL4	0x04	/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08	/* Level triggered mode */
#define ICW1_INIT	0x10	/* Initialization */
#define ICW4_8086	0x01	/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02	/* Auto EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered slave */
#define ICW4_BUF_MASTER	0x0c	/* Buffered master */
#define ICW4_SFNM	0x10	/* Special fully nested */

void		pic_remap(void);
void		pic_eoi(u_int32_t);
void		pic_mask(u_int8_t, int);
void		pic_on(void);
void		pic_off(void);

#endif /* _PIC_H_ */
