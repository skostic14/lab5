/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 *
 *
 *
 */


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include "vga_periph_mem.h"
#include "xil_exception.h"




void print(char *str);

int main()
{
    init_platform();
    unsigned char string_s[] = "LPRS 2\n";
    int cursor_pos_y = 0;
    int cursor_pos_x = 0;
    int text_pos = 350;

    int smerx = 0;		// 0 - ne krece se		1 - ide levo			2 - ide desno
    int smery = 1;		// 0 - ne krece se		1 - ide gore			2 - ide dole
    int block_x = 0;

    int i = 0;

    XStatus Status;
	Xuint32 value1, value2, value3;

    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0);// direct mode   0
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3);// display_mode  1
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x1);// show frame      2
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0x1);// font size       3
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0xFFFFFF);// foreground 4
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0x0000FF);// background color 5
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xFF0000);// frame color      6

    print("Hello World\n\r");

    //Set Terminal count for my_timer
    XIo_Out32(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0, 0x5F5E100);
    XIo_Out32(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x4, 0x2);

    value1 = XIo_In32(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0);
    xil_printf("\n\rvalue1 = %x.", value1);




    clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    clear_graphics_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    while(1){
    	draw_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, cursor_pos_y, cursor_pos_x);
    	set_cursor(text_pos);
    	clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    	print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, string_s, 6);
    	for(i=0; i<500000; i++);

    	text_pos += 1;

    	if(smerx == 0){
			if(smery == 2)
				cursor_pos_y -= 3;
			else if(smery == 1)
				cursor_pos_y += 3;
    	}

    	if(smerx == 1)
			cursor_pos_x -= 1;
    	else if(smerx == 2)
			cursor_pos_x += 1;

    	if(cursor_pos_y > 200){
    		smery = 0;
    		if(block_x == 0){
    			block_x = 1;
        		smerx = 1;
    		}
    		else
				smery = 2;
    	}

    	else if(cursor_pos_y < -200){
    		smery = 0;

    		if(block_x == 0){
    			block_x = 1;
    			smerx = 2;
    		}
    		else
    			smery = 1;
    	}

    	/*if(cursor_pos_x > 6){
    		if(smerx)
    			block_x = 0;
			smerx = 0;
			smery = 1;
    	}

    	else if(cursor_pos_x < -6){
    		if(smerx)
    			block_x = 0;
    		smerx = 0;
    		smery = 2;
    	}*/

    	if(smerx == 1){
    		if(cursor_pos_x < -6){
    			block_x = 0;
    			smerx = 0;
    			smery = 2;
    		}
    	}

    	else if(smerx == 2){
    		if(cursor_pos_x > 6){
    			block_x = 0;
    			smerx = 0;
    			smery = 1;
    		}

    	}

    }


    return 0;
}
