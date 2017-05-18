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
	void Write_IIC_Byte(unsigned char IIC_Byte);//дһ���ֽ�
	void OLED_WrDat(unsigned char IIC_Data);//д����
	void OLED_WrCmd(unsigned char IIC_Command);//д����
	void OLED_Set_Pos(unsigned char x, unsigned char y);//��������
	void OLED_Fill(unsigned char bmp_dat);//oledȫ��
	void OLED_CLS(void);//��λ
	void disnum(unsigned int num, unsigned char x, unsigned char y);//��ʾ������ֵ����ִ�
	void OLED_P6x8Str(unsigned char x, unsigned char y, char ch[]);//��ʾ6*8һ���׼ASCII�ַ��� ����(x,y) yΪҳ��Χ0~7
	void disfloat_num(unsigned char x, unsigned char y, float num);//������������ʾ ֻ��ʾʮλ��ʼ��С�����3λ
	void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
	void OLED_Show4CN(unsigned char x, unsigned char y);
	int OLED_printf(uint8_t x, uint8_t y, const char *fmt, ...);
	void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
};