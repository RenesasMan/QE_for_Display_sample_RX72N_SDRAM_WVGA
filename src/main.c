/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * System Name  : RX72N group, GLCDC sample program using QE for Display [RX].
 * File Name    : main.c
 * Version      : 1.00
 * Device       : R5F572NNDDBD(RX72N Group)
 * Abstract     :
 * Tool-Chain   : e2 studio Version 7.7.0
 *              : C/C++ Compiler Package for RX Family (V.3.01.00)
 * OS           : not use
 * H/W Platform : Renesas Starter Kit+ for RX72N
 * Description  : Example of setting GLCDC using QE for Display [RX].
 * Limitation   : none
 *******************************************************************************/
/*******************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 20.11.2019 1.00     First Release
 *******************************************************************************/

/*******************************************************************************
 Includes <System Includes> , "Project Includes"
 *******************************************************************************/
#include "r_smc_entry.h"
#include "r_glcdc_rx_if.h"
//#include "r_glcdc_rx_pinset.h"

/* Header files for RSKRX72N board output by QE for Display [RX] */
#include "r_image_config_RX72N_RSK.h"
#include "r_lcd_timing_RX72N_RSK.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* Image info definition */
#define IMAGE_WIDTH          (LCD_CH0_DISP_HW)  /* Width of image used in this sample. */
#define IMAGE_HEIGHT         (LCD_CH0_DISP_VW)  /* Height of image used in this sample. */
#define IMAGE_PIXEL_SIZE     (2u)    /* Graphic format: RGB565, 2 bytes per pixel */

/* Frame buffer address of graphics 2 is the FRAME_BUFFER section top address */
//#define FRAME_BUF_BASE_ADDR  (__sectop("FRAME_BUFFER"))
//#define FRAME_BUF_BASE_ADDR  (__sectop("BEXRAM"))
#define FRAME_BUF_BASE_ADDR  ((void*)(0x08000000))

/* Pixel color code of RGB565 format, 2 bytes per pixel */
#define PIXEL_COLOR_BLUE     (0x001F)
#define PIXEL_COLOR_RED      (0xF800)

#define BORDER_COLOR		PIXEL_COLOR_RED
#define BACKGROUND_COLOR	PIXEL_COLOR_RED

/* SDRAM Definitions */
/* **** SDRAM address **** */
#define SDRAM_TOP   ((void*)(0x08000000))             /* SDRAM top address 0x0800 0000 */
#define SDRAM_END   ((void*)(0x09000000))             /* SDRAM end address 0x0900 0000 */

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* Using GLCDC fit module */
static glcdc_cfg_t gs_glcdc_init_cfg;                   /* Argument of R_GLCDC_Open function. */
static volatile bool gs_first_interrupt_flag;           /* It is used for interrupt control of GLCDC module */

/* ---- Setting gamma parameters to be adjusted with QE for Display [RX] ---- */
/* Gamma Red data table */
static const gamma_correction_t gs_gamma_table_r =
{
    /* Gain (Adjusted value by QE) */
    { IMGC_GAMMA_R_GAIN_00, IMGC_GAMMA_R_GAIN_01, IMGC_GAMMA_R_GAIN_02, IMGC_GAMMA_R_GAIN_03,
      IMGC_GAMMA_R_GAIN_04, IMGC_GAMMA_R_GAIN_05, IMGC_GAMMA_R_GAIN_06, IMGC_GAMMA_R_GAIN_07,
      IMGC_GAMMA_R_GAIN_08, IMGC_GAMMA_R_GAIN_09, IMGC_GAMMA_R_GAIN_10, IMGC_GAMMA_R_GAIN_11,
      IMGC_GAMMA_R_GAIN_12, IMGC_GAMMA_R_GAIN_13, IMGC_GAMMA_R_GAIN_14, IMGC_GAMMA_R_GAIN_15 },

    /* Threshold (Adjusted value by QE) */
    { IMGC_GAMMA_R_TH_01, IMGC_GAMMA_R_TH_02, IMGC_GAMMA_R_TH_03, IMGC_GAMMA_R_TH_04,
      IMGC_GAMMA_R_TH_05, IMGC_GAMMA_R_TH_06, IMGC_GAMMA_R_TH_07, IMGC_GAMMA_R_TH_08,
      IMGC_GAMMA_R_TH_09, IMGC_GAMMA_R_TH_10, IMGC_GAMMA_R_TH_11, IMGC_GAMMA_R_TH_12,
      IMGC_GAMMA_R_TH_13, IMGC_GAMMA_R_TH_14, IMGC_GAMMA_R_TH_15 }
};

/* Gamma Green data table */
static const gamma_correction_t gs_gamma_table_g =
{
    /* Gain (Adjusted value by QE) */
    { IMGC_GAMMA_G_GAIN_00, IMGC_GAMMA_G_GAIN_01, IMGC_GAMMA_G_GAIN_02, IMGC_GAMMA_G_GAIN_03,
      IMGC_GAMMA_G_GAIN_04, IMGC_GAMMA_G_GAIN_05, IMGC_GAMMA_G_GAIN_06, IMGC_GAMMA_G_GAIN_07,
      IMGC_GAMMA_G_GAIN_08, IMGC_GAMMA_G_GAIN_09, IMGC_GAMMA_G_GAIN_10, IMGC_GAMMA_G_GAIN_11,
      IMGC_GAMMA_G_GAIN_12, IMGC_GAMMA_G_GAIN_13, IMGC_GAMMA_G_GAIN_14, IMGC_GAMMA_G_GAIN_15 },

    /* Threshold (Adjusted value by QE) */
    { IMGC_GAMMA_G_TH_01, IMGC_GAMMA_G_TH_02, IMGC_GAMMA_G_TH_03, IMGC_GAMMA_G_TH_04,
      IMGC_GAMMA_G_TH_05, IMGC_GAMMA_G_TH_06, IMGC_GAMMA_G_TH_07, IMGC_GAMMA_G_TH_08,
      IMGC_GAMMA_G_TH_09, IMGC_GAMMA_G_TH_10, IMGC_GAMMA_G_TH_11, IMGC_GAMMA_G_TH_12,
      IMGC_GAMMA_G_TH_13, IMGC_GAMMA_G_TH_14, IMGC_GAMMA_G_TH_15 }
};

/* Gamma Blue data table */
static const gamma_correction_t gs_gamma_table_b =
{
    /* Gain (Adjusted value by QE) */
    { IMGC_GAMMA_B_GAIN_00, IMGC_GAMMA_B_GAIN_01, IMGC_GAMMA_B_GAIN_02, IMGC_GAMMA_B_GAIN_03,
      IMGC_GAMMA_B_GAIN_04, IMGC_GAMMA_B_GAIN_05, IMGC_GAMMA_B_GAIN_06, IMGC_GAMMA_B_GAIN_07,
      IMGC_GAMMA_B_GAIN_08, IMGC_GAMMA_B_GAIN_09, IMGC_GAMMA_B_GAIN_10, IMGC_GAMMA_B_GAIN_11,
      IMGC_GAMMA_B_GAIN_12, IMGC_GAMMA_B_GAIN_13, IMGC_GAMMA_B_GAIN_14, IMGC_GAMMA_B_GAIN_15 },

    /* Threshold (Adjusted value by QE) */
    { IMGC_GAMMA_B_TH_01, IMGC_GAMMA_B_TH_02, IMGC_GAMMA_B_TH_03, IMGC_GAMMA_B_TH_04,
      IMGC_GAMMA_B_TH_05, IMGC_GAMMA_B_TH_06, IMGC_GAMMA_B_TH_07, IMGC_GAMMA_B_TH_08,
      IMGC_GAMMA_B_TH_09, IMGC_GAMMA_B_TH_10, IMGC_GAMMA_B_TH_11, IMGC_GAMMA_B_TH_12,
      IMGC_GAMMA_B_TH_13, IMGC_GAMMA_B_TH_14, IMGC_GAMMA_B_TH_15 }
};


static void glcdc_initialize (void);
static void qe_for_display_parameter_set (glcdc_cfg_t * glcdc_qe_cfg);
static void glcdc_callback (void * pdata);
static void board_port_setting (void);
static void frame_buffer_initialize (void);
void R_GLCDC_PinSet(void);

void main(void);


/*******************************************************************************
 * Outline      : Main processing routine
 * Header       : none
 * Function Name: main
 * Description  : Simple GLCDC sample using GLCDC FIT module and QE for Display[RX].
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void main (void)
{
    volatile static uint16_t *sp_sdram_adr;      /* address pointer(SDRAM) */
    volatile static uint16_t s_sdram_data;       /* sdram write data (SDRAM) */

    R_BSP_SoftwareDelay(100, BSP_DELAY_MICROSECS);

    /* ---- Initialize SDRAMC ---- */
    R_Config_BSC_InitializeSDRAM();

    /* --- test SDRAM --- */
    /* ---- Data initialize for SDRAM  ---- */
	for(sp_sdram_adr = SDRAM_TOP; sp_sdram_adr < SDRAM_END; sp_sdram_adr++)
	{
		*sp_sdram_adr = 0x0000;          /* Initialize data */
	}

	/* ---- Write data for SDRAM  ---- */
	s_sdram_data = 0x0000;            /* SDRAM write data initialize */
	for(sp_sdram_adr = SDRAM_TOP; sp_sdram_adr < SDRAM_END; sp_sdram_adr++)
	{
		*sp_sdram_adr = s_sdram_data++;    /* Write increment data */
	}

    /* ---- Initialization of frame buffer area ---- */
    frame_buffer_initialize();

    /* ---- Initialization of the GLCDC, and start display ---- */
    glcdc_initialize();

    while (1)
    {
        nop();
    }

} /* End of function main() */


/*******************************************************************************
 * Outline      : Initialize the frame buffer area
 * Header       : none
 * Function Name: frame_buffer_initialize
 * Description  : Initialize the frame buffer area. Frame with red color code (RGB 565),
 *              : and fill the inside with the blue color code (RGB 565).
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
static void frame_buffer_initialize (void)
{

    volatile uint16_t * p_pixel_addr;
    volatile uint32_t i;
    volatile uint32_t j;

    /* Get the top address of the frame buffer */
    p_pixel_addr = (uint16_t *)FRAME_BUF_BASE_ADDR;

    /* Top line */
    for (i = 0; i < IMAGE_WIDTH; i++)
    {
        *p_pixel_addr++ = BORDER_COLOR; /* Red color code(RGB565) */
    }

    for (i = 0; i < (IMAGE_HEIGHT - 2); i++)
    {
        for (j = 0; j < IMAGE_WIDTH; j++)
        {
#ifdef __BIG
            /*  Edge pixel (big endian) */
            if ((1 == j) || ((IMAGE_WIDTH - 2) == j))
            {
                *p_pixel_addr++ = BORDER_COLOR; /* Red color code(RGB565) */
            }
#else
            /*  Edge pixel (little endian) */
            if ((0 == j) || ((IMAGE_WIDTH - 1) == j))
            {
                *p_pixel_addr++ = BORDER_COLOR; /* Red color code(RGB565) */
            }
#endif
            else
            {
                *p_pixel_addr++ = BACKGROUND_COLOR; /* Blue color code(RGB565) */
            }
        }
    }

    /* End line */
    for (i = 0; i < IMAGE_WIDTH; i++)
    {
        *p_pixel_addr++ = BORDER_COLOR; /* Red color code(RGB565) */
    }

} /* End of function frame_buffer_initialize() */


/*******************************************************************************
 * Outline      : GLCDC Initial Setting and Operation Start
 * Header       : none
 * Function Name: glcdc_initialize
 * Description  : Prepare GLDCD ports, initialize GLCDC and start GLCDC operation.
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
static void glcdc_initialize (void)
{
    glcdc_err_t ret;

    /* ==== QE for Display [RX] setting parameter ==== */
    qe_for_display_parameter_set( &gs_glcdc_init_cfg);

    /* ==== User setting parameter ==== */
    /* ---- Graphic 2 setting ---- */
    /* Image format */
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].p_base        = (uint32_t *) FRAME_BUF_BASE_ADDR;
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].hsize         = IMAGE_WIDTH;
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].vsize         = IMAGE_HEIGHT;
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].offset        = (IMAGE_WIDTH * IMAGE_PIXEL_SIZE);
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].format        = GLCDC_IN_FORMAT_16BITS_RGB565;
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].frame_edge    = false;
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.x  = 0;
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.y  = 0;
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].bg_color.argb = 0x00CCCCCC;

    /* Alpha blend */
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].visible            = true;
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].blend_control      = GLCDC_BLEND_CONTROL_NONE;

    /* When blend_control is GLCDC_BLEND_CONTROL_NONE, setting values of following blend structure members are ignored. */
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].frame_edge         = false;
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].fixed_blend_value  = 0x00;
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].fade_speed         = 0x00;
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.x = 0;
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.y = 0;
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.x   = IMAGE_WIDTH;
    gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.y   = IMAGE_HEIGHT;

    /* Chromakey */
    gs_glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].enable      = false;

    /* When enable is false, setting values of structure members under chromakey are ignored. */
    gs_glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].before.argb = 0x00FFFFFF;
    gs_glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].after.argb  = 0xFFFFFFFF;

    /* Color look-up table */
    gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].enable = false;

    /* When enable is false, setting values of structure members under clut are ignored. */
    gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = (uint32_t *) FIT_NO_PTR;
    gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].start  = 0;
    gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].size   = 256;

    /* ---- Graphic 1 setting ---- */
    gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_1].p_base = (uint32_t *) NULL; /* Graphic 1 is disable */

    /* When p_base is NULL, setting values of structure members under input[GLCDC_FRAME_LAYER_1] are ignored. */

    /* ---- Hard ware and other setting ---- */
    /* Back ground color */
    gs_glcdc_init_cfg.output.bg_color.argb = 0x00CCCCCC;

    /* Endian */
    gs_glcdc_init_cfg.output.endian = GLCDC_ENDIAN_LITTLE;

    /* Color order */
    gs_glcdc_init_cfg.output.color_order = GLCDC_COLOR_ORDER_RGB;  /* RSKRX72N board dependent setting */

    /* Output clock */
    gs_glcdc_init_cfg.output.clksrc          = GLCDC_CLK_SRC_INTERNAL;
    gs_glcdc_init_cfg.output.clock_div_ratio = GLCDC_PANEL_CLK_DIVISOR_24;

    /* Detection */
    gs_glcdc_init_cfg.detection.vpos_detect  = true;
    gs_glcdc_init_cfg.detection.gr1uf_detect = false;
    gs_glcdc_init_cfg.detection.gr2uf_detect = true;

    /* Interrupt */
    gs_glcdc_init_cfg.interrupt.vpos_enable  = true;
    gs_glcdc_init_cfg.interrupt.gr1uf_enable = false;
    gs_glcdc_init_cfg.interrupt.gr2uf_enable = true;

    /* Call back function register */
    gs_glcdc_init_cfg.p_callback = (void (*) (void *)) glcdc_callback;

    /* -- Initialize a first time interrupt flag -- */
    /* Unintended specified line notification from graphic 2 and graphic 1, 2 underflow is detected only
     for first time after release GLCDC software reset.
     This variable is a flag to skip the first time interrupt processing.
     Refer to Graphic LCD Controller (GLCDC) section of User's Manual: Hardware for details. */
    gs_first_interrupt_flag = false;

    /* ==== GLCDC initialize and start GLCDC operation ==== */

    /* GLCDC initialize */
    ret = R_GLCDC_Open( &gs_glcdc_init_cfg);
    if (GLCDC_SUCCESS != ret)
    {
        while (1)
        {
            nop();
        }
    }

    /* GLCDC port setting */
    R_GLCDC_PinSet();

    /* Board dependent port setting */
    board_port_setting();

    /* Start displaying graphics */
    ret = R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, NULL);
    if (GLCDC_SUCCESS != ret)
    {
        while (1)
        {
            nop();
        }
    }

} /* End of function glcdc_initialize() */


/*******************************************************************************
 * Outline      : GLCDC QE output parameter setting
 * Header       : none
 * Function Name: qe_for_display_parameter_set
 * Description  : Setting parameters to be adjusted with QE for Display [RX].
 *              : The definition written in the header file output by QE for Display [RX]
 *              : is set as the parameter of the GLCDC FIT module.
 * Arguments    : glcdc_cfg_t -
 *                  initial configuration parameter
 * Return Value : none
 *******************************************************************************/
static void qe_for_display_parameter_set (glcdc_cfg_t *glcdc_qe_cfg)
{
    /* Output timing */
    glcdc_qe_cfg->output.htiming.front_porch = LCD_CH0_W_HFP;
    glcdc_qe_cfg->output.htiming.back_porch  = LCD_CH0_W_HBP;
    glcdc_qe_cfg->output.htiming.display_cyc = LCD_CH0_DISP_HW;
    glcdc_qe_cfg->output.htiming.sync_width  = LCD_CH0_W_HSYNC;

    glcdc_qe_cfg->output.vtiming.front_porch = LCD_CH0_W_VFP;
    glcdc_qe_cfg->output.vtiming.back_porch  = LCD_CH0_W_VBP;
    glcdc_qe_cfg->output.vtiming.display_cyc = LCD_CH0_DISP_VW;
    glcdc_qe_cfg->output.vtiming.sync_width  = LCD_CH0_W_VSYNC;

    /* Output format */
    glcdc_qe_cfg->output.format = LCD_CH0_OUT_FORMAT;

    /* Tcon polarity*/
    glcdc_qe_cfg->output.data_enable_polarity = LCD_CH0_TCON_POL_DE;
    glcdc_qe_cfg->output.hsync_polarity       = LCD_CH0_TCON_POL_HSYNC;
    glcdc_qe_cfg->output.vsync_polarity       = LCD_CH0_TCON_POL_VSYNC;

    /* Sync signal edge */
    glcdc_qe_cfg->output.sync_edge = LCD_CH0_OUT_EDGE;

    /* Output tcon pin */
    glcdc_qe_cfg->output.tcon_hsync = LCD_CH0_TCON_PIN_HSYNC;
    glcdc_qe_cfg->output.tcon_vsync = LCD_CH0_TCON_PIN_VSYNC;
    glcdc_qe_cfg->output.tcon_de    = LCD_CH0_TCON_PIN_DE;

    /* Correction circuit sequence */
    gs_glcdc_init_cfg.output.correction_proc_order = IMGC_OUTCTL_CALIB_ROUTE;

    /* Brightness */
    glcdc_qe_cfg->output.brightness.enable = IMGC_BRIGHT_OUTCTL_ACTIVE;
    glcdc_qe_cfg->output.brightness.r      = IMGC_BRIGHT_OUTCTL_OFFSET_R;
    glcdc_qe_cfg->output.brightness.g      = IMGC_BRIGHT_OUTCTL_OFFSET_G;
    glcdc_qe_cfg->output.brightness.b      = IMGC_BRIGHT_OUTCTL_OFFSET_B;

    /* Contrast */
    glcdc_qe_cfg->output.contrast.enable = IMGC_CONTRAST_OUTCTL_ACTIVE;
    glcdc_qe_cfg->output.contrast.r      = IMGC_CONTRAST_OUTCTL_GAIN_R;
    glcdc_qe_cfg->output.contrast.g      = IMGC_CONTRAST_OUTCTL_GAIN_G;
    glcdc_qe_cfg->output.contrast.b      = IMGC_CONTRAST_OUTCTL_GAIN_B;

    /* Gamma */
    glcdc_qe_cfg->output.gamma.enable = IMGC_GAMMA_ACTIVE;
    glcdc_qe_cfg->output.gamma.p_r    = (gamma_correction_t *) &gs_gamma_table_r;
    glcdc_qe_cfg->output.gamma.p_g    = (gamma_correction_t *) &gs_gamma_table_g;
    glcdc_qe_cfg->output.gamma.p_b    = (gamma_correction_t *) &gs_gamma_table_b;

    /* Dethering */
    glcdc_qe_cfg->output.dithering.dithering_on        = IMGC_DITHER_ACTIVE;
    glcdc_qe_cfg->output.dithering.dithering_mode      = IMGC_DITHER_MODE;
    glcdc_qe_cfg->output.dithering.dithering_pattern_a = IMGC_DITHER_2X2_PA;
    glcdc_qe_cfg->output.dithering.dithering_pattern_b = IMGC_DITHER_2X2_PB;
    glcdc_qe_cfg->output.dithering.dithering_pattern_c = IMGC_DITHER_2X2_PC;
    glcdc_qe_cfg->output.dithering.dithering_pattern_d = IMGC_DITHER_2X2_PD;

} /* End of function qe_for_display_parameter_set() */


/*******************************************************************************
 * Outline      : GLCDC callback function
 * Header       : none
 * Function Name: glcdc_callback
 * Description  : Be called from within GLCDC interrupt handling.
 *                Registering callback function with API function.
 * Arguments    : callback_event -
 *                  Generated interrupt event
 *                    GLCDC_EVENT_GR1_UNDERFLOW: Graphic 1 Underflow
 *                    GLCDC_EVENT_GR2_UNDERFLOW: Graphic 2 Underflow
 *                    GLCDC_EVENT_LINE_DETECTION: Graphic 2 Line Detection
 * Return Value : none
 * Note         : Unintended specified line notification from graphic 2 and graphic 1,2 underflow is detected
 *                only for first time after release GLCDC software reset.
 *                Therefore, do nothing with first VPOS interrupt processing after execution of R_GLCDC_Open function,
 *                execute user process from next interrupt.
 *                In this sample program, processing depend on detection is not done.
 *                When using on the user system, please incorporate the above processing.
 *******************************************************************************/
static void glcdc_callback (void * pdata)
{

    if (false == gs_first_interrupt_flag)
    {
        gs_first_interrupt_flag = true;

        /* Do nothing */
    }
    else
    {
        glcdc_callback_args_t * p_decode;
        p_decode = (glcdc_callback_args_t *) pdata;

        if (GLCDC_EVENT_GR1_UNDERFLOW == p_decode->event)
        {
            nop(); /* GR1 underflow , if necessary */
        }
        else if (GLCDC_EVENT_GR2_UNDERFLOW == p_decode->event)
        {
            nop(); /* GR2 underflow , if necessary */
        }
        else /* GLCDC_EVENT_LINE_DETECTION */
        {
            nop(); /* GR2 line detection , if necessary */
        }
    }

} /* End of function glcdc_callback() */


/*******************************************************************************
 * Outline      : Board Dependent Port Setting
 * Header       : none
 * Function Name: board_port_setting
 * Description  : Set the board dependent ports (back light, reset control port etc.)
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
static void board_port_setting (void)
{
    /* ---- Port setting ---- */
    /* LCD back light and display-on */
	PORT2.PODR.BIT.B7 = 1;  /* Back light */
	PORTK.PODR.BIT.B4 = 1;  /* Display */
	PORT2.PDR.BIT.B7 = 1;
	PORTK.PDR.BIT.B4 = 1;

} /* End of function board_port_setting() */


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

/* End of File */
