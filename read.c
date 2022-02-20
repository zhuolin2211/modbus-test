#include "modbus.h"
#include "stdint.h"
#include "errno.h"
#include "stdio.h"
#include "string.h"
uint8_t dest[1024];
void rtu_read_register(char* dev,uint8_t addr,uint8_t cmd, uint16_t reg, uint16_t len)
{
    modbus_t *ctx = NULL;
    int ret=0;
    ctx = modbus_new_rtu(dev, 9600, 'N', 8, 1);
    //设置从机地址
    modbus_set_slave(ctx, 1);
    //设置串口模式(可选)
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    //建立连接
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connexion failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return ;
    }
    memset(dest,00,1024);
    switch (cmd)
    {
    case 0x01:
    {
        ret=modbus_read_bits(ctx,reg,len,dest);
        break;
    }
    case 0x02:
    {
    ret=modbus_read_input_bits(ctx,reg,len,dest);
        break;
    }
    case 0x03:
    {
    ret=modbus_read_registers(ctx,reg,len,(uint16_t*)dest);
        break;
    }
    case 0x04:
    {
        ret=modbus_read_input_registers(ctx,reg,len,(uint16_t*)dest);
        break;
    }
    default:break;
    }
    for(int i=0;i<((cmd==0x01 ||cmd==0x02)?len:len*2);i++)
    {
        printf("[%2x]",dest[i]);
    }
    if(ret==-1)
    {
        printf("%s\n",modbus_strerror(errno));
    }
    printf("\n");
    modbus_close(ctx);
    modbus_free(ctx);

}

void tcp_read_register(char *ip, uint16_t port, uint8_t cmd, uint16_t reg, uint16_t len)
{
    modbus_t *mb;
    int ret=0;
    mb = modbus_new_tcp(ip, port);
    if (modbus_connect(mb) == -1) {
        fprintf(stderr, "Connexion failed: %s\n", modbus_strerror(errno));
        modbus_free(mb);
        return ;
    }
    memset(dest,00,1024);
    switch (cmd)
    {
    case 0x01:
    {
        ret=modbus_read_bits(mb,reg,len,dest);
        break;
    }
    case 0x02:
    {
    ret=modbus_read_input_bits(mb,reg,len,dest);
        break;
    }
    case 0x03:
    {
    ret=modbus_read_registers(mb,reg,len,(uint16_t*)dest);
        break;
    }
    case 0x04:
    {
        ret=modbus_read_input_registers(mb,reg,len,(uint16_t*)dest);
        break;
    }
    default:break;
    }
    for(int i=0;i<((cmd==0x01 ||cmd==0x02)?len:len*2);i++)
    {
        printf("[%2x]",dest[i]);
    }
    
    if(ret==-1)
    {
        printf("%s\n",modbus_strerror(errno));
    }
    printf("\n");
    
    modbus_close(mb);
    modbus_free(mb);
}