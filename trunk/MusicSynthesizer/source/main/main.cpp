/** @file
 *  @brief ÄÏÍøÖ÷³ÌĞò
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 *  @see appcmd.h
 */
#include "nwappcmd.h"

int main(int argc, char *argv[])
{
	C_NwAppCmd NwAppCmd(argc,argv);
	return NwAppCmd.ExecuteCmd();
}

