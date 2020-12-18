#ifndef I2C_MASTER_NOINT_H__
#define I2C_MASTER_NOINT_H__
// Header file for I2C_master_noint.c
// helps implement use I2C1 as a master without using interrupts

void I2C_master_setup(void);              // set up I2C 1 as a master, at 100 kHz
void I2C_master_start(void);              // send a START signal
void I2C_master_restart(void);            // send a RESTART signal
void I2C_master_send(unsigned char byte); // send a byte (either an address or data)
unsigned char I2C_master_recv(void);      // receive a byte of data
void I2C_master_ack(int val);             // send an ACK (0) or NACK (1)
void I2C_master_stop(void);               // send a stop

#endif