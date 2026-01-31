#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"


#include "tools/tools.h"
#include "figure_vending_machine/figure_vending_machine.h"
#include "figure_vending_machine/main_screen.h"
#include "figure_vending_machine/login_screen.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include<stdio.h>