/*****************************************************************************/
/*! file ap94.c
** /brief Boot support for AP94 board
**    
**  This provides the support code required for the AP94 board in the U-Boot
**  environment.  This board is a Hydra based system with two Merlin WLAN
**  interfaces.
**
**  Copyright (c) 2008 Atheros Communications Inc.  All rights reserved.
**
*/

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>
#include "ar7100_soc.h"
#ifdef CFG_NMRP
extern int NmrpState;
#endif

extern flash_info_t flash_info[];

/******************************************************************************/
/*!
**  \brief ar7100_mem_config
**
**  This is a "C" level implementation of the memory configuration interface
**  for the AP94.  
**
**  \return RAM size in bytes
*/

int
ar7100_mem_config(void)
{

    /* XXX - should be set based board configuration */
    *(volatile unsigned int *)0xb8050004 = 0x50C0;
    udelay(10);
    *(volatile unsigned int *)0xb8050018 = 0x1313;
    udelay(10);
    *(volatile unsigned int *)0xb805001c = 0x67;/*set PCI clock to 66MHz*/
    udelay(10);
    *(volatile unsigned int *)0xb8050010 = 0x1099;
    udelay(10);

    return (ar7100_ddr_find_size());
}

/******************************************************************************/
/*!
**  \brief ar7100_usb_initial_config
**
**  -- Enter Detailed Description --
**
**  \param param1 Describe Parameter 1
**  \param param2 Describe Parameter 2
**  \return Describe return value, or N/A for void
*/

long int initdram(int board_type)
{
    printf("b8050000: 0x%x\n",* (unsigned long *)(0xb8050000));
    return (ar7100_mem_config());
}

/******************************************************************************/
/*!
**  \brief ar7100_usb_initial_config
**
**  -- Enter Detailed Description --
**
**  \param param1 Describe Parameter 1
**  \param param2 Describe Parameter 2
**  \return Describe return value, or N/A for void
*/

int checkboard (void)
{

    printf("WNDR3700U (ar7100) U-boot " ATH_AP83_UBOOT_VERSION " dni7 V1.7\n");
    return 0;
}

/*
 * sets up flash_info and returns size of FLASH (bytes)
 */
unsigned long 
flash_get_geom (flash_info_t *flash_info)
{
    int i;
    
    /* XXX this is hardcoded until we figure out how to read flash id */

    flash_info->flash_id  = FLASH_M25P64;
    flash_info->size = 8*1024*1024; /* bytes */
    flash_info->sector_count = flash_info->size/CFG_FLASH_SECTOR_SIZE;

    for (i = 0; i < flash_info->sector_count; i++) {
        flash_info->start[i] = CFG_FLASH_BASE + (i * CFG_FLASH_SECTOR_SIZE);
        flash_info->protect[i] = 0;
    }

    printf ("flash size 8MB, sector count = %d\n", flash_info->sector_count);
    return (flash_info->size);

}
/*ledstat 0:on; 1:off*/
void wndr3700u_power_led(int ledstat)
{
   GpioSet(AR7100_GPIO_OE,TEST_LED,1);
   GpioSet(AR7100_GPIO_OUT,TEST_LED,1);

   GpioSet(AR7100_GPIO_OE,POWER_LED,1);
   GpioSet(AR7100_GPIO_OUT,POWER_LED,ledstat);
}
/*ledstat 0:on; 1:off*/
void wndr3700u_test_led(int ledstat)
{
   GpioSet(AR7100_GPIO_OE,TEST_LED,1);
   GpioSet(AR7100_GPIO_OUT,TEST_LED,ledstat);

   GpioSet(AR7100_GPIO_OE,POWER_LED,1);
   GpioSet(AR7100_GPIO_OUT,POWER_LED,1);
}
/*return value 0:not pressed, 1:pressed*/
int wndr3700u_reset_button_is_press()
{
	if(GpioGet(AR7100_GPIO_IN,RESET_BUTTON_GPIO))
		return 0;
	return 1;
}
void wndr3700u_reset_default_LedSet(void)
{
       static int DiagnosLedCount = 1;
       if ((DiagnosLedCount++ % 2)== 1)
        {
               /*power on test led 0.25s*/
		wndr3700u_test_led(0);
		NetSetTimeout ((CFG_HZ*1)/4,wndr3700u_reset_default_LedSet);
         }
         else{
               /*power off test led 0.75s*/
		wndr3700u_test_led(1);
		NetSetTimeout ((CFG_HZ*3)/4, wndr3700u_reset_default_LedSet);
         }
}
void Update_LedSet(void)
{
#ifdef CFG_NMRP
	if(NmrpState != 0)
		return;
#endif
	static int DiagnosLedCount = 1;
	if ((DiagnosLedCount++ % 2)== 1)
	{
		/*power on test led 0.25s*/
		wndr3700u_power_led(0);
		NetSetTimeout ((CFG_HZ*1)/4,Update_LedSet);
	}
	else{
		/*power off test led 0.75s*/
		wndr3700u_power_led(1);
		NetSetTimeout ((CFG_HZ*3)/4, Update_LedSet);
	}
}
/*erase the config sector of flash*/
void wndr3700u_reset_default()
{
    unsigned int s_first,s_end;
    s_first = flash_in_which_sec (&flash_info[0], CFG_FLASH_CONFIG_BASE - flash_info[0].start[0]);
    s_end = flash_in_which_sec (&flash_info[0],  CFG_FLASH_CONFIG_BASE + CFG_FLASH_CONFIG_PARTITION_SIZE - flash_info[0].start[0]) - 1;
    printf("Restore to factory default\n");
    flash_erase (&flash_info[0], s_first, s_end);
#ifdef CFG_NMRP
    if(NmrpState != 0)
        return;
#endif
    printf ("Rebooting...\n");
    do_reset(NULL,0,0,NULL);

}

