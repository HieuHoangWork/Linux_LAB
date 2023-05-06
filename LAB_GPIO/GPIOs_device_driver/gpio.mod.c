#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x5d8544d7, "module_layout" },
	{ 0xfae4f3cd, "cdev_del" },
	{ 0x37a0cba, "kfree" },
	{ 0xfe990052, "gpio_free" },
	{ 0xbab257a3, "gpiod_unexport" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x548fdaf3, "class_destroy" },
	{ 0x97475ca0, "device_destroy" },
	{ 0xdb8f00bc, "kmem_cache_alloc_trace" },
	{ 0x643e59ba, "kmalloc_caches" },
	{ 0xcc73eecd, "device_create" },
	{ 0x1efe2a2b, "__class_create" },
	{ 0xcc103d81, "cdev_add" },
	{ 0x305c93b0, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x7f195356, "gpiod_direction_input" },
	{ 0x14215c06, "gpiod_set_raw_value" },
	{ 0xb091f131, "gpiod_direction_output_raw" },
	{ 0x8ba48dd7, "gpio_to_desc" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x5f754e5a, "memset" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

