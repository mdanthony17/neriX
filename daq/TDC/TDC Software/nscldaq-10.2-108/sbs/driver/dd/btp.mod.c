#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x568fba06, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xeec68492, "pci_bus_read_config_byte" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0xf5893abf, "up_read" },
	{ 0x4c4fef19, "kernel_stack" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x3ec8886f, "param_ops_int" },
	{ 0xc996d097, "del_timer" },
	{ 0x6676e3e5, "dma_set_mask" },
	{ 0xa09103b5, "set_page_dirty_lock" },
	{ 0x9f6b4c62, "down_interruptible" },
	{ 0xd4dd3418, "__register_chrdev" },
	{ 0xf873629b, "x86_dma_fallback_dev" },
	{ 0xfb0e29f, "init_timer_key" },
	{ 0x999e8297, "vfree" },
	{ 0x2b021c00, "pci_bus_write_config_word" },
	{ 0x7d11c268, "jiffies" },
	{ 0x57a6ccd0, "down_read" },
	{ 0x71de9b3f, "_copy_to_user" },
	{ 0x49fadb13, "pci_set_master" },
	{ 0x8f64aa4, "_raw_spin_unlock_irqrestore" },
	{ 0x9021c4eb, "current_task" },
	{ 0x940602e5, "down_trylock" },
	{ 0x27e1a049, "printk" },
	{ 0x4141f80, "__tracepoint_module_get" },
	{ 0xe52592a, "panic" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x7ec9bfbc, "strncpy" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x226138e7, "pci_bus_write_config_dword" },
	{ 0x68aca4ad, "down" },
	{ 0xbe2c0274, "add_timer" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x3b4ba06e, "__get_page_tail" },
	{ 0x61651be, "strcat" },
	{ 0x5fcd7fb6, "module_put" },
	{ 0x78764f4e, "pv_irq_ops" },
	{ 0x42c8de35, "ioremap_nocache" },
	{ 0x38c954bc, "pci_bus_read_config_word" },
	{ 0x1714e2cd, "pci_bus_read_config_dword" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x14af4703, "get_user_pages" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0xb1b13d8f, "pci_unregister_driver" },
	{ 0xd52bf1ce, "_raw_spin_lock" },
	{ 0x9327f5ce, "_raw_spin_lock_irqsave" },
	{ 0xe52947e7, "__phys_addr" },
	{ 0xa1d283e4, "pci_bus_write_config_byte" },
	{ 0x37a0cba, "kfree" },
	{ 0xb80dc366, "remap_pfn_range" },
	{ 0xf59f197, "param_array_ops" },
	{ 0xea8a6c75, "param_ops_long" },
	{ 0xf1b1309d, "dma_supported" },
	{ 0xedc03953, "iounmap" },
	{ 0x71e3cecb, "up" },
	{ 0x87d591c5, "__pci_register_driver" },
	{ 0x3c87b587, "put_page" },
	{ 0xbf457cc9, "vmalloc_to_page" },
	{ 0xc1211685, "pci_enable_device" },
	{ 0x77e2f33, "_copy_from_user" },
	{ 0xc3fe87c8, "param_ops_uint" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xba9eb9d3, "dma_ops" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v0000108Ad00000001sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000108Ad00000002sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000108Ad00000003sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000108Ad00000004sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000108Ad00000010sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000108Ad00000011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000108Ad00000040sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000108Ad00000041sv*sd*bc*sc*i*");
