#ifndef EXPORT_SYMTAB
#define EXPORT_SYMTAB
#endif

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <asm/types.h>
#include <asm/irq.h>
#include <asm/delay.h>
#include <asm/system.h>

#include "ar7240.h"

#define WNR612_LED	1

#define AR7240_FACTORY_RESET		0x89ABCDEF
#define AR7240_HANDLE_KASSERT       (AR7240_FACTORY_RESET+1)
#define REASON_RESET 0x1
#define REASON_KASSERT 0x2

static atomic_t ar7240_fr_status = ATOMIC_INIT(0);
static volatile int ar7240_fr_opened=0;
static wait_queue_head_t ar7240_fr_wq;
struct timer_list  os_button_timer_t;
struct timer_list  os_test_led_timer_t;
struct timer_list  os_wps_err_led_timer_t;
static int reset_button_count = 0;
static int wps_led_blink_time = 0;

/* AR7240 GPIO */
#define TEST_LED_GPIO		1	//amber
#define POWER_LED_GPIO		11	//green

/* AR9285 GPIO */
#define PUSHBUTTON_GPIO		6
#define RESETBUTTON_GPIO	7
#define WLAN_SWITCH_GPIO	8
#define POLL_INTERVAL		100
#define PUSH_BUTTON_TIME	100
#define RESET_BUTTON_TIME	5000
#define WLAN_SWITCH_TIME	100
#define RESET_BUTTON_COUNT	(RESET_BUTTON_TIME/POLL_INTERVAL)
#define WPS_LED_ERR_BLINK_TIME	100	//millisecond

#define frdbg printk

#define WPS_LED_OFF 1
#define WPS_LED_ON  0

#define SIMPLE_CONFIG_OFF     1
#define SIMPLE_CONFIG_ON      2
#define SIMPLE_CONFIG_BLINK   3

#define TEST_LED_ONTIME		250	//millisecond
#define TEST_LED_OFFTIME	750	//millisecond

#define OS_TIMER_FUNC(_fn)                      \
    void _fn(unsigned long timer_arg)

#define OS_GET_TIMER_ARG(_arg, _type)           \
    (_arg) = (_type)(timer_arg)

#define OS_INIT_TIMER(_osdev, _timer, _fn, _arg) \
do {                                             \
        init_timer(_timer);                      \
        (_timer)->function = (_fn);              \
        (_timer)->data = (unsigned long)(_arg);  \
} while (0)

#define OS_SET_TIMER(_timer, _ms)       mod_timer(_timer, jiffies + ((_ms)*HZ)/1000)

#define OS_CANCEL_TIMER(_timer)         del_timer(_timer)


/*
 * GPIO interrupt stuff
 */
typedef enum {
    INT_TYPE_EDGE,
    INT_TYPE_LEVEL,
}ar7240_gpio_int_type_t;

typedef enum {
    INT_POL_ACTIVE_LOW,
    INT_POL_ACTIVE_HIGH,
}ar7240_gpio_int_pol_t;


/*
** Simple Config stuff
*/

#if !defined(IRQ_NONE)
#define IRQ_NONE
#define IRQ_HANDLED
#endif /* !defined(IRQ_NONE) */


typedef irqreturn_t(*sc_callback_t)(int, void *, struct pt_regs *);

static sc_callback_t registered_cb = NULL;
static void *cb_arg;
static volatile int ignore_pushbutton = 0;
static struct proc_dir_entry *simple_config_entry = NULL;
static struct proc_dir_entry *wps_entry = NULL;
static struct proc_dir_entry *simulate_push_button_entry = NULL;
static struct proc_dir_entry *simulate_push_button_s_entry = NULL;
static struct proc_dir_entry *simulate_reset_button_s_entry = NULL;
static struct proc_dir_entry *simulate_wlan_switch_s_entry = NULL;
static struct proc_dir_entry *simple_config_led_entry = NULL;
static struct proc_dir_entry *simple_config_led_err_entry = NULL;
static int wps_led_blinking = 0;
static unsigned char wakeup_reason = 0;
 /* 
  * By default KASSERT is enabled 
  * Means KASSERT shall cause panic
  */
static unsigned char enable_kassert = 1;

static void ar7240_misc_wakeup_process(void)
{
      wakeup_reason = REASON_KASSERT;
      wake_up(&ar7240_fr_wq);
}

void ar7xxx_misc_wakeup_process(void)
{
    ar7240_misc_wakeup_process();
}
EXPORT_SYMBOL(ar7xxx_misc_wakeup_process);

void ar7xxx_set_kassert(unsigned char enable)
{
    enable_kassert = enable;
}
EXPORT_SYMBOL(ar7xxx_set_kassert);

unsigned char ar7xxx_is_kassert_enabled(void)
{
    return enable_kassert;
}
EXPORT_SYMBOL(ar7xxx_is_kassert_enabled);

static int wps_led_err_state = 0;

extern void ar9285GpioCfgOutput(unsigned int gpio);
extern void ar9285GpioCfgInput(unsigned int gpio);
extern void ar9285GpioSet(unsigned int gpio, int val);
extern u32 ar9285GpioGet(unsigned int gpio);

#ifdef JUMPSTART_GPIO
void ar7240_gpio_config_int(int gpio,
                       ar7240_gpio_int_type_t type,
                       ar7240_gpio_int_pol_t polarity)
{
    u32 val;

    /*
     * allow edge sensitive/rising edge too
     */
    if (type == INT_TYPE_LEVEL) {
        /* level sensitive */
        ar7240_reg_rmw_set(AR7240_GPIO_INT_TYPE, (1 << gpio));
    }
    else {
       /* edge triggered */
       val = ar7240_reg_rd(AR7240_GPIO_INT_TYPE);
       val &= ~(1 << gpio);
       ar7240_reg_wr(AR7240_GPIO_INT_TYPE, val);
    }

    if (polarity == INT_POL_ACTIVE_HIGH) {
        ar7240_reg_rmw_set (AR7240_GPIO_INT_POLARITY, (1 << gpio));
    }
    else {
       val = ar7240_reg_rd(AR7240_GPIO_INT_POLARITY);
       val &= ~(1 << gpio);
       ar7240_reg_wr(AR7240_GPIO_INT_POLARITY, val);
    }

    ar7240_reg_rmw_set(AR7240_GPIO_INT_ENABLE, (1 << gpio));
}

void
ar7240_gpio_config_output(int gpio)
{
    ar7240_reg_rmw_set(AR7240_GPIO_OE, (1 << gpio));
}

void
ar7240_gpio_config_input(int gpio)
{
    ar7240_reg_rmw_clear(AR7240_GPIO_OE, (1 << gpio));
}
#endif

void
ar7240_gpio_out_val(int gpio, int val)
{
    if (val & 0x1) {
        ar7240_reg_rmw_set(AR7240_GPIO_OUT, (1 << gpio));
    }
    else {
        ar7240_reg_rmw_clear(AR7240_GPIO_OUT, (1 << gpio));
    }
}

int
ar7240_gpio_in_val(int gpio)
{
    return((1 << gpio) & (ar7240_reg_rd(AR7240_GPIO_IN)));
}

static void
ar7240_gpio_intr_enable(unsigned int irq)
{
    ar7240_reg_rmw_set(AR7240_GPIO_INT_MASK,
                      (1 << (irq - AR7240_GPIO_IRQ_BASE)));
}

static void
ar7240_gpio_intr_disable(unsigned int irq)
{
    ar7240_reg_rmw_clear(AR7240_GPIO_INT_MASK, 
                        (1 << (irq - AR7240_GPIO_IRQ_BASE)));
}

static unsigned int
ar7240_gpio_intr_startup(unsigned int irq)
{
	ar7240_gpio_intr_enable(irq);
	return 0;
}

static void
ar7240_gpio_intr_shutdown(unsigned int irq)
{
	ar7240_gpio_intr_disable(irq);
}

static void
ar7240_gpio_intr_ack(unsigned int irq)
{
	ar7240_gpio_intr_disable(irq);
}

static void
ar7240_gpio_intr_end(unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED | IRQ_INPROGRESS)))
		ar7240_gpio_intr_enable(irq);
}

static void
ar7240_gpio_intr_set_affinity(unsigned int irq, cpumask_t mask)
{
	/*
     * Only 1 CPU; ignore affinity request
     */
}

struct hw_interrupt_type ar7240_gpio_intr_controller = {
	"AR7240 GPIO",
	ar7240_gpio_intr_startup,
	ar7240_gpio_intr_shutdown,
	ar7240_gpio_intr_enable,
	ar7240_gpio_intr_disable,
	ar7240_gpio_intr_ack,
	ar7240_gpio_intr_end,
	ar7240_gpio_intr_set_affinity,
};

void
ar7240_gpio_irq_init(int irq_base)
{
	int i;

	for (i = irq_base; i < irq_base + AR7240_GPIO_IRQ_COUNT; i++) {
		irq_desc[i].status  = IRQ_DISABLED;
		irq_desc[i].action  = NULL;
		irq_desc[i].depth   = 1;
		irq_desc[i].handler = &ar7240_gpio_intr_controller;
	}
}


void register_simple_config_callback (char *cbname, void *callback, void *arg)
{
    registered_cb = (sc_callback_t) callback;
    cb_arg = arg;
}
EXPORT_SYMBOL(register_simple_config_callback);

void unregister_simple_config_callback (char *cbname)
{
    registered_cb = NULL;
    cb_arg = NULL;
}
EXPORT_SYMBOL(unregister_simple_config_callback);

/* test led(amber led of power) blinking */
static OS_TIMER_FUNC(test_led_action)
{
	static int test_led_status=0;
#if (WNR612_LED)	
	if (test_led_status) {
		ar7240_gpio_out_val(POWER_LED_GPIO, 1);	//off
		ar7240_gpio_out_val(TEST_LED_GPIO, 1);	//off
		test_led_status = 0;
		OS_SET_TIMER(&os_test_led_timer_t, TEST_LED_OFFTIME);
	} else {
		ar7240_gpio_out_val(POWER_LED_GPIO, 0); //on
		ar7240_gpio_out_val(TEST_LED_GPIO, 1);  //of
		test_led_status = 1;
		OS_SET_TIMER(&os_test_led_timer_t, TEST_LED_ONTIME);
	}
#else
	if (test_led_status) {
		ar7240_gpio_out_val(POWER_LED_GPIO, 1);	//off
		ar7240_gpio_out_val(TEST_LED_GPIO, 1);	//off
		test_led_status = 0;
		OS_SET_TIMER(&os_test_led_timer_t, TEST_LED_OFFTIME);
	} else {
		ar7240_gpio_out_val(POWER_LED_GPIO, 1); //off
		ar7240_gpio_out_val(TEST_LED_GPIO, 0);  //on
		test_led_status = 1;
		OS_SET_TIMER(&os_test_led_timer_t, TEST_LED_ONTIME);
	}
#endif
}

#ifdef JUMPSTART_GPIO 
int ar7240_simple_config_invoke_cb(int simplecfg_only, int irq_enable,
                                   int cpl, struct pt_regs *regs)
{
    if (simplecfg_only) {
        if (ignore_pushbutton) {
            ar7240_gpio_config_int (JUMPSTART_GPIO, INT_TYPE_LEVEL, INT_POL_ACTIVE_HIGH);
            ignore_pushbutton = 0;
            return IRQ_HANDLED;
        }

        ar7240_gpio_config_int (JUMPSTART_GPIO, INT_TYPE_LEVEL, INT_POL_ACTIVE_LOW);
        ignore_pushbutton = 1;
    }

    if (irq_enable)
        local_irq_enable();

    printk ("\nar7240: calling simple_config callback..\n");

    if (registered_cb) {
        return (registered_cb (cpl, cb_arg, regs));
    }

    return IRQ_HANDLED;

}
/*
 * Irq for front panel SW jumpstart switch
 * Connected to XSCALE through GPIO4
 */
irqreturn_t jumpstart_irq(int cpl, void *dev_id, struct pt_regs *regs)
{
    unsigned int delay;

    if (atomic_read(&ar7240_fr_status))
    {
        local_irq_disable();

#define UDELAY_COUNT 4000

        for (delay = UDELAY_COUNT; delay; delay--) {
            if (ar7240_gpio_in_val(JUMPSTART_GPIO)) {
                break;
            }
            udelay(1000);
        }

        if (!delay) {
            atomic_dec(&ar7240_fr_status);
            /*
             * since we are going to reboot the board, we
             * don't need the interrupt handler anymore,
             * so disable it.
             */
            disable_irq(AR7240_GPIO_IRQn(JUMPSTART_GPIO));
            wakeup_reason = REASON_RESET;
            wake_up(&ar7240_fr_wq);
            printk("\nar7240: factory configuration restored..\n");
            local_irq_enable();
            return IRQ_HANDLED;
        } else {
            return (ar7240_simple_config_invoke_cb(0, 1, cpl, regs));
        }
    }
    else
        return (ar7240_simple_config_invoke_cb(1, 0, cpl, regs));
}
#endif

static int gpio_simple_config_led_err_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
    u_int32_t val;

    if (sscanf(buf, "%d", &val) != 1)
        return -EINVAL;

    if (0 == val) {
	//poped
    } else if (2 == val) {
	//pushed
	wps_led_blinking = 1;
	wps_led_blink_time = 0;
	wps_led_err_state = 2;
	OS_SET_TIMER(&os_wps_err_led_timer_t, WPS_LED_ERR_BLINK_TIME);
    } else if (3 == val) {
	//released
	wps_led_blinking = 1;
	wps_led_blink_time = 5000/WPS_LED_ERR_BLINK_TIME;
	wps_led_err_state = 0;
    }
    return count;
}

static int push_button_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
    return 0;
}

static int push_button_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
    if (registered_cb) {
        registered_cb (0, cb_arg, 0);
    }
    return count;
}

static int wps_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
    if (registered_cb) {
	registered_cb (0, cb_arg, (struct pt_regs *)"info");
    }
    return 0;
}

static int wps_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
    if (0 == strncmp(buf,"pin=",4) || 0 == strncmp(buf,"stop",4)) {
        if (registered_cb) {
            registered_cb (0, cb_arg, (struct pt_regs *)buf);
        }
    }
    return count;
}

typedef enum {
	PUSH_BUTTON_PUSHED = 0,
	PUSH_BUTTON_POPED = 1,
} push_button_state_e;

static push_button_state_e push_button_state = PUSH_BUTTON_POPED;

static int push_button_s_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
    return sprintf(page, "%d\n", push_button_state);
}

static int push_button_s_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
	u_int32_t val;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

	if (PUSH_BUTTON_POPED == val) {
		push_button_state = PUSH_BUTTON_POPED;
	}
	return count;
}

typedef enum {
	RESET_BUTTON_PUSHED_GT5S = 0,
	RESET_BUTTON_POPED = 1,
	RESET_BUTTON_PUSHED_LT5S = 2,
} reset_button_state_e;

static reset_button_state_e reset_button_state = RESET_BUTTON_POPED;

static int reset_button_s_read (char *page, char **start, off_t off, int count, int *eof, void *data)
{
    return sprintf(page, "%d\n", reset_button_state);
}

static int reset_button_s_write (struct file *file, const char *buf, unsigned long count, void *data)
{
	u_int32_t val;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

	if (RESET_BUTTON_POPED == val) {
		reset_button_state = RESET_BUTTON_POPED;
		reset_button_count = 0;
	}
	return count;
}

typedef enum {
	WLAN_SWITCH_PUSHED = 0,
	WLAN_SWITCH_POPED = 1,
} wlan_switch_state_e;

static wlan_switch_state_e wlan_switch_state = WLAN_SWITCH_POPED;

static int wlan_switch_s_read (char *page, char **start, off_t off, int count, int *eof, void *data)
{
    return sprintf(page, "%d\n", wlan_switch_state);
}

static int wlan_switch_s_write (struct file *file, const char *buf, unsigned long count, void *data)
{
	u_int32_t val;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

	if (WLAN_SWITCH_POPED == val)
		wlan_switch_state = WLAN_SWITCH_POPED;
	return count;
}

static OS_TIMER_FUNC(button_detect)
{
	static int test_led_status=0;
	static int wireless_on_off_realese = 1;
        static int wireless_on_off_count = 0;
        static int push_button_realese = 1;
        static int push_button_count = 0;

	if (!ar9285GpioGet(PUSHBUTTON_GPIO)) {	//push button is pressed
                push_button_count++;
                if (1 < push_button_count) {    //push button should be pressed in and stayed 0.1s before taking effect.
                        if (1 == push_button_realese) {
                                push_button_realese = 0;
		                push_button_state = PUSH_BUTTON_PUSHED;
                        }
                }
	} else {
                push_button_count = 0;
                if (0 == push_button_realese)
                        push_button_realese = 1;
		if (2 == wps_led_err_state) {	//wps is disable, and pushbutton is released
			gpio_simple_config_led_err_write(NULL, "3", 1, NULL);	//blinking wps led more than 5 seconds
		}
	}

	if (!ar9285GpioGet(RESETBUTTON_GPIO)) {	//reset button is pressed
		reset_button_count++;
		if (!test_led_status) {		//turn on amber led of power, off green led
#if (WNR612_LED)	
			ar7240_gpio_out_val(POWER_LED_GPIO, 0); //on
			ar7240_gpio_out_val(TEST_LED_GPIO, 1);  //off		
#else
			ar7240_gpio_out_val(POWER_LED_GPIO, 1); //off
			ar7240_gpio_out_val(TEST_LED_GPIO, 0);  //on
#endif
			OS_SET_TIMER(&os_test_led_timer_t, RESET_BUTTON_TIME);	//reset button is pressed and keep on 5s, blink led
			test_led_status = 1;
		}
	} else {
		if (RESET_BUTTON_COUNT <= reset_button_count) {
			reset_button_state = RESET_BUTTON_PUSHED_GT5S;	//reset button is pressed more than 5 seconds
		} else if ((0 < reset_button_count) && (RESET_BUTTON_COUNT > reset_button_count)) {
			reset_button_state = RESET_BUTTON_PUSHED_LT5S;	//reset button is pressed less than 5 seconds
		}
		if (test_led_status) {
			OS_CANCEL_TIMER(&os_test_led_timer_t);
			ar7240_gpio_out_val(POWER_LED_GPIO, 1);	//off
			ar7240_gpio_out_val(TEST_LED_GPIO, 1);	//off
			test_led_status = 0;
		}
		reset_button_count = 0;
	}

	if (!ar9285GpioGet(WLAN_SWITCH_GPIO)) {	//wlan on/off switch is pushed
                wireless_on_off_count++;
                if (1 < wireless_on_off_count) {//wlan on/off button should be pressed in and stayed 0.1s before taking effect.
		        if (1 == wireless_on_off_realese) {
			        wireless_on_off_realese = 0;
			        wlan_switch_state = WLAN_SWITCH_PUSHED;
		        }
                }
	} else {
                wireless_on_off_count = 0;
		if (0 == wireless_on_off_realese)
			wireless_on_off_realese = 1;
	}
	OS_SET_TIMER(&os_button_timer_t, POLL_INTERVAL);
}

typedef enum {
        LED_STATE_OFF   =       1,
        LED_STATE_ON    =       2,
        LED_STATE_BLINKING =    3,
} led_state_e;

static led_state_e simple_config_led_state = LED_STATE_OFF;

static int gpio_simple_config_led_read (char *page, char **start, off_t off,
               int count, int *eof, void *data)
{
    return sprintf (page, "%d\n", simple_config_led_state);
}

static int gpio_simple_config_led_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
    u_int32_t val;

    if (sscanf(buf, "%d", &val) != 1)
        return -EINVAL;

	if ((val == SIMPLE_CONFIG_BLINK) && !wps_led_blinking)  /* wps LED blinking */
	{
		wps_led_blinking = 1 ;
		simple_config_led_state = SIMPLE_CONFIG_BLINK ;
		ar9285GpioSet(WPS_LED_GPIO, WPS_LED_ON);
	}
	else if (val == SIMPLE_CONFIG_ON) /* WPS Success  */
	{
		wps_led_blinking = 0 ;
		simple_config_led_state = SIMPLE_CONFIG_ON ;
		ar9285GpioSet(WPS_LED_GPIO, WPS_LED_ON);
	}
	else if (val == SIMPLE_CONFIG_OFF)  /* WPS failed */
	{
		wps_led_blinking = 0 ;
		simple_config_led_state = SIMPLE_CONFIG_OFF ;
		ar9285GpioSet(WPS_LED_GPIO, WPS_LED_OFF);
	}

    return count;
}

static OS_TIMER_FUNC(wps_err_led_blink)
{
   static int WPSled = WPS_LED_ON,sec = 0;
   ar9285GpioSet(WPS_LED_GPIO,WPSled);
   WPSled=!WPSled;
   if (0 == wps_led_blinking) {
   	sec = 0;
   	wps_led_blink_time = 0;
	OS_CANCEL_TIMER(&os_wps_err_led_timer_t);
	ar9285GpioSet(WPS_LED_GPIO, WPS_LED_OFF);
   }
   if (0 == wps_led_blink_time) {
	OS_SET_TIMER(&os_wps_err_led_timer_t, WPS_LED_ERR_BLINK_TIME);
   } else {
	sec++ ;
	if(sec < wps_led_blink_time) {
		OS_SET_TIMER(&os_wps_err_led_timer_t, WPS_LED_ERR_BLINK_TIME);
	} else {
		sec = 0;
		wps_led_blinking = 0;
		wps_led_blink_time = 0;
		OS_CANCEL_TIMER(&os_wps_err_led_timer_t);
		ar9285GpioSet(WPS_LED_GPIO, WPS_LED_OFF);
	}
   }
}

static int gpio_simple_config_led_err_read (char *page, char **start, off_t off,
               int count, int *eof, void *data)
{
    return wps_led_err_state;
}


static int create_simple_config_led_proc_entry (void)
{
    if (simple_config_entry != NULL) {
        printk ("Already have a proc entry for /proc/simple_config!\n");
        return -ENOENT;
    }

    simple_config_entry = proc_mkdir("simple_config", NULL);
    if (!simple_config_entry)
        return -ENOENT;

    simulate_push_button_entry = create_proc_entry ("push_button", 0644, simple_config_entry);
    if (!simulate_push_button_entry)
        return -ENOENT;
    simulate_push_button_entry->write_proc = push_button_write;
    simulate_push_button_entry->read_proc = push_button_read;

    wps_entry = create_proc_entry ("wps", 0644, simple_config_entry);
    if (!wps_entry)
    	return -ENOENT;
    wps_entry->write_proc = wps_write;
    wps_entry->read_proc = wps_read;

    simulate_push_button_s_entry = create_proc_entry ("push_button_s", 0644, simple_config_entry);
    if (!simulate_push_button_s_entry)
        return -ENOENT;
    simulate_push_button_s_entry->write_proc = push_button_s_write;
    simulate_push_button_s_entry->read_proc = push_button_s_read;

    simulate_reset_button_s_entry = create_proc_entry ("reset_button_s", 0644, simple_config_entry);
    if (!simulate_reset_button_s_entry)
        return -ENOENT;
    simulate_reset_button_s_entry->write_proc = reset_button_s_write;
    simulate_reset_button_s_entry->read_proc = reset_button_s_read;

    simulate_wlan_switch_s_entry = create_proc_entry ("wlan_switch_s", 0644, simple_config_entry);
    if (!simulate_wlan_switch_s_entry)
        return -ENOENT;
    simulate_wlan_switch_s_entry->write_proc = wlan_switch_s_write;
    simulate_wlan_switch_s_entry->read_proc = wlan_switch_s_read;

    simple_config_led_entry = create_proc_entry ("simple_config_led", 0644, simple_config_entry);
    if (!simple_config_led_entry)
        return -ENOENT;
    simple_config_led_entry->write_proc = gpio_simple_config_led_write;
    simple_config_led_entry->read_proc = gpio_simple_config_led_read;

    simple_config_led_err_entry = create_proc_entry ("simple_config_err_led", 0644, simple_config_entry);
    if (!simple_config_led_entry)
        return -ENOENT;
    simple_config_led_err_entry->write_proc = gpio_simple_config_led_err_write;
    simple_config_led_err_entry->read_proc = gpio_simple_config_led_err_read;

	ar9285GpioCfgOutput(WPS_LED_GPIO);
	ar9285GpioSet(WPS_LED_GPIO, WPS_LED_OFF);

	ar9285GpioCfgInput(PUSHBUTTON_GPIO);
	ar9285GpioCfgInput(RESETBUTTON_GPIO);
	ar9285GpioCfgInput(WLAN_SWITCH_GPIO);
	OS_INIT_TIMER(NULL, &os_button_timer_t, button_detect, &os_button_timer_t);
	OS_SET_TIMER(&os_button_timer_t, POLL_INTERVAL);

	OS_INIT_TIMER(NULL, &os_test_led_timer_t, test_led_action, &os_test_led_timer_t);
	OS_INIT_TIMER(NULL, &os_wps_err_led_timer_t, wps_err_led_blink, &os_wps_err_led_timer_t);

    return 0;
}

static int
ar7240fr_open(struct inode *inode, struct file *file)
{
	if (MINOR(inode->i_rdev) != FACTORY_RESET_MINOR) {
		return -ENODEV;
	}

	if (ar7240_fr_opened) {
		return -EBUSY;
	}

        ar7240_fr_opened = 1;
	return nonseekable_open(inode, file);
}

static int
ar7240fr_close(struct inode *inode, struct file *file)
{
	if (MINOR(inode->i_rdev) != FACTORY_RESET_MINOR) {
		return -ENODEV;
	}

	ar7240_fr_opened = 0;
	return 0;
}

static ssize_t
ar7240fr_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	return -ENOTSUPP;
}

static ssize_t
ar7240fr_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	return -ENOTSUPP;
}

static int
ar7240fr_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
		unsigned long arg)
{
	int ret = 0;

	switch(cmd) {
		case AR7240_FACTORY_RESET:
        case AR7240_HANDLE_KASSERT:
                        atomic_inc(&ar7240_fr_status);
			sleep_on(&ar7240_fr_wq);
            ret = wakeup_reason;
			break;

		default: ret = -EINVAL;
	}

	return ret;
}


static struct file_operations ar7240fr_fops = {
	read:	ar7240fr_read,
	write:	ar7240fr_write,
	ioctl:	ar7240fr_ioctl,
	open:	ar7240fr_open,
	release:ar7240fr_close
};

static struct miscdevice ar7240fr_miscdev =
{ FACTORY_RESET_MINOR, "Factory reset", &ar7240fr_fops };

int __init ar7240_simple_config_init(void)
{
#ifdef CONFIG_CUS100
	u32 mask = 0;
#endif
    int ret;
#ifdef JUMPSTART_GPIO
    int req;
#endif

    ret = misc_register(&ar7240fr_miscdev);

    if (ret < 0) {
            printk("*** ar7240 misc_register failed %d *** \n", ret);
            return -1;
    }


#ifdef CONFIG_CUS100
	mask = ar7240_reg_rd(AR7240_MISC_INT_MASK);
	ar7240_reg_wr(AR7240_MISC_INT_MASK, mask | (1 << 2)); /* Enable GPIO interrupt mask */
#ifdef JUMPSTART_GPIO
        ar7240_gpio_config_int(JUMPSTART_GPIO, INT_TYPE_LEVEL,INT_POL_ACTIVE_HIGH);
	ar7240_gpio_intr_enable(JUMPSTART_GPIO);
	ar7240_gpio_config_input(JUMPSTART_GPIO);
#endif
#else
#ifdef JUMPSTART_GPIO
	ar7240_gpio_config_input(JUMPSTART_GPIO);
	/* configure Jumpstart GPIO as level triggered interrupt */
	ar7240_gpio_config_int(JUMPSTART_GPIO, INT_TYPE_LEVEL, INT_POL_ACTIVE_LOW);
	printk("%s (%s) JUMPSTART_GPIO: %d\n", __FILE__, __func__, JUMPSTART_GPIO);
#endif
	ar7240_reg_rmw_clear(AR7240_GPIO_FUNCTIONS, (1 << 2));
	ar7240_reg_rmw_clear(AR7240_GPIO_FUNCTIONS, (1 << 16));
	ar7240_reg_rmw_clear(AR7240_GPIO_FUNCTIONS, (1 << 20));
#endif

#ifdef JUMPSTART_GPIO
    req = request_irq (AR7240_GPIO_IRQn(JUMPSTART_GPIO), jumpstart_irq, 0,
                       "SW JUMPSTART/FACTORY RESET", NULL);
    if (req != 0) {
        printk (KERN_ERR "unable to request IRQ for SWJUMPSTART GPIO (error %d)\n", req);
        misc_deregister(&ar7240fr_miscdev);
        ar7240_gpio_intr_shutdown(AR7240_GPIO_IRQn(JUMPSTART_GPIO));
        return -1;
    }
#endif

    init_waitqueue_head(&ar7240_fr_wq);

    create_simple_config_led_proc_entry();
    return 0;
}
/*
 * used late_initcall so that misc_register will succeed
 * otherwise, misc driver won't be in a initializated state
 * thereby resulting in misc_register api to fail.
 */
late_initcall(ar7240_simple_config_init);
