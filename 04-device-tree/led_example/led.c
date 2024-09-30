#include <linux/module.h>     /* defines functions such as module_init/module_exit */
#include <linux/platform_device.h>  /* For platform device */
#include <linux/gpio/consumer.h> /* For GPIO Descriptor */
#include <linux/of.h>  /* For device tree*/

#define DRIVER_AUTHOR   "phonglt15_linuxfromscratch@gmail.com"
#define DRIVER_DESC     "LED blinking"

static struct gpio_desc *LED;

static const struct of_device_id gpio_led_dt_ids[] = {
    { .compatible = "bbd-led,dts", },
    { /* sentinel */ }
};

static int led_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    LED = gpiod_get_index(dev, "led", 0, GPIOD_OUT_HIGH);

    pr_info("Hello! Driver probe successfully!\n");
    return 0;
}

static int led_remove(struct platform_device *pdev)
{
    gpiod_put(LED);

    pr_info("Good bye my fen !!\n");
    return 0;
}

static struct platform_driver my_led_drv = {
    .probe  = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "bbd_led_dt_sample",
        .of_match_table = of_match_ptr(gpio_led_dt_ids),
        .owner = THIS_MODULE,
    },
};

module_platform_driver(my_led_drv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
