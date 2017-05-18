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
	//���ڴ洢��������λ��ת��Ϊ����
	union num
	{
		float f;
		unsigned char c[UART_NUM_BUF_SIZE_CHAR];
	}charsBuf;

	int charBufIndex;//��������λ��index
	int numBufIndex;//�������ֵ�index
	bool isTrans;//��־��һ���Ƿ���ת���ַ�
	
	//bool isBusy;//��־�Ƿ����ڽ���
	bool isBegin;//��־�Ƿ���֡ͷ

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
		//���ڷ�ת��״̬
		if (!isTrans)
		{
			if (c == '\\')
			{
				isTrans = true;
				return;
			}
			else if (c == '\n')
			{
				//��δ���뷴ת��ʱ������'\n'��������֡�Ľ���
				recievedNum = numBufIndex;
				isBegin = true;
				for (int j = 0; j < recievedNum; j++)
				{
					numsBufOld[j]=numsBuf[j];
				}
				
#ifdef __UART_NUM_DEBUG
				printf("ת�����%d����", recievedNum);
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
				//��ǰλ������λ
				charsBuf.c[charBufIndex] = c;
				charBufIndex++;
			}
		}
		//�ڷ�ת��״̬
		else
		{
			//��ǰλ������λ
			charsBuf.c[charBufIndex] = c;
			charBufIndex++;
			isTrans = false;
		}

		//�ж��ѽ�������λ����������������ݽ�����ϣ��洢����
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
	float numsBuf[UART_NUM_BUF_SIZE_NUM], numsBufOld[UART_NUM_BUF_SIZE_NUM];//���ڴ洢���յ�����
	int recievedNum;
	//��������uartX��UartNum��
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
	//��ʼ��uart�����жϺ���
	void begin(uint32_t baud_rate, uint8_t _use_dma = 1)
	{
		uart->begin(baud_rate, _use_dma);
		uart->attach(this, &UartNum::rxEvent, RxIrq);
	}
	//��ʼ��uart�����жϺ���
	void begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit, uint8_t _use_dma)
	{
		uart->begin(baud_rate, data_bit, parity, stop_bit, _use_dma);
		uart->attach(this, &UartNum::rxEvent, RxIrq);
	}
	void printf(const char *fmt, ...)
	{
		uart->printf(fmt);
	}
	////��ȡ��������
	//void getNums(float* nums)
	//{
	//	for (int i = 0; i < recievedNum; i++)
	//	{
	//		nums[i] = numsBufOld[i];
	//	}
	//}
	////����Ϊ���ڽ���
	//void setBusy()
	//{
	//	isBusy = true;
	//}
	////�Ƿ����ڽ���
	//bool busy()
	//{
	//	return isBusy;
	//}
};

#endif