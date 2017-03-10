/** @file
 *  @brief 多进程配置文件
 *  @author Steven Wan
 *  @date 2015/03/15
 *  @version 0.1
 *  1、进程宏名定义
 *  2、注意：增减进程后一定要修改“进程总数宏”的定义
 */
#ifndef __MP_CFG_PROCESS_H__
#define __MP_CFG_PROCESS_H__

/** @brief 进程服务名称定义 */
#define MP_PROCESS_HTTX1	"httx1"
#define MP_PROCESS_HTTX2	"httx2"
#define MP_PROCESS_HTTX3	"httx3"
#define MP_PROCESS_DBCJ		"dbcj"
#define MP_PROCESS_PPP		"ppp"
#define MP_PROCESS_LCD		"lcd"
#define MP_PROCESS_JLCY		"jlcy"
#define MP_PROCESS_FHKZ		"fhkz"

/** @brief 进程总数宏
 *  @remarks 增减进程后一定要修改本宏的定义，
 *  它的值必须与gMP_ProcessAll数组中进程个数相等，否则将出错。PLEASE BE CAREFUL.
 */
#define MP_PROCESS_CNT	(1/*daemon*/+1/*httx1*/+1/*httx2*/+1/*httx3*/+1/*dbcj*/+1/*ppp*/+1/*lcd*/+1/*jlcy*/+1/*fhkz*/)


#endif//__MP_CFG_PROCESS_H__

