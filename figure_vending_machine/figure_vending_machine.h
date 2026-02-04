#ifndef _FIGURE_VENDING_MACHINE_H
#define _FIGURE_VENDING_MACHINE_H
#include "figure_vending_machine/login_screen.h"    
#include "figure_vending_machine/main_screen.h"
#include "figure_vending_machine/user_info_mangm.h"
#include"figure_vending_machine/goods_mangm.h"
#include "tools/tools.h"

#include"figure_vending_machine/screen_objs.h"  //标准库
/*贩卖机主控制程序
*/
int figure_vending_machine();
void init_base_widgets();
#endif