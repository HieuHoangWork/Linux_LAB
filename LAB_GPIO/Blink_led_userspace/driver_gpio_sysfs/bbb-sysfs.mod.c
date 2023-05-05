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
	{ 0x5b4047c7, "sysfs_remove_group" },
	{ 0xfe990052, "gpio_free" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x64426a76, "kobject_put" },
	{ 0x9672d109, "sysfs_create_group" },
	{ 0x6a926b21, "kobject_create_and_add" },
	{ 0xb091f131, "gpiod_direction_output_raw" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x7f195356, "gpiod_direction_input" },
	{ 0x84b183ae, "strncmp" },
	{ 0x9d669763, "memcpy" },
	{ 0x97255bdf, "strlen" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xc5850110, "printk" },
	{ 0x14215c06, "gpiod_set_raw_value" },
	{ 0x8ba48dd7, "gpio_to_desc" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CB5508C7CF750E45C71E3A1");
