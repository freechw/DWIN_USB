/******************************************************************************
																	
                  ��Ȩ���� (C), 2019, �������ĿƼ����޹�˾	
																			  
*******************************************************************************
�� �� ��   : usb_dgus.c
�� �� ��   : V1.0
��    ��   : chenxianyue
��������   : 2019��6��27��
��������   : USB�ӿ���صĵ�DGUSӦ�ó���ʵ��
�޸���ʷ   :
��    ��   : 
��    ��   : 
�޸�����   : 	
******************************************************************************/

#include "usb_dgus.h"
#include "app/app_usb/app_interface.h"
#include "driver/dgus/dgus.h"

/********************************�궨��***************************************/
/* USB DGUS�Ĵ�����ַ */
#define DGUS_ADDR_GET_OR_SET_PATH		(0x5C0)
#define DGUS_ADDR_CREATE_OR_DEL_PATH	(0x5C4)
#define DGUS_ADDR_READ_OR_WRITE_FILE	(0x5C8)
#define DGUS_ADDR_SEARCH_FILE			(0x5D0)
#define DGUS_ADDR_SYSTEM_UP				(0x5D4)
/* USB ������������ */
#define ACK_GET_OR_SET_PATH				(0x05)
#define ACK_CREATE_OR_DEL_PATH			(0x04)
#define ACK_READ_OR_WRITE_FILE			(0x03)
#define ACK_SEARCH_FILE					(0x02)
#define ACK_SYSTEM_UP					(0x01)	
/* ��־λ���� */
#define FLAG_START						(0x5A)
#define FLAG_END						(0x00)
#define FLAG_READ						(0x5A)
#define FLAG_WRITE						(0xA5)
#define FLAG_CREATE						(0x5A)
#define FLAG_DELETE						(0xA5)
#define TYPE_FILE						(0x55)
#define TYPE_DIR 						(0xAA)

UINT8 CompareDgusRegValue(UINT32 AddrDgus, UINT8 Value)
{
	UINT8 DgusValue = 0;
	ReadDGUS(AddrDgus, &DgusValue, sizeof(DgusValue));
	if (DgusValue == Value) return DWIN_OK;
	else return DWIN_ERROR;
}

void USBModule(void)
{
	UINT8 ACK = 0;
	/* (1) ɨ��DGUS������־λ ȷ����ӦӦ���־ ��ɨ��ļĴ��� ִ�����ȼ��� */
	if (DWIN_OK == CompareDgusRegValue(DGUS_ADDR_GET_OR_SET_PATH, FLAG_READ) ||
		DWIN_OK == CompareDgusRegValue(DGUS_ADDR_GET_OR_SET_PATH, FLAG_WRITE))
		ACK = ACK_GET_OR_SET_PATH;
	if (DWIN_OK == CompareDgusRegValue(DGUS_ADDR_CREATE_OR_DEL_PATH, FLAG_CREATE) ||
		DWIN_OK == CompareDgusRegValue(DGUS_ADDR_CREATE_OR_DEL_PATH, FLAG_DELETE))
		ACK = ACK_CREATE_OR_DEL_PATH;
	if (DWIN_OK == CompareDgusRegValue(DGUS_ADDR_READ_OR_WRITE_FILE, FLAG_READ) ||
		DWIN_OK == CompareDgusRegValue(DGUS_ADDR_READ_OR_WRITE_FILE, FLAG_WRITE))
		ACK = ACK_READ_OR_WRITE_FILE;
	if (DWIN_OK == CompareDgusRegValue(DGUS_ADDR_SEARCH_FILE, FLAG_START))
		ACK = ACK_SEARCH_FILE;
	if (DWIN_OK == CompareDgusRegValue(ACK_SYSTEM_UP, FLAG_START))
		ACK = ACK_SYSTEM_UP;

	/* (2) Ӧ����Ӧ */
	switch (ACK)
	{
		case ACK_SYSTEM_UP:
		{
			//AckSystemUp();
			break;
		}
		case ACK_SEARCH_FILE:
		{
			//AckSearchFile();
			break;
		}
		case ACK_READ_OR_WRITE_FILE:
		{
			//AckReadOrWriteFile();
			break;
		}
		case ACK_CREATE_OR_DEL_PATH:
		{
			//AckCreateOrDelPath();
			break;
		}
		case ACK_GET_OR_SET_PATH:
		{
			//AckGetOrSetPath();
			break;
		}
		default:
			break;
	}
}