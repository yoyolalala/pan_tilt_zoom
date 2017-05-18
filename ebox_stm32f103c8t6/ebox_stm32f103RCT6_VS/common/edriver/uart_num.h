#ifndef __UART_NUM
#define __UART_NUM

#include "ebox.h"
#include "uart_string.h"

//#define __UART_NUM_DEBUG

#define UART_NUM_BUF_SIZE_CHAR 4
#define UART_NUM_BUF_SIZE_NUM 10
class UartNum
{
	Uart *uart;
	//用于存储接收数据位，转换为数字
	union num
	{
		float f;
		unsigned char c[UART_NUM_BUF_SIZE_CHAR];
	}charsBuf;

	int charBufIndex;//接收数据位的index
	int numBufIndex;//接收数字的index
	bool isTrans;//标志上一次是否是转义字符
	
	//bool isBusy;//标志是否正在接收
	bool isBegin;//标志是否处于帧头

	void rxEvent()
	{
		uint8_t c;
		c = uart->read();
		if (isBegin)
		{
			recievedNum = 0;
			charBufIndex = 0;
			numBufIndex = 0;
			isTrans = false;
			isBegin = false;
		}
		//不在反转义状态
		if (!isTrans)
		{
			if (c == '\\')
			{
				isTrans = true;
				return;
			}
			else if (c == '\n')
			{
				//当未进入反转义时，遇到'\n'即结束该帧的结束
				recievedNum = numBufIndex;
				isBegin = true;
				for (int j = 0; j < recievedNum; j++)
				{
					numsBufOld[j]=numsBuf[j];
				}
				
#ifdef __UART_NUM_DEBUG
				printf("转换结果%d个：", recievedNum);
				for (int j = 0; j < recievedNum; j++)
				{
					printf("%f ", numsBuf[j]);
				}
				printf("\n");
#endif
				return;
			}
			else
			{
				//当前位是数据位
				charsBuf.c[charBufIndex] = c;
				charBufIndex++;
			}
		}
		//在反转义状态
		else
		{
			//当前位是数据位
			charsBuf.c[charBufIndex] = c;
			charBufIndex++;
			isTrans = false;
		}

		//判断已接收数据位个数，如果单个数据接收完毕，存储数据
		if (charBufIndex >= UART_NUM_BUF_SIZE_CHAR)
		{
			if (numBufIndex >= UART_NUM_BUF_SIZE_NUM)
			{
				recievedNum = charBufIndex;
				printf("Num buffer overflow!!!\n");
				isBegin = true;
			}
			numsBuf[numBufIndex] = charsBuf.f;
			numBufIndex++;
			charBufIndex = 0;
		}
	}
public:
	float numsBuf[UART_NUM_BUF_SIZE_NUM], numsBufOld[UART_NUM_BUF_SIZE_NUM];//用于存储接收的数据
	int recievedNum;
	//构建基于uartX的UartNum类
	UartNum(Uart *uartX) :
		uart(uartX),
		isTrans(false),
		charBufIndex(0),
		numBufIndex(0),
		//isBusy(false),
		recievedNum(0),
		isBegin(true)
	{

	}
	//初始化uart、绑定中断函数
	void begin(uint32_t baud_rate, uint8_t _use_dma = 1)
	{
		uart->begin(baud_rate, _use_dma);
		uart->attach(this, &UartNum::rxEvent, RxIrq);
	}
	//初始化uart、绑定中断函数
	void begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit, uint8_t _use_dma)
	{
		uart->begin(baud_rate, data_bit, parity, stop_bit, _use_dma);
		uart->attach(this, &UartNum::rxEvent, RxIrq);
	}
	void printf(const char *fmt, ...)
	{
		uart->printf(fmt);
	}
	////获取接收数据
	//void getNums(float* nums)
	//{
	//	for (int i = 0; i < recievedNum; i++)
	//	{
	//		nums[i] = numsBufOld[i];
	//	}
	//}
	////设置为正在接收
	//void setBusy()
	//{
	//	isBusy = true;
	//}
	////是否正在接收
	//bool busy()
	//{
	//	return isBusy;
	//}
};

#endif