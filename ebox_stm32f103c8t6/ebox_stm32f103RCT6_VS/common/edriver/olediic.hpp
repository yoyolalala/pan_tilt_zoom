#include "ebox.h"
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF 
#define X_WIDTH 128
#define Y_WIDTH 64
#define OLED_ADDR  0x78
#define SCL_1 scl_1()
#define SCL_0 scl_0()
#define SDA_1 sda_1()
#define SDA_0 sda_0()
class Oled
{
	Gpio *SCL; Gpio *SDA;
	void scl_1();
	void scl_0();
	void sda_0();
	void sda_1();
	void IIC_delay();
	void IIC_Start();
	void IIC_Stop();
public:
	Oled(Gpio *scl, Gpio *sda) :
		SCL(scl),SDA(sda)
	{

	}
	void OLED_Init(void);
	void Write_IIC_Byte(unsigned char IIC_Byte);//写一个字节
	void OLED_WrDat(unsigned char IIC_Data);//写数据
	void OLED_WrCmd(unsigned char IIC_Command);//写命令
	void OLED_Set_Pos(unsigned char x, unsigned char y);//设置坐标
	void OLED_Fill(unsigned char bmp_dat);//oled全屏
	void OLED_CLS(void);//复位
	void disnum(unsigned int num, unsigned char x, unsigned char y);//显示五个数字的数字串
	void OLED_P6x8Str(unsigned char x, unsigned char y, char ch[]);//显示6*8一组标准ASCII字符串 坐标(x,y) y为页范围0~7
	void disfloat_num(unsigned char x, unsigned char y, float num);//浮点型数据显示 只显示十位开始到小数点后3位
	void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
	void OLED_Show4CN(unsigned char x, unsigned char y);
	int OLED_printf(uint8_t x, uint8_t y, const char *fmt, ...);
	void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
};