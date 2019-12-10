#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>
#include "ar7240_soc.h"
#ifdef CFG_NMRP
extern int NmrpState;
#endif

extern void ar7240_ddr_initial_config(uint32_t refresh);
extern int ar7240_ddr_find_size(void);

extern flash_info_t flash_info[];

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

int
ar7240GpioGet(int gpio)
{
    ar7240_gpio_config_input(gpio);
    ar7240_gpio_in_val(gpio);
}

int
ar7240GpioSet(int gpio, int val)
{
    ar7240_gpio_config_output(gpio);
    ar7240_gpio_out_val(gpio, val);
}

void
ar7240_usb_initial_config(void)
{
    ar7240_reg_wr_nf(AR7240_USB_PLL_CONFIG, 0x0a04081e);
    ar7240_reg_wr_nf(AR7240_USB_PLL_CONFIG, 0x0804081e);
}

void ar7240_gpio_config()
{
    /* Disable clock obs */
    ar7240_reg_wr (AR7240_GPIO_FUNC, (ar7240_reg_rd(AR7240_GPIO_FUNC) & 0xffe7e0ff));
    /* Enable eth Switch LEDs */
    ar7240_reg_wr (AR7240_GPIO_FUNC, (ar7240_reg_rd(AR7240_GPIO_FUNC) | 0xf8));
}

int
ar7240_mem_config(void)
{
    unsigned int tap_val1, tap_val2;

#ifndef RAM_VERSION	
    ar7240_ddr_initial_config(CFG_DDR_REFRESH_VAL);

/* Default tap values for starting the tap_init*/
    ar7240_reg_wr (AR7240_DDR_TAP_CONTROL0, 0x8);
    ar7240_reg_wr (AR7240_DDR_TAP_CONTROL1, 0x9);

    /* ar7240 gpio would be configured in start.S, so don't need to be set again here.*/
    //ar7240_gpio_config();
    ar7240_ddr_tap_init();
#endif

    tap_val1 = ar7240_reg_rd(0xb800001c);
    tap_val2 = ar7240_reg_rd(0xb8000020);
    printf("#### TAP VALUE 1 = %x, 2 = %x\n",tap_val1, tap_val2);

    ar7240_usb_initial_config();

    return (ar7240_ddr_find_size());
}

long int initdram(int board_type)
{
    return (ar7240_mem_config());
}

// Removed by Wayne on 2009/11/10
#if 0
int checkboard (void)
{
    printf("U-boot WNR612 V0.2, built on %s\n", __DATE__);
    return 0;
}
#endif

uint32_t GetMagicNumberOfWnr612(void)
{
    return ((uint32_t)(simple_strtoul(getenv("magic_number"),NULL,16))?(simple_strtoul(getenv("magic_number"),NULL,16)):(IH_MAGIC_DEFAULT));
}

/*ledstat 0:on; 1:off*/
void wnr612_power_led(int ledstat)
{
//    ar7240GpioSet(TEST_LED,1);
//    ar7240GpioSet(TEST_LED,1);

    ar7240GpioSet(POWER_LED,1);
    ar7240GpioSet(POWER_LED,ledstat);
}

/*ledstat 0:on; 1:off*/
void wnr612_test_led(int ledstat)
{
    ar7240GpioSet(TEST_LED,1);
    ar7240GpioSet(TEST_LED,ledstat);

//    ar7240GpioSet(POWER_LED,1);
//    ar7240GpioSet(POWER_LED,1);
}

void wnr612_jtag( unsigned int uDisable)
{
	ar9285DisableJTAG(uDisable);
}

/*ledstat 0:on; 1:off*/
void wnr612_wlan_led(int ledstat)
{
static int iCfgInit = 0;

	if( !iCfgInit )
	{
		ar9285GpioCfgOutput(WLAN_LED);
		iCfgInit = 1;
	}
	ar9285GpioSet(WLAN_LED, ledstat );
}


void wnr612_reset_default_LedSet(void)
{
    static int DiagnosLedCount = 1;
    if ((DiagnosLedCount++ % 2)== 1)
    {
	/*power on test led 0.25s*/
	wnr612_test_led(0);
	NetSetTimeout ((CFG_HZ*1)/4, wnr612_reset_default_LedSet);
    }else{
	/*power off test led 0.75s*/
        wnr612_test_led(1);
        NetSetTimeout ((CFG_HZ*3)/4, wnr612_reset_default_LedSet);
    }
}

/*return value 0: not pressed, 1: pressed*/
int wnr612_reset_button_is_press()
{
    if(ar9285GpioGet(RESET_BUTTON_GPIO))
	return 0;
    return 1;
}

extern int flash_sect_erase (ulong, ulong);

/*erase the config sector of flash*/
void wnr612_reset_default()
{
    printf("Restore to factory default\n");
    flash_sect_erase (CFG_FLASH_CONFIG_BASE, CFG_FLASH_CONFIG_BASE+CFG_FLASH_CONFIG_PARTITION_SIZE-1);
#ifdef CFG_NMRP
    if(NmrpState != 0)
        return;
#endif
    printf("Rebooting...\n");
    do_reset(NULL,0,0,NULL);
}

