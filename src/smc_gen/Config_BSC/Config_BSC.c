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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_BSC.c
* Component Version: 1.11.0
* Device(s)        : R5F572NNDxBD
* Description      : This file implements device driver for Config_BSC.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_BSC.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_BSC_Create
* Description  : This function initializes the BUSES module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_BSC_Create(void)
{
    volatile uint16_t dummy_syscr0;

    /* Disable CS0 operation */
    BSC.CS0CR.BIT.EXENB = _0000_BSC_EXENB_DISABLED;

    /* Set BSC control registers */
    BSC.CSRECEN.WORD = _0000_BSC_RCVEN0_DISABLED | _0002_BSC_RCVEN1_ENABLED | _0004_BSC_RCVEN2_ENABLED | 
                       _0008_BSC_RCVEN3_ENABLED | _0010_BSC_RCVEN4_ENABLED | _0020_BSC_RCVEN5_ENABLED | 
                       _0000_BSC_RCVEN6_DISABLED | _0000_BSC_RCVEN7_DISABLED | _0000_BSC_RCVENM0_DISABLED | 
                       _0200_BSC_RCVENM1_ENABLED | _0400_BSC_RCVENM2_ENABLED | _0800_BSC_RCVENM3_ENABLED | 
                       _1000_BSC_RCVENM4_ENABLED | _2000_BSC_RCVENM5_ENABLED | _0000_BSC_RCVENM6_DISABLED | 
                       _0000_BSC_RCVENM7_DISABLED;
    BSC.BEREN.BYTE = _00_BSC_IGAEN_DISABLE | _00_BSC_TOEN_DISABLE;
    BSC.BUSPRI.WORD = _0000_BSC_BPRA_FIX | _0000_BSC_BPRO_FIX | _0000_BSC_BPIB_FIX | _0000_BSC_BPGB_FIX | 
                      _0000_BSC_BPHB_FIX | _0000_BSC_BPFB_FIX | _0000_BSC_BPEB_FIX;

    /* Set address output enable registers */
    MPC.PFAOE0.BYTE = _01_BSC_A8E_ENABLED | _02_BSC_A9E_ENABLED | _04_BSC_A10E_ENABLED | _08_BSC_A11E_ENABLED | 
                      _10_BSC_A12E_ENABLED | _20_BSC_A13E_ENABLED | _40_BSC_A14E_ENABLED | _80_BSC_A15E_ENABLED;

    /* Set MPC registers for external bus interface */
    MPC.PFBCR0.BYTE |= (_01_BSC_ADRLE_ENABLED | _10_BSC_DHE_ENABLED | _00_BSC_WR1BC1E_DISABLED | 
                       _00_BSC_WR32BC32E_DISABLED);
    MPC.PFBCR1.BYTE |= (_10_BSC_MDSDE_ENABLED | _40_BSC_DQM1E_ENABLED);
    MPC.PFBCR2.BYTE |= _00_BSC_D0S_PD0;
    MPC.PFBCR2.BYTE |= _00_BSC_D1S_PD1;
    MPC.PFBCR2.BYTE |= _00_BSC_D2S_PD2;
    MPC.PFBCR2.BYTE |= _00_BSC_D3S_PD3;
    MPC.PFBCR3.BYTE |= _00_BSC_DLHS_PD4_PD7;

    /* Set SDCS# pin */
    PORT6.PMR.BYTE &= 0xFDU;

    /* Set A1 pin */
    PORTA.PMR.BYTE &= 0xFDU;

    /* Set A2 pin */
    PORTA.PMR.BYTE &= 0xFBU;

    /* Set A3 pin */
    PORTA.PMR.BYTE &= 0xF7U;

    /* Set A4 pin */
    PORTA.PMR.BYTE &= 0xEFU;

    /* Set A5 pin */
    PORTA.PMR.BYTE &= 0xDFU;

    /* Set A6 pin */
    PORTA.PMR.BYTE &= 0xBFU;

    /* Set A7 pin */
    PORTA.PMR.BYTE &= 0x7FU;

    /* Set A8 pin */
    PORTB.PMR.BYTE &= 0xFEU;

    /* Set A9 pin */
    PORTB.PMR.BYTE &= 0xFDU;

    /* Set A10 pin */
    PORTB.PMR.BYTE &= 0xFBU;

    /* Set A11 pin */
    PORTB.PMR.BYTE &= 0xF7U;

    /* Set A12 pin */
    PORTB.PMR.BYTE &= 0xEFU;

    /* Set A13 pin */
    PORTB.PMR.BYTE &= 0xDFU;

    /* Set A14 pin */
    PORTB.PMR.BYTE &= 0xBFU;

    /* Set A15 pin */
    PORTB.PMR.BYTE &= 0x7FU;

    /* Set D0 pin */
    PORTD.PMR.BYTE &= 0xFEU;

    /* Set D1 pin */
    PORTD.PMR.BYTE &= 0xFDU;

    /* Set D2 pin */
    PORTD.PMR.BYTE &= 0xFBU;

    /* Set D3 pin */
    PORTD.PMR.BYTE &= 0xF7U;

    /* Set D4 pin */
    PORTD.PMR.BYTE &= 0xEFU;

    /* Set D5 pin */
    PORTD.PMR.BYTE &= 0xDFU;

    /* Set D6 pin */
    PORTD.PMR.BYTE &= 0xBFU;

    /* Set D7 pin */
    PORTD.PMR.BYTE &= 0x7FU;

    /* Set D8 pin */
    PORTE.PMR.BYTE &= 0xFEU;

    /* Set D9 pin */
    PORTE.PMR.BYTE &= 0xFDU;

    /* Set D10 pin */
    PORTE.PMR.BYTE &= 0xFBU;

    /* Set D11 pin */
    PORTE.PMR.BYTE &= 0xF7U;

    /* Set D12 pin */
    PORTE.PMR.BYTE &= 0xEFU;

    /* Set D13 pin */
    PORTE.PMR.BYTE &= 0xDFU;

    /* Set D14 pin */
    PORTE.PMR.BYTE &= 0xBFU;

    /* Set D15 pin */
    PORTE.PMR.BYTE &= 0x7FU;

    /* Set DQM0 pin */
    PORT6.PMR.BYTE &= 0xBFU;

    /* Set DQM1 pin */
    PORT6.PMR.BYTE &= 0x7FU;

    /* Set CKE pin */
    PORT6.PMR.BYTE &= 0xDFU;

    /* Set RAS# pin */
    PORT6.PMR.BYTE &= 0xFBU;

    /* Set CAS# pin */
    PORT6.PMR.BYTE &= 0xF7U;

    /* Set WE# pin */
    PORT6.PMR.BYTE &= 0xEFU;

    /* Set external bus/on-chip ROM */
    SYSTEM.SCKCR.BIT.PSTOP1 = 1U;
    dummy_syscr0 = _5A01_BSC_ROME_ENABLED | _5A02_BSC_EXBE_ENABLED;
    SYSTEM.SYSCR0.WORD = dummy_syscr0;
    dummy_syscr0 &= 0x00FFU;

    while (dummy_syscr0 != SYSTEM.SYSCR0.WORD)
    {
        nop();
    }

    SYSTEM.SCKCR.BIT.PSTOP1 = 0U;

    R_Config_BSC_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_BSC_InitializeSDRAM
* Description  : This function initial SDRAM setting
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_BSC_InitializeSDRAM(void)
{
    /* Set SDCS control registers */
    BSC.SDCCR.BIT.EXENB = 0U;

    /* Wait for EXENB bit is clear to 0 */
    while (0U != BSC.SDCCR.BIT.EXENB)
    {
        nop();
    }

    BSC.SDRFEN.BIT.RFEN = 0U;
    BSC.SDSELF.BIT.SFEN = 0U;

    /* Wait for status bits in SDSR are 0 */
    while (0x00U != BSC.SDSR.BYTE)
    {
        nop();
    }
    BSC.SDIR.WORD = _0002_BSC_ARFI_5 | _0020_BSC_ARFC_2 | _0000_BSC_PRC_3;
    BSC.SDICR.BIT.INIRQ = _01_BSC_INIT_SEQUENCE_START; 

    /* Wait for status bits in SDSR are 0 */
    while (0x00U != BSC.SDSR.BYTE)
    {
        nop();
    }
    BSC.SDCCR.BYTE = _00_BSC_BSIZE_16;
    BSC.SDMOD.WORD = _0230_BSC_MODE_REG_VALUE;
    BSC.SDTR.LONG = _00000003_BSC_CL_3 | _00000100_BSC_WR_2 | _00000200_BSC_RP_2 | _00001000_BSC_RCD_2 | 
                    _00030000_BSC_RAS_4;
    BSC.SDADR.BYTE = _01_BSC_MXC_BIT_SHIFT_9;
    BSC.SDAMOD.BYTE = _00_BSC_SD_CONTINUOUS_DISABLED;
    BSC.SDCMOD.BYTE = _00_BSC_SD_EMODE_SAME;
    BSC.SDRFCR.WORD = _04E0_BSC_RFC_VALUE | _4000_BSC_REFW_5; 
    BSC.SDRFEN.BIT.RFEN = 1U;
    BSC.SDCCR.BIT.EXENB = 1U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
