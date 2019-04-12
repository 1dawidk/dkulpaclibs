#ifndef DKULPAPROT_DEFS
#define DKULPAPROT_DEFS


#ifdef CPU_BCM2835

#define gpio_fsel(x, y) bcm2835_gpio_fsel(x, y)
#define gpio_write(x, y) bcm2835_gpio_write(x, y)
#define gpio_read(x) bcm_gpio_lev(x)
#define gpio_set_pud(x, y bcm2835_gpio_set_pud(x, y)



#endif


#endif //DKULPAPROT_DEFS