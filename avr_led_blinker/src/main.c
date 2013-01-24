//////////////////////////////////////////////////////////////////////////////
// RGG LED main.c
//////////////////////////////////////////////////////////////////////////////


#include "compiler.h"
#include "preprocessor.h"
#include "pm.h"
#include "gpio.h"
#include "pwm.h"

//#include "print_funcs.h"
#include "intc.h"
#include "power_clocks_lib.h"
#include "conf_usb.h"
#include "usb_task.h"

#if USB_DEVICE_FEATURE == ENABLED
#include "usb_drv.h"
#include "usb_descriptors.h"
#include "usb_standard_request.h"
#include "device_task.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// clock setup
//////////////////////////////////////////////////////////////////////////////

#define FOSC0           12000000                              //!< Osc0 frequency: Hz.
#define OSC0_STARTUP    AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC  //!< Osc0 startup time: RCOsc periods.

void init_clock() {
	// source: gpio_local_bus_example.c

	// Initialize domain clocks (CPU, HSB, PBA and PBB) to the max frequency available
	// without flash wait states.
	// Some of the registers in the GPIO module are mapped onto the CPU local bus.
	// To ensure maximum transfer speed and cycle determinism, any slaves being
	// addressed by the CPU on the local bus must be able to receive and transmit
	// data on the bus at CPU clock speeds. The consequences of this is that the
	// GPIO module has to run at the CPU clock frequency when local bus transfers
	// are being performed => we want fPBA = fCPU.

	// Switch the main clock source to Osc0.
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

	// Setup PLL0 on Osc0, mul=10 ,no divisor, lockcount=16: 12Mhzx11 = 132MHz output
	pm_pll_setup(&AVR32_PM, 0, // pll.
			10, // mul.
			1, // div.
			0, // osc.
			16); // lockcount.
	// PLL output VCO frequency is 132MHz.
	// We divide it by 2 with the pll_div2=1 to get a main clock at 66MHz.
	pm_pll_set_option(&AVR32_PM, 0, // pll.
			1, // pll_freq.
			1, // pll_div2.
			0); // pll_wbwdisable.
	// Enable the PLL.
	pm_pll_enable(&AVR32_PM, 0);
	// Wait until the PLL output is stable.
	pm_wait_for_pll0_locked(&AVR32_PM);
	// Configure each clock domain to use the main clock divided by 2
	// => fCPU = fPBA = fPBB = 33MHz.
	pm_cksel(&AVR32_PM, 1, // pbadiv.
			0, // pbasel.
			1, // pbbdiv.
			0, // pbbsel.
			1, // hsbdiv=cpudiv
			0); // hsbsel=cpusel
	// Switch the main clock source to PLL0.
	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCCTRL_MCSEL_PLL0);
}

//////////////////////////////////////////////////////////////////////////////
// PWM setup
//////////////////////////////////////////////////////////////////////////////

//pwm

#define NUM_CHANNELS	5

#define	PWM_PIN_PA07			AVR32_PWM_0_0_PIN
#define PWM_FUNCTION_PA07		AVR32_PWM_0_0_FUNCTION
#define PWM_CHANNEL_ID_PA07		0

#define	PWM_PIN_PA08			AVR32_PWM_1_0_PIN
#define PWM_FUNCTION_PA08		AVR32_PWM_1_0_FUNCTION
#define PWM_CHANNEL_ID_PA08		1

#define	PWM_PIN_PA11			AVR32_PWM_0_1_PIN
#define PWM_FUNCTION_PA11		AVR32_PWM_0_1_FUNCTION
#define PWM_CHANNEL_ID_PA11		0

#define	PWM_PIN_PA12			AVR32_PWM_1_1_PIN
#define PWM_FUNCTION_PA12		AVR32_PWM_1_1_FUNCTION
#define PWM_CHANNEL_ID_PA12		1

#define	PWM_PIN_PA13			AVR32_PWM_2_1_PIN
#define PWM_FUNCTION_PA13		AVR32_PWM_2_1_FUNCTION
#define PWM_CHANNEL_ID_PA13		2

#define	PWM_PIN_PA14			AVR32_PWM_3_0_PIN
#define PWM_FUNCTION_PA14		AVR32_PWM_3_0_FUNCTION
#define PWM_CHANNEL_ID_PA14		3

#define	PWM_PIN_PA15			AVR32_PWM_4_0_PIN
#define PWM_FUNCTION_PA15		AVR32_PWM_4_0_FUNCTION
#define PWM_CHANNEL_ID_PA15		4


// channel presets
static unsigned int freq_preset[5] = {740,1030,1320,1,1610};	// Hz
static unsigned int dc_preset[5] = {50,50,50,0,50};				// duty cycle in %

// storage:
static pwm_opt_t pwm_opt;
static avr32_pwm_channel_t pwm_channel[7];
static unsigned int channel_id[7];

// WARNING! the PWM example app does:
//avr32_pwm_channel_t pwm_channel = { .ccnt = 0 };  // One channel config.
// we don't, as pwm_channel_init() does not use the .ccnt field...!?!?

void set_pwm(U8 c_id, unsigned int f, U8 dtyc){
	if(c_id < 0 || c_id >= NUM_CHANNELS)
		return;
	unsigned int c = c_id;
	f = 128906/f;

	pwm_channel[c].cdty = f-f*dtyc/100; // Channel duty cycle, should be < CPRD.
	pwm_channel[c].cprd = f; // Channel period.
	pwm_channel[c].cupd = 0; // Channel update is not used here.10

	pwm_channel_init(channel_id[c], &pwm_channel[c]);

	unsigned int channel_mask = 0;
	unsigned int i;
	//for (i = 0; i < NUM_CHANNELS; i++) {
		channel_mask |= (1 << channel_id[c]);
	//}
	pwm_start_channels(channel_mask);
}

void init_pwm() {
	// set PWM GPIOs
//	gpio_enable_module_pin(PWM_PIN_PA07, PWM_FUNCTION_PA07);
//	gpio_enable_module_pin(PWM_PIN_PA08, PWM_FUNCTION_PA08);
	gpio_enable_module_pin(PWM_PIN_PA11, PWM_FUNCTION_PA11);
	gpio_enable_module_pin(PWM_PIN_PA12, PWM_FUNCTION_PA12);
	gpio_enable_module_pin(PWM_PIN_PA13, PWM_FUNCTION_PA13);
	gpio_enable_module_pin(PWM_PIN_PA14, PWM_FUNCTION_PA14);
	gpio_enable_module_pin(PWM_PIN_PA15, PWM_FUNCTION_PA15);


	int i;
	for (i = 0; i < NUM_CHANNELS; i++) {
		pwm_channel[i].CMR.calg = PWM_MODE_LEFT_ALIGNED; // Channel mode.
		pwm_channel[i].CMR.cpol = PWM_POLARITY_LOW; // Channel polarity.
		pwm_channel[i].CMR.cpd = PWM_UPDATE_DUTY; // Not used the first time.
		pwm_channel[i].CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_256; // Channel prescaler.
	}

	// PWM controller configuration.
	pwm_opt.diva = AVR32_PWM_DIVA_CLK_OFF;
	pwm_opt.divb = AVR32_PWM_DIVB_CLK_OFF;
	pwm_opt.prea = AVR32_PWM_PREA_MCK;
	pwm_opt.preb = AVR32_PWM_PREB_MCK;
	// init pwm globals
	pwm_init(&pwm_opt);

	channel_id[0] = PWM_CHANNEL_ID_PA11;
	channel_id[1] = PWM_CHANNEL_ID_PA12;
	channel_id[2] = PWM_CHANNEL_ID_PA13;
	channel_id[3] = PWM_CHANNEL_ID_PA14;
	channel_id[4] = PWM_CHANNEL_ID_PA15;
//	channel_id[5] = PWM_CHANNEL_ID_PA07;
//	channel_id[6] = PWM_CHANNEL_ID_PA08;
}

//////////////////////////////////////////////////////////////////////////////
// USB
//////////////////////////////////////////////////////////////////////////////

static U32 sof_cnt;

static U8 out_data_length;
static U8 out_buf[EP_SIZE_TEMP2];

//!
//! @brief This function initializes the hardware/software resources required for device applicative task.
//!
void device_task_init(void) {
	sof_cnt = 0;
	out_data_length = 0;

	Usb_enable_sof_interrupt();
}

//!
//! @brief Entry point of the device applicative task management
//!
//! This function links the device application to the USB bus.
//!

void device_task(void) {
	// First, check the device enumeration state
	if (!Is_device_enumerated())
		return;

	// If we receive something in the OUT endpoint, just store it in the RAM buffer
	if (Is_usb_out_received(EP_TEMP_OUT)) {
		gpio_local_tgl_gpio_pin(AVR32_PIN_PA12);

		Usb_reset_endpoint_fifo_access(EP_TEMP_OUT);
		out_data_length = Usb_byte_count(EP_TEMP_OUT);
		usb_read_ep_rxpacket(EP_TEMP_OUT, out_buf, out_data_length, NULL);
		Usb_ack_out_received_free(EP_TEMP_OUT);

		unsigned int f = (out_buf[2] << 8) | out_buf[3];
		// update PWM:
		set_pwm(out_buf[1], f, out_buf[4]);
	}
}

//!
//! @brief usb_sof_action
//!
//! This function increments the sof_cnt counter each time
//! the USB Start-of-Frame interrupt subroutine is executed (1 ms).
//! Useful to manage time delays
//!
void usb_sof_action(void) {
	gpio_local_tgl_gpio_pin(AVR32_PIN_PA10);
	sof_cnt++;
}

//////////////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////////////

/*
 unsigned int xx() {
 unsigned int r = rand();
 r = r % 256;
 r = r * r / 256;
 r = r * r / 256;
 return r;
 }
 */

int main() {

	Enable_global_exception();
	INTC_init_interrupts();

	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

	init_clock();

	pcl_configure_usb_clock();

	usb_task_init();

#if USB_DEVICE_FEATURE == ENABLED
	device_task_init();
#endif

	init_pwm();
	gpio_local_init();

	gpio_local_enable_pin_output_driver(AVR32_PIN_PA10);
	gpio_local_enable_pin_output_driver(AVR32_PIN_PA11);
	gpio_local_enable_pin_output_driver(AVR32_PIN_PA12);

	gpio_local_clr_gpio_pin(AVR32_PIN_PA10);
	gpio_local_clr_gpio_pin(AVR32_PIN_PA11);
	gpio_local_clr_gpio_pin(AVR32_PIN_PA12);

	//set default frequencies:
	int i;
	for (i = 0; i < NUM_CHANNELS; i++) {
		set_pwm(i,freq_preset[i],dc_preset[i]);
	}

	while (TRUE) {
		usb_task();

#if USB_DEVICE_FEATURE == ENABLED
		device_task();
#endif
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////
