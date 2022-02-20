#ifndef _READ_H_
#define _READ_H_

void rtu_read_register(char* dev,uint8_t addr,uint8_t cmd, uint16_t reg, uint16_t len);
void tcp_read_register(char *ip, uint16_t port, uint8_t cmd, uint16_t reg, uint16_t len);

#endif