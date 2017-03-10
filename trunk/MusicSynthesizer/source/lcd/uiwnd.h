/** @file
 *  @brief ���ز��������ڶ���
 *  @author Steven Wan
 *  @date 2012/01/09
 *  @version 0.1
 *  �û����ڱ���̳�C_UIMWND
 */
#ifndef __LCD_UIWND_H__
#define __LCD_UIWND_H__
#include "sguiwnd.h"
#include "sguidlog.h"

#define BTNW    32  /** button��ťͳһ���*/
#define BTNH    16  /** button��ťͳһ�߶�*/
#define BTNRECT(x,y)	{x,y,BTNW,BTNH}	/**	button��ťͳһ��С�趨*/
#define STRINGX 0  /** �ַ�����ʾ��ʼX����*/
#define STRINGW 160  /** ���Գ�*/
#define HEADIDEL	8	/**	ҳü��ҳ�ۼ��*/
#define LINEHEIGHT	18	/**	�и�*/
#define CHARWIDTH	12	/** �ֿ�*/
#define LINEXY(n)   (n*LINEHEIGHT+HEADIDEL)
#define LINEOFSCRT  6   /** ��������ʾ�ַ�����*/


class C_UIMWND:public C_SGUIWND
{
public:
	C_UIMWND(void){/*m_WndRect.y=20;m_WndRect.h-=20;*/};
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	/** @brief �����źŻ�ȡ
	 *  @return 0-4 
	 */
	int GetSignal(void);
	/** @brief ͨѶ״̬��ȡ
	 *  @return 0-2 GprsStatus 0δ��½1����2ͨѶ
	 *  @return 3-5 CdmaStatus 3δ��½4����5ͨѶ
	 *  @return 6-7 LanStatus 6δ��½7����8ͨѶ
	 */
	int GetCommunicationStatus(void);
	/** @brief �¼�״̬��ȡ
	 *  @return -1 ��ʾ��ð��
	 *  @return 0 ����ʾ
	 *  @return >0 �¼�����
	 */
	int GetEventIndicator(void);
	/** @brief ��ؼ����ȡ
	 *  @return 0-5
	 */
	int GetBatteryLevel(void);
	/** @brief ȡ��ǰ��ʾ��Ϣ�Ĳ������
	 *  @return 0 ��
	 *  @return >0 ����������
	 */
	int GetDisplayedMP(void);

	/** @brief delete �ؼ�
	 *  @param[in] idMin ��С�ؼ�ID��
	 *  @param[in] idMax ���ؼ�ID��
	 *  @return 0
	 *  @note �ؼ���������new������
	 */
	int DeleteWndCtrl(int idMin, int idMax);
};


/** @brief ������֤��
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

/** @brief ������֤
 */
class C_PWD
{
public:
	/** @brief ������֤
	 *  @return true ��֤ͨ��;false δͨ��
	 *  @remarks ��Ϣ��ͶԻ����ܵ��ñ�����
	 */
	static bool Certification(void);
	/** @brief ����������֤��־
	 */
	static void SetCertification(bool fOk);

private:
	static bool m_fCertification;
};

#endif//__LCD_UIWND_H__

