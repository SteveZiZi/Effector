/** @file
 *  @brief 产品版本定义
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#ifndef __NW_VERSION_H__
#define __NW_VERSION_H__

/** @brief 主程序版本号及发布日期宏定义
 *  @note
 *  以下各版本号的数字最大不得超过 9
 */
#define CONS_MAJOR_VERSION	"0"						//主版本号
#define CONS_MINOR_VERSION	"0"						//次版本号
#define CONS_FIX_VERSION	"3"						//修正版本号
#define CONS_SW_REAL_DATE	"2012-06-15"		//发布日期，格式: YYYY-MM-DD

#define CONS_MAIN_VERSION CONS_MAJOR_VERSION"."CONS_MINOR_VERSION CONS_FIX_VERSION

/** @brief 规约版本号，本规约规定为0100
 */
#define CONS_MSRULE_VER  0x0100

/** @brief 硬件版本号 */
#define CONS_HARD_VERSION	"0.01"
#define CONS_HW_REAL_DATE	"2010-08-25"		//发布日期，格式: YYYY-MM-DD

/** @brief 用户版本号
 */
#define CONS_SOFT_VERSION CONS_MAIN_VERSION
#define CONS_SW_RELEASE_DATE CONS_SW_REAL_DATE

#endif//__NW_VERSION_H__


