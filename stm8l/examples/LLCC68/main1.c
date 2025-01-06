
#include <stdint.h>
#include <stm8l.h>
#include <delay.h>
#include "driver_llcc68_lora.h"
#include <uart.h>

int gpio_interrupt_init(void);
void gpio_interrupt_deinit(void);

//////////////////////////////
uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
static uint8_t gs_rx_done;
uint32_t timeout;


static void a_callback(uint16_t type, uint8_t *buf, uint16_t len)
{
	printf("interrupt callback!\r\n");


    switch (type)
    {
        case LLCC68_IRQ_TX_DONE :
        {
            llcc68_interface_debug_print("llcc68: irq tx done.\n");
            

            break;
        }
        case LLCC68_IRQ_RX_DONE :
        {
            uint16_t i;
            llcc68_bool_t enable;
            float rssi;
            float snr;
            
            llcc68_interface_debug_print("llcc68: irq rx done.\n");
            
            if (llcc68_lora_get_status((float *)&rssi, (float *)&snr) != 0)
            {
                return;
            }
            llcc68_interface_debug_print("llcc68: rssi is %0.1f.\n", rssi);
            llcc68_interface_debug_print("llcc68: snr is %0.2f.\n", snr);
            
            if (llcc68_lora_check_packet_error(&enable) != 0)
            {
                return;
            }
            if ((enable == LLCC68_BOOL_FALSE) && len)
            {
                for (i = 0; i < len; i++)
                {
                    llcc68_interface_debug_print("x %d %d",8, buf[i]);
                }
                llcc68_interface_debug_print("\n");
                gs_rx_done = 1;
            }
            
            break;
        }
        case LLCC68_IRQ_PREAMBLE_DETECTED :
        {
            llcc68_interface_debug_print("llcc68: irq preamble detected.\n");
            
            break;
        }
        case LLCC68_IRQ_SYNC_WORD_VALID :
        {
            llcc68_interface_debug_print("llcc68: irq valid sync word detected.\n");
            
            break;
        }
        case LLCC68_IRQ_HEADER_VALID :
        {
            llcc68_interface_debug_print("llcc68: irq valid header.\n");
            
            break;
        }
        case LLCC68_IRQ_HEADER_ERR :
        {
            llcc68_interface_debug_print("llcc68: irq header error.\n");
            
            break;
        }
        case LLCC68_IRQ_CRC_ERR :
        {
            llcc68_interface_debug_print("llcc68: irq crc error.\n");
            
            break;
        }
        case LLCC68_IRQ_CAD_DONE :
        {
            llcc68_interface_debug_print("llcc68: irq cad done.\n");
            
            break;
        }
        case LLCC68_IRQ_CAD_DETECTED :
        {
            llcc68_interface_debug_print("llcc68: irq cad detected.\n");
            
            break;
        }
        case LLCC68_IRQ_TIMEOUT :
        {
            llcc68_interface_debug_print("llcc68: irq timeout.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
}

int lora_send(void)
{
	res = gpio_interrupt_init();
	if (res != 0)
	{
		return 1;
	}
	g_gpio_irq = llcc68_lora_irq_handler;

	llcc68_interface_debug_print("preinit");
	res = llcc68_lora_init(a_callback);
	if (res != 0)
	{
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}
	llcc68_interface_debug_print("postinit");

	res = llcc68_lora_set_send_mode();
	if (res != 0)
	{
		(void)llcc68_lora_deinit();
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}

	llcc68_interface_debug_print("llcc68: send %s.\n", "123");

	res = llcc68_lora_send((uint8_t *)"123", strlen("123"));
	if (res != 0)
	{
		(void)llcc68_lora_deinit();
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}

	res = llcc68_lora_deinit();
	if (res != 0)
	{
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}
	(void)gpio_interrupt_deinit();
	g_gpio_irq = NULL;

	return 0;
}

int lora_receive(void)
{

	res = gpio_interrupt_init();
	if (res != 0)
	{
		return 1;
	}
	g_gpio_irq = llcc68_lora_irq_handler;

	res = llcc68_lora_init(a_callback);
	if (res != 0)
	{
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}

	llcc68_interface_debug_print("llcc68: start receiving...\n");
	gs_rx_done = 0;
	timeout = 3000;

	res = llcc68_lora_set_continuous_receive_mode();
	if (res != 0)
	{
		(void)llcc68_lora_deinit();
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}

	while ((timeout != 0) && (gs_rx_done == 0))
	{
		timeout--;
		llcc68_interface_delay_ms(1000);
	}
	if (gs_rx_done == 0)
	{
		llcc68_interface_debug_print("llcc68: receive timeout.\n");
		(void)llcc68_lora_deinit();
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}

	res = llcc68_lora_deinit();
	if (res != 0)
	{
		(void)gpio_interrupt_deinit();
		g_gpio_irq = NULL;

		return 1;

	}
	(void)gpio_interrupt_deinit();
	g_gpio_irq = NULL;

	return 0;
}
//////////////////////////////



int putchar(int c) {
    uart_write(c);
    return 0;
}

int gpio_interrupt_init(void){
	//pin interrupt
    PB_DDR &= ~(1 << 2);//input
    PB_CR1 &= ~(1 << 2);//floating
    //PB_ODR &= ~(1 << 2);	//
    PB_CR2 |= (1 << 2);	//enable interrupt
	EXTI_CR1 |= (1<<4);//(4,5) port A,B,C,D bit 1 (from 0) rising edge only sensitivity

    enable_interrupts();

	printf("interrupts inited\r\n");

	return 0;
}


void gpio_interrupt_deinit(void){
	PB_CR2 &= ~(1 << 2);
}


void pin_isr4() __interrupt(EXTI2_ISR ) {
	printf("interrupt!\r\n");

	if (g_gpio_irq)
	{
		printf("g_gpio_irq present!\r\n");
		if(g_gpio_irq() == 0)
			printf("callback result OK\r\n");
		else
			printf("callback result NOT OK\r\n");		
	}		

	EXTI_SR1 |= (1<<2);
}


int main(void)
{
    PD_DDR |= (1 << 6);
    PD_CR1 |= (1 << 6);

/*

	while(1)
	{
		for(uint8_t a=0;a<10;a++)
		{
			PD_ODR ^= (1 << 6);
			delay_ms(500);
		}
	}
*/

    uart_init();
    //NSS OUT HIGH
    PB_DDR |= (1 << 4);
    PB_CR1 |= (1 << 4);
    PB_ODR |= (1 << 4);


	while(1)
	{
		for(uint8_t a=0;a<10;a++)
		{
			PD_ODR ^= (1 << 6);
			delay_ms(500);
			printf("abc\r\n");
		}

		//
		//lora_receive();	
		lora_send();
	}	
	
}

