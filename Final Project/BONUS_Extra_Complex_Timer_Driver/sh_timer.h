/*
 * sh_timer.h
 *
 *  Created on: Apr 1, 2021
 *      Author: Shady Thomas
 */

#ifndef SH_TIMER_H_
#define SH_TIMER_H_

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

/******************************************************/
/**************   Timer 0  Typedefs ******************/
/******************************************************/

typedef enum
{
	NORMAL, CTC = 2, FAST_PWM
}T0_WGM;

typedef enum
{
	OC0_DISCONNECTED, TOGGLE_OC0_RESERVED, CLEAR_OC0, SET_OC0
}T0_COM;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}T0_PRESCALER;

typedef enum
{
	OCIE_DISABLE, OCIE_ENABLE
}T0_OCIE;

typedef enum
{
	TOIE_DISABLE, TOIE_ENABLE
}T0_TOIE;

typedef struct
{
	T0_WGM 			t0_wgm;
	T0_COM			t0_com;
	T0_PRESCALER	t0_prescaler;
	uint8 			t0_tcnt0;
	uint8 			t0_ocr0;
	T0_OCIE			t0_ocie;
	T0_TOIE			t0_toie;

}TIMER0_CONFIG;

/******************************************************/
/**************   Timer 1  Typedefs ******************/
/******************************************************/
typedef enum
{
	OC1A_DISCONNECTED, TOGGLE_OC1A_RESERVED, CLEAR_OC1A, SET_OC1A
}T1_COM_A;

typedef enum
{
	OC1B_DISCONNECTED, TOGGLE_OC1B_RESERVED, CLEAR_OC1B, SET_OC1B
}T1_COM_B;

typedef enum
{
	T1_NORMAL, T1_CTC_8_BIT = 4, T1_CTC_16_BIT = 12, T1_FAST_PWM_ICR1 = 14, T1_FAST_PWM_OCR1
}T1_WGM;

typedef enum
{
	T1_NO_CLOCK,T1_F_CPU_CLOCK,T1_F_CPU_8,T1_F_CPU_64,T1_F_CPU_256,T1_F_CPU_1024
}T1_PRESCALER;

typedef enum
{
	ICU_NOISE_CANCEL_DIS, ICU_NOISE_CANCEL_EN
}T1_INPUT_CAPTURE_NOISE_CANCELER;

typedef enum
{
	ICU_NEGATIVE_EDGE, ICU_POSITIVE_EDGE
}T1_ICU_EDGE_SELECT;

typedef enum
{
	TICU1_DIS, TICU1_EN
}T1_ICU_INT_EN;

typedef enum
{
	OCIE1A_DIS, OCIE1A_EN
}T1_OCIE1A;

typedef enum
{
	OCIE1B_DIS, OCIE1B_EN
}T1_OCIE1B;

typedef enum
{
	TOIE1_DIS, TOIE1_EN
}T1_TOIE1;

typedef struct
{
	T1_COM_A		t1_com_a;
	T1_COM_B		t1_com_b;
	T1_WGM			t1_wgm;
	T1_PRESCALER	t1_prescaler;
	T1_INPUT_CAPTURE_NOISE_CANCELER		t1_icu_nc;
	T1_ICU_EDGE_SELECT					t1_icu_edge;
	uint16 			t1_tcnt1;
	uint16 			t1_ocr1a;
	uint16			t1_ocr1b;
	uint16			t1_icr1;
	T1_ICU_INT_EN	t1_interrupt_icu;
	T1_OCIE1A		t1_interrupt_oc1a;
	T1_OCIE1B		t1_interrupt_oc1b;
	T1_TOIE1		t1_interrupt_ovf;
}TIMER1_CONFIG;

/******************************************************/
/**************   Timer 2  Typedefs ******************/
/******************************************************/
typedef enum
{
	OC2_DISCONNECTED, TOGGLE_OC2_RESERVED, CLEAR_OC2, SET_OC2
}T2_COM;

typedef enum
{
	T2_NORMAL, T2_CTC = 2, T2_FAST_PWM
}T2_WGM;

typedef enum
{
	T2_NO_CLOCK,T2_F_CPU_CLOCK,T2_F_CPU_8,T2_F_CPU_64,T2_F_CPU_256,T2_F_CPU_1024
}T2_PRESCALER;

typedef enum
{
	OCIE2_DISABLE, OCIE2_ENABLE
}T2_OCIE;

typedef enum
{
	TOIE2_DISABLE, TOIE2_ENABLE
}T2_TOIE;

typedef struct
{
	T2_WGM			t2_wgm;
	T2_COM			t2_com;
	T2_PRESCALER	t2_prescaler;
	uint8			t2_tcnt2;
	uint8			t2_ocr2;
	T2_OCIE			t2_ocie;
	T2_TOIE			t2_toie;

}TIMER2_CONFIG;

/******************************************************/
/**************  Final Structure	 ******************/
/******************************************************/

typedef enum
{
	TIMER0, TIMER1, TIMER2
}TIMER_TYPE;

typedef struct
{
	TIMER_TYPE 		timer_type;
	TIMER0_CONFIG	* t0_ptr;
	TIMER1_CONFIG	* t1_ptr;
	TIMER2_CONFIG 	* t2_ptr;
}TIMER_CONFIG;

/**************************************
			Function Prototypes
**************************************/

void TIMER_init(const TIMER_CONFIG * config_ptr);

void TIMER0_OVF_setCallBack(void(*a_ptr)(void));
void TIMER0_CTC_setCallBack(void(*a_ptr)(void));
void TIMER2_OVF_setCallBack(void(*a_ptr)(void));
void TIMER2_CTC_setCallBack(void(*a_ptr)(void));
void TIMER1_OVF_setCallBack(void(*a_ptr)(void));
void TIMER1_CTC_A_setCallBack(void(*a_ptr)(void));
void TIMER1_CTC_B_setCallBack(void(*a_ptr)(void));
void TIMER1_ICU_setCallBack(void(*a_ptr)(void));

void TIMER_deinit(const TIMER_CONFIG * config_ptr);

#endif /* SH_TIMER_H_ */
