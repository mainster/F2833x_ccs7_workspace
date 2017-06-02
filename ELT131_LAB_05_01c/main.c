/*
 *  ======== main.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>


/*
 *  ======== main ========
 */
Void main()
{ 
    /*
     * use ROV->SysMin to view the characters in the circular buffer
     */
    System_printf("enter main()\n");
	
    BIOS_start();        /* enable interrupts and start SYS/BIOS */
}
