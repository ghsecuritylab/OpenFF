#include <linux/config.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/resource.h>

#include <linux/console.h>
#include <asm/serial.h>

#include <linux/tty.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/serial_8250.h>
#include <linux/miscdevice.h>

#include <asm/mach-ar7240/ar7240.h>
#include <asm/delay.h>

// one tick means 1 ms (10^-3)
// one watchdog tick means 10 nsec (10^-9)
#define UPDATE_TIMER (HZ/10)
#define WDTIMEOUT (8*100*1000*1000) // 4 sec (unit : nsec)
static struct timer_list wd_timer;

static void update_watchdog_count(unsigned long data)
{
	ar7240_reg_wr(AR7240_WATCHDOG_TMR, WDTIMEOUT);
	mod_timer(&wd_timer, jiffies + UPDATE_TIMER);
}

int init(void)
{
	ar7240_reg_wr(AR7240_WATCHDOG_TMR, WDTIMEOUT);
	ar7240_reg_wr(AR7240_WATCHDOG_TMR_CONTROL, 0x3); //full chip reset

	init_timer(&wd_timer);
	wd_timer.data = 0;
	wd_timer.function = update_watchdog_count;
	mod_timer(&wd_timer, jiffies + UPDATE_TIMER);

	return 0;
}

void fini(void)
{
	ar7240_reg_wr(AR7240_WATCHDOG_TMR_CONTROL, 0x0); //do nothing
	del_timer(&wd_timer);
	return;
}

module_init(init);
module_exit(fini);


