#include "stdio.h"
#include "getopt.h"
#include "stdint.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "read.h"
struct ModbusRTU
{
char* serial;
uint16_t addr;
uint8_t cmd;
uint16_t reg;
uint16_t len;
};
struct ModbusTCP
{
char* ip;
uint16_t port;
uint8_t cmd;
uint16_t reg;
uint16_t len;
};


int lopt;
static struct option longOpts[] = {
	{"type",required_argument,NULL,'t'},
    {"serial",required_argument,NULL,'s'},
    {"ip",required_argument,NULL,'i'},
    {"port",required_argument,NULL,'p'},
    {"addr",required_argument,NULL,'a'},
    {"cmd",required_argument,NULL,'c'},
    {"reg",required_argument,NULL,'r'},
    {"len",required_argument,NULL,'l'},
    {"help",no_argument,NULL,'h'},
	{ 0,		 0,		    0,	   0   }
};

//modbus rtu read and write
//modbus tcp read and write
//Protocol etyp  -type RTU or TCP
//-serial /dev/tty
//-ip 192.168.8.10
//-port 503
//-addr 1
//-register 1001
//-len 2
struct ModbusRTU modbusrtu;
struct ModbusTCP modbustcp;
uint16_t modbus_type=0;
void put_help(void)
{
    puts("Options:" );
	puts(" -t, --type modbus          Modbus protocol type eg.TCP RTU" );
	puts(" -s, --serial               When using TCP, the serial port path." );
	puts(" -i, --ip                   When using TCP, the tcp ip." );
	puts(" -a, --addr                 When using RTU, the modbus device addr." );
    puts(" -c, --cmd                  Modbus instruction 1~4." );
	puts(" -r, --reg                  register addr." );
	puts(" -l, --len                  Read the length of the register." );
    puts(" -h, --help                 Output more help information.");
	exit(0);
}

int main (char argc,char* argv[])
{
     int optIndex = 0;
     char cmd;
     /*RTs:i:p:a:r:l*/
        cmd=getopt_long(argc,argv,"ht:",longOpts,&optIndex);
        if(cmd=='?'|| cmd==-1)
        {
            /*display help*/
            return -1;
        }
        if(cmd=='t')
        {
            if(strcmp(optarg ,"TCP")==0)
            {
                
                int opt_num=0;
                while(1)
                {
                    cmd=getopt_long(argc,argv,"i:p:r:l:c:",longOpts,&optIndex);
                    if(cmd=='?'|| cmd==-1)return -1;
                    switch (cmd)
                    {
                        case 'i' :{modbustcp.ip=optarg;opt_num++;break;}
                        case 'p' :{modbustcp.port=atoi(optarg) ;opt_num++;break;}
                        case 'c' :{modbustcp.cmd=atoi(optarg) ;opt_num++;break;}
                        case 'r' :{modbustcp.reg=atoi(optarg) ;opt_num++;break;}
                        case 'l' :{modbustcp.len=atoi(optarg) ;opt_num++;break;}
                        default:break;
                    }
                    if(opt_num>=5)
                    {
                        modbus_type=1;
                        break;
                    }
                }
                
            }
            else if(strcmp(optarg ,"RTU")==0)
            {
                int opt_num=0;
                while(1)
                {
                    cmd=getopt_long(argc,argv,"s:a:r:l:c:",longOpts,&optIndex);
                    if(cmd=='?'|| cmd==-1)return -1;
                    switch (cmd)
                    {
                        case 's' :{modbusrtu.serial=optarg;opt_num++;break;}
                        case 'a' :{modbusrtu.addr=atoi(optarg) ;opt_num++;break;}
                        case 'c' :{modbustcp.cmd=atoi(optarg) ;opt_num++;break;}
                        case 'r' :{modbusrtu.reg=atoi(optarg) ;opt_num++;break;}
                        case 'l' :{modbusrtu.len=atoi(optarg) ;opt_num++;break;}
                        default:break;
                    }
                    if(opt_num>=5)
                    {
                        modbus_type=2;
                        break;
                    }
                }
            }
        }
        else if(cmd=='h')
        {
            put_help();
        }
        else return 1;

        if(modbus_type==1)//tcp
        {
            printf("%s %d %d %d %d\n",modbustcp.ip,modbustcp.port,modbustcp.cmd,modbustcp.reg,modbustcp.len);
            tcp_read_register(modbustcp.ip,modbustcp.port,modbustcp.cmd,modbustcp.reg,modbustcp.len);
        }
        else if(modbus_type==2)//rtu
        {
            printf("%s %d %d %d %d\n",modbusrtu.serial,modbusrtu.addr,modbusrtu.cmd,modbusrtu.reg,modbusrtu.len);
            rtu_read_register(modbusrtu.serial,modbusrtu.addr,modbusrtu.cmd,modbusrtu.reg,modbusrtu.len);
        }
        return -1;
        

}