/** @file
 *  @brief 配置文件基础类定义
 *  @author Steven Wan
 *  @date 2015/03/15
 *  @version 0.1
 *  1、提供用户配置文件基础类接口
 *  @see foundation/confuse
 */
#ifndef __MP_INI_FILEB_H__
#define __MP_INI_FILEB_H__
#include "confuse.h"
#include "basedef.h"

/** @brief 配置项对应表 */
struct S_CFGITEM
{
	int m_val;
	const char *m_pkey;
};

/** @class
 *  @brief 初始化文件分析基础类
 *  @see foundation/confuse/confuse.h
 *  @remarks INI文件分析采用此类进行解析
 */
class C_IniBase
{
public:
	/** @fn C_IniBase::ParseSetFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 *  @note 查找顺序:先找用户文件再找缺省文件
	 */
	static int ParseSetFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseSystemFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 */
	static int ParseSystemFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseRegisterFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 */
	static int ParseRegisterFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::SaveSetFile
	 *  @brief 保存配置文件
	 *  @param[in] strBuffer 配置信息串
	 *  @param[in] fname 文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSetFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveSystemFile
	 *  @brief 保存系统文件
	 *  @param[in] strBuffer 配置信息串
	 *  @param[in] fname 文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSystemFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveRegisterFile
	 *  @brief 保存系统文件
	 *  @param[in] strBuffer 配置信息串
	 *  @param[in] fname 文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveRegisterFile(const string &strBuffer, const char *fname);

	/** @fn C_IniBase::MakeValue
	 *  @brief 取索引号
	 *  @param[in] pkey 指定名称
	 *  @param[in] pkeys 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return >=0 有效索引 -1 无效索引
	 */
	static int MakeValue(const char *pkey, const char **pkeys, int size);
	/** @fn C_IniBase::MakeKey
	 *  @brief 取指定名称
	 *  @param[in] idx 索引号
	 *  @param[in] pkeys 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return NULL 无效指针；!NULL 数组项指针
	 */
	static const char *MakeKey(int idx, const char **pkeys, int size);
	/** @fn C_IniBase::GetBoolKey
	 *  @brief 取bool串：true/false
	 *  @param[in] fbool true/false
	 *  @return 指向"true|false"的指针
	 */
	static const char *GetBoolKey(bool fbool);

	/** @fn C_IniBase::MakeKey
	 *  @brief 取指定名称
	 *  @param[in] val 指定值
	 *  @param[in] pCfgItem 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return "unknow"或Key名称//NULL 无效指针；!NULL 数组项指针
	 */
	static const char *MakeKey(const S_CFGITEM *pCfgItem, S_WORD size, int val);
	/** @fn C_IniBase::MakeValue
	 *  @brief 取索引号
	 *  @param[in] pkey 指定名称
	 *  @param[in] pCfgItem 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return >=0 有效值 -1 无效值
	 */
	static int MakeValue(const S_CFGITEM *pCfgItem, S_WORD size, const char *pkey);

private:
	/** @fn C_IniBase::ParseCfgFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] cfgFile:文件名(带路径)
	 *  @return 0:successful, -1:failed
	 */
	static int ParseCfgFile(cfg_t* cfg, const char *cfgFile);
	/** @fn C_IniBase::SaveCfgFile
	 *  @brief 保存配置文件
	 *  @param[in] strBuffer:配置信息串
	 *  @param[in] cfgFile:文件名(带路径)
	 *  @return 0:successful, -1:failed
	 */
	static int SaveCfgFile(const string &strBuffer, const char *cfgFile);
};


#endif //__MP_INI_FILEB_H__
