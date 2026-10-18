/**
 * @file main.c
 * @author gary
 * @date 2026-10-17
 * @brief Main function
 */
#include <stdint.h>
#include <xc.h>

#include "../inc/aht30.h"
#include "../inc/board.h"
#include "../inc/i2c1.h"

// Boot straight into HFINTOSC at 1 MHz (HFFRQ = 4 MHz, CDIV = 4:1) instead of the
// EXTOSC manufacturing default, and keep clock switching enabled (DS40002043F, CONFIG1).
#pragma config RSTOSC = HFINTOSC_1MHZ
#pragma config CSWEN = ON

static void osc_init_1mhz(void)
{
    // OSCFRQ.HFFRQ = 0000 -> HFINTOSC 1 MHz (DS40002043F, 5.6.5)
    OSCFRQ = 0x00;
    // OSCCON1: NOSC = 110 (HFINTOSC), NDIV = 0000 (1:1) -> Fosc = 1 MHz
    OSCCON1 = 0x60;
    // Wait for the clock switch to complete (OSCCON3.ORDY)
    while (!OSCCON3bits.ORDY) {
    }
}

// 28-pin device: PORTA, PORTB, PORTC only (RE3 is input-only MCLR/VPP).
// Every pin is driven low except RB6/RB7 (ICSPCLK/ICSPDAT, left as inputs so the PICkit 4
// can program/debug without contention) and RC3/RC4 (MSSP1 SCL1/SDA1, must be inputs).
static void gpio_init(void)
{
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    ANSELA = 0x00;
    ANSELB &= 0xC0;       // leave RB6/RB7 analog setting untouched
    ANSELC = 0x00;
    TRISA = 0x00;         // all outputs low
    TRISB = 0xC0;         // RB7:RB6 inputs (ICSP), RB5:RB0 outputs low
    TRISC = 0x18;         // RC4:RC3 inputs (I2C), others output low; RC2 is the Timer0 toggle pin
}

// Timer0 tick: 8-bit mode, FOSC/4 = 250 kHz, prescaler 1:4 (62.5 kHz), TMR0H = 249
// -> 250 counts = 4 ms per match, postscaler 1:5 -> TMR0IF every 20 ms.
// 25 interrupts x 20 ms = 500 ms between RC2 toggles.
#define TMR0_TICKS_PER_TOGGLE 25

#define TMR0_TICKS_PER_SAMPLE 50    // 1 s: AHT30 should not be sampled faster

// The first tick after aht30_start() can come 0-20 ms later, so waiting 6 ticks gives
// 100-120 ms: the datasheet's >= 80 ms plus margin for oscillator tolerance. Starts
// here follow a tick, so it is ~120 ms in practice; the CPU is free meanwhile.
#define AHT30_WAIT_TICKS 6

static volatile unsigned char tmr0_ticks;
static volatile unsigned char sample_ticks;
static volatile unsigned char sample_due;
static volatile unsigned char tick_count;   // free-running, wraps every 256 ticks (5.12 s)

// Latest AHT30 reading (x100) and last status, for the debugger watch window.
static volatile int16_t g_temp_x100;
static volatile uint16_t g_rh_x100;
static volatile aht30_status_t g_aht30_status;

static void tmr0_init_20ms(void)
{
    T0CON0 = 0x04;        // T0EN = 0 (off while configuring), 8-bit, T0OUTPS = 0100 (1:5)
    T0CON1 = 0x42;        // T0CS = 010 (FOSC/4), T0ASYNC = 0, T0CKPS = 0010 (1:4)
    TMR0H = 249;          // match value (8-bit period register)
    TMR0L = 0;
    PIR0bits.TMR0IF = 0;
    PIE0bits.TMR0IE = 1;
    T0CON0bits.T0EN = 1;
}

static void __interrupt() isr(void)
{
    if (PIE0bits.TMR0IE && PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        tick_count++;
        if (++sample_ticks >= TMR0_TICKS_PER_SAMPLE) {
            sample_ticks = 0;
            sample_due = 1;
        }
        if (++tmr0_ticks >= TMR0_TICKS_PER_TOGGLE) {
            tmr0_ticks = 0;
            LATCbits.LATC2 ^= 1;
        }
    }
}

int main(){
    osc_init_1mhz();

    gpio_init();

    i2c1_init();
    aht30_init();

    tmr0_init_20ms();
    INTCONbits.PEIE = 1;      // IPEN = 0 (default): GIE + PEIE enable peripheral interrupts
    INTCONbits.GIE = 1;

    unsigned char measuring = 0;
    unsigned char start_tick = 0;

    while(1) {
        if (!measuring) {
            if (sample_due) {
                sample_due = 0;
                g_aht30_status = aht30_start();
                if (g_aht30_status == AHT30_OK) {
                    start_tick = tick_count;
                    measuring = 1;
                }
            }
        } else if ((unsigned char)(tick_count - start_tick) >= AHT30_WAIT_TICKS) {
            int16_t t;
            uint16_t rh;

            measuring = 0;
            g_aht30_status = aht30_fetch(&t, &rh);
            if (g_aht30_status == AHT30_OK) {
                g_temp_x100 = t;
                g_rh_x100 = rh;
            }
        }
    }
}
