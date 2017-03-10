/** @file
 *  @brief 本地操作父窗口定义
 *  @author Steven Wan
 *  @date 2012/01/09
 *  @version 0.1
 *  用户窗口必须继承C_UIMWND
 */
#ifndef __LCD_UIWND_H__
#define __LCD_UIWND_H__
#include "sguiwnd.h"
#include "sguidlog.h"

#define BTNW    32  /** button按钮统一宽度*/
#define BTNH    16  /** button按钮统一高度*/
#define BTNRECT(x,y)	{x,y,BTNW,BTNH}	/**	button按钮统一大小设定*/
#define STRINGX 0  /** 字符串显示起始X坐标*/
#define STRINGW 160  /** 反显长*/
#define HEADIDEL	8	/**	页眉与页眼间距*/
#define LINEHEIGHT	18	/**	行高*/
#define CHARWIDTH	12	/** 字宽*/
#define LINEXY(n)   (n*LINEHEIGHT+HEADIDEL)
#define LINEOFSCRT  6   /** 单屏可显示字符行数*/


class C_UIMWND:public C_SGUIWND
{
public:
	C_UIMWND(void){/*m_WndRect.y=20;m_WndRect.h-=20;*/};
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	/** @brief 无线信号获取
	 *  @return 0-4 
	 */
	int GetSignal(void);
	/** @brief 通讯状态获取
	 *  @return 0-2 GprsStatus 0未登陆1空闲2通讯
	 *  @return 3-5 CdmaStatus 3未登陆4空闲5通讯
	 *  @return 6-7 LanStatus 6未登陆7空闲8通讯
	 */
	int GetCommunicationStatus(void);
	/** @brief 事件状态获取
	 *  @return -1 显示感冒号
	 *  @return 0 无显示
	 *  @return >0 事件编码
	 */
	int GetEventIndicator(void);
	/** @brief 电池级别获取
	 *  @return 0-5
	 */
	int GetBatteryLevel(void);
	/** @brief 取当前显示信息的测量点号
	 *  @return 0 无
	 *  @return >0 正常测量点
	 */
	int GetDisplayedMP(void);

	/** @brief delete 控件
	 *  @param[in] idMin 最小控件ID号
	 *  @param[in] idMax 最大控件ID号
	 *  @return 0
	 *  @note 控件必需是用new创建的
	 */
	int DeleteWndCtrl(int idMin, int idMax);
};


/** @brief 密码验证框
 */
class C_PWDDLOG:public C_SGUIDLG
{
public:
	C_PWDDLOG(int pwd);
	virtual ~C_PWDDLOG();

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int CreateCtrl(void);
	int DestroyCtrl(void);
	
protected:
	int m_pwd;
	S_DWORD m_DelayMS;
};

/** @brief 密码验证
 */
class C_PWD
{
public:
	/** @brief 密码验证
	 *  @return true 验证通过;false 未通过
	 *  @remarks 消息框和对话框不能调用本函数
	 */
	static bool Certification(void);
	/** @brief 设置密码验证标志
	 */
	static void SetCertification(bool fOk);

private:
	static bool m_fCertification;
};

#endif//__LCD_UIWND_H__

