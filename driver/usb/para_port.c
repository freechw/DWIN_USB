/******************************************************************************
																	
                  版权所有 (C), 2019, 北京迪文科技有限公司	
																			  
*******************************************************************************
文 件 名   : para_port.c
版 本 号   : V1.0
作    者   : chenxianyue
生成日期   : 2019年6月21日
功能描述   : CH376并口初始化以及与T5L并口通讯实现
修改历史   :
日    期   : 
作    者   : 
修改内容   : 	
******************************************************************************/

#include "para_port.h"
#include "t5los8051.h"
#include "intrins.h"

/********************************宏定义***************************************/

#define CH376_DATA_DAT_OUT(data)	{P2 = data;}		/* 向并口输出数据 */
#define CH376_DATA_DAT_IN()			(P2)				/* 从并口输入数据 */
#define	CH376_DATA_DIR_OUT()		{P2MDOUT |= 0xFF;}	/* 设置并口方向为输出 */
#define	CH376_DATA_DIR_IN()			{P2MDOUT &= 0x00;}	/* 设置并口方向为输入 */

/********************************函数定义开始*********************************/

/*****************************************************************************
 函 数 名  : mDelay0_5uS
 功能描述  : 软件延时0.5ms
 输入参数  : 无 
 输出参数  : 无
 修改历史  :
 日    期  : 2019年6月21日
 作    者  : chenxianyue
 修改内容  : 创建
*****************************************************************************/
void mDelay0_5uS()
{
	UINT8 i;
	for(i = 0; i < 9; i++) _nop_();
}

/*****************************************************************************
 函 数 名  : CH376_PORT_INIT
 功能描述  : Ch376相关端口初始化
 输入参数  : 无 
 输出参数  : 无
 修改历史  :
 日    期  : 2019年6月21日
 作    者  : chenxianyue
 修改内容  : 创建
*****************************************************************************/
void CH376_PORT_INIT(void)
{
	P2MDOUT |= 0x00;
	P1MDOUT |= 0x2F;
	CH376_CS = 1;
	CH376_WR = 1;
	CH376_RD = 1;
	CH376_A0 = 0;
	CH376_DATA_DIR_IN();			/* 设置并口输出 */
}

/*****************************************************************************
 函 数 名  : xReadCH376Status
 功能描述  : 从CH376读状态
 输入参数  : 无 
 输出参数  : 无
 修改历史  :
 日    期  : 2019年6月21日
 作    者  : chenxianyue
 修改内容  : 创建
*****************************************************************************/
UINT8 xReadCH376Status(void)
{
	UINT8 mData = 0;
	CH376_DATA_DIR_IN();
	CH376_WR = 1;
	CH376_RD = 0;
	CH376_A0 = 1;
	CH376_CS = 0;
	mData = CH376_DATA_DAT_IN();	/* 从CH376的并口输入数据 */
	CH376_WR = 1;
	CH376_RD = 1;
	CH376_A0 = 0;
	CH376_CS = 1;
	return mData;
}

/*****************************************************************************
 函 数 名  : xWriteCH376Cmd
 功能描述  : 向CH376写命令
 输入参数  : 无 
 输出参数  : 无
 修改历史  :
 日    期  : 2019年6月21日
 作    者  : chenxianyue
 修改内容  : 创建
*****************************************************************************/
void xWriteCH376Cmd(UINT8 mCmd)
{
	CH376_DATA_DAT_OUT(mCmd);  		/* 向CH376的并口输出数据 */
	CH376_DATA_DIR_OUT();  			/* 设置并口方向为输出 */
	CH376_A0 = 1;
	CH376_CS = 0;
	CH376_WR = 0;  					/* 输出有效写控制信号, 写CH376芯片的命令端口 */
	CH376_CS = 0;  					/* 该操作无意义,仅作延时,CH376要求读写脉冲宽度大于40nS */
	CH376_WR = 1;  					/* 输出无效的控制信号, 完成操作CH376芯片 */
	CH376_CS = 1;
	CH376_A0 = 0;
	CH376_DATA_DIR_IN();  			/* 禁止数据输出 */
	mDelay0_5uS(); mDelay0_5uS(); mDelay0_5uS();  /* 延时1.5uS确保读写周期大于1.5uS,或者用状态查询代替 */
}

/*****************************************************************************
 函 数 名  : xWriteCH376Data
 功能描述  : 向CH376写数据
 输入参数  : 无 
 输出参数  : 无
 修改历史  :
 日    期  : 2019年6月21日
 作    者  : chenxianyue
 修改内容  : 创建
*****************************************************************************/
void xWriteCH376Data(UINT8 mData)
{
	CH376_DATA_DAT_OUT(mData);  	/* 向CH376的并口输出数据 */
	CH376_DATA_DIR_OUT();  			/* 设置并口方向为输出 */
	CH376_A0 = 0;
	CH376_CS = 0;
	CH376_WR = 0;  					/* 输出有效写控制信号, 写CH376芯片的数据端口 */
	CH376_CS = 0;  					/* 该操作无意义,仅作延时,CH376要求读写脉冲宽度大于40nS */
	CH376_WR = 1;  					/* 输出无效的控制信号, 完成操作CH376芯片 */
	CH376_CS = 1;
	CH376_DATA_DIR_IN();  			/* 禁止数据输出 */
	mDelay0_5uS();  				/* 确保读写周期大于0.6uS */
}

/*****************************************************************************
 函 数 名  : xReadCH376Data
 功能描述  : 从CH376读数据
 输入参数  : 无 
 输出参数  : 无
 修改历史  :
 日    期  : 2019年6月21日
 作    者  : chenxianyue
 修改内容  : 创建
*****************************************************************************/
UINT8 xReadCH376Data(void)
{
	UINT8 mData;
	mDelay0_5uS();  				/* 确保读写周期大于0.6uS */
	CH376_DATA_DIR_IN();  			/* 设置并口方向为输入 */
	CH376_A0 = 0;
	CH376_CS = 0;
	CH376_RD = 0;  					/* 输出有效读控制信号, 读CH376芯片的数据端口 */
	CH376_CS = 0;
	CH376_CS = 0;
	CH376_CS = 0;
	CH376_CS = 0;  					/* 该操作作指令延时以确保并口有足够时间输入数据 */
	mData = CH376_DATA_DAT_IN();	/* 从CH376的并口输入数据 */
	CH376_RD = 1;					/* 输出无效的控制信号, 完成操作CH376芯片 */
	CH376_CS = 1;
	return mData;
}