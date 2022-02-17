#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#define LED0_NODE	 DT_ALIAS(led0)
#define LED0 		 DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN		 DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS		 DT_GPIO_FLAGS(LED0_NODE, gpios)


/* Prototypes */
int led0_init(void);
int led0_deinit(void);
int led0_on(void);
int led0_off(void);


#endif
