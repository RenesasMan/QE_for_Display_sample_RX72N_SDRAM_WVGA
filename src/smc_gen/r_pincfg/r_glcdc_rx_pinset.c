/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_glcdc_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F572NNDxBD
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_glcdc_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: R_GLCDC_PinSet
* Description  : This function initializes pins for r_glcdc_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_GLCDC_PinSet()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set LCD_CLK pin */
    MPC.P14PFS.BYTE = 0x25U;
    PORT1.PMR.BIT.B4 = 1U;

    /* Set LCD_TCON0 pin */
    MPC.P13PFS.BYTE = 0x25U;
    PORT1.PMR.BIT.B3 = 1U;

    /* Set LCD_TCON2 pin */
    MPC.PJ2PFS.BYTE = 0x25U;
    PORTJ.PMR.BIT.B2 = 1U;

    /* Set LCD_TCON3 pin */
    MPC.PJ1PFS.BYTE = 0x25U;
    PORTJ.PMR.BIT.B1 = 1U;

    /* Set LCD_DATA0 pin */
    MPC.PJ0PFS.BYTE = 0x25U;
    PORTJ.PMR.BIT.B0 = 1U;

    /* Set LCD_DATA1 pin */
    MPC.P85PFS.BYTE = 0x25U;
    PORT8.PMR.BIT.B5 = 1U;

    /* Set LCD_DATA2 pin */
    MPC.P84PFS.BYTE = 0x25U;
    PORT8.PMR.BIT.B4 = 1U;

    /* Set LCD_DATA3 pin */
    MPC.P57PFS.BYTE = 0x25U;
    PORT5.PMR.BIT.B7 = 1U;

    /* Set LCD_DATA4 pin */
    MPC.P56PFS.BYTE = 0x25U;
    PORT5.PMR.BIT.B6 = 1U;

    /* Set LCD_DATA5 pin */
    MPC.P55PFS.BYTE = 0x25U;
    PORT5.PMR.BIT.B5 = 1U;

    /* Set LCD_DATA6 pin */
    MPC.P54PFS.BYTE = 0x25U;
    PORT5.PMR.BIT.B4 = 1U;

    /* Set LCD_DATA7 pin */
    MPC.P11PFS.BYTE = 0x25U;
    PORT1.PMR.BIT.B1 = 1U;

    /* Set LCD_DATA8 pin */
    MPC.P83PFS.BYTE = 0x25U;
    PORT8.PMR.BIT.B3 = 1U;

    /* Set LCD_DATA9 pin */
    MPC.PC7PFS.BYTE = 0x25U;
    PORTC.PMR.BIT.B7 = 1U;

    /* Set LCD_DATA10 pin */
    MPC.PC6PFS.BYTE = 0x25U;
    PORTC.PMR.BIT.B6 = 1U;

    /* Set LCD_DATA11 pin */
    MPC.PC5PFS.BYTE = 0x25U;
    PORTC.PMR.BIT.B5 = 1U;

    /* Set LCD_DATA12 pin */
    MPC.P82PFS.BYTE = 0x25U;
    PORT8.PMR.BIT.B2 = 1U;

    /* Set LCD_DATA13 pin */
    MPC.P81PFS.BYTE = 0x25U;
    PORT8.PMR.BIT.B1 = 1U;

    /* Set LCD_DATA14 pin */
    MPC.P80PFS.BYTE = 0x25U;
    PORT8.PMR.BIT.B0 = 1U;

    /* Set LCD_DATA15 pin */
    MPC.PC4PFS.BYTE = 0x25U;
    PORTC.PMR.BIT.B4 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

