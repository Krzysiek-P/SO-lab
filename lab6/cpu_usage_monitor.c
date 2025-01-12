#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel_stat.h>
#include <linux/sched/cputime.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/cpufreq.h>

static int interval = 10;
module_param(interval, int, 0444);

static u64 prev_idle = 0, prev_total = 0;

static void log_cpu_load(struct timer_list *t);

static DEFINE_TIMER(cpu_timer, log_cpu_load);

static u64 calculate_cpu_load(void) {
    unsigned int cpu = smp_processor_id();
    u64 idle_time, wall_time;
    int io_busy = 0;

    idle_time = get_cpu_idle_time(cpu, &wall_time, io_busy);
    u64 total_time = jiffies_to_usecs(jiffies);

    if (prev_total == 0) {
        prev_idle = idle_time;
        prev_total = total_time;
        return 0;
    }

    u64 delta_idle = idle_time > prev_idle ? idle_time - prev_idle : 0;
    u64 delta_total = total_time > prev_total ? total_time - prev_total : 1;

    prev_idle = idle_time;
    prev_total = total_time;

    if (delta_total == 0) {
        return 0;
    }

    u64 cpu_usage = 100 - (delta_idle * 100 / delta_total);

    return cpu_usage;
}

static void log_cpu_load(struct timer_list *t) {
    u64 cpu_usage = calculate_cpu_load();
    pr_info("CPU Load: %llu%%\n", cpu_usage);
    mod_timer(&cpu_timer, jiffies + interval * HZ);
}

static int __init cpu_monitor_init(void) {
    pr_info("CPU Monitor: Initializing with interval = %d seconds\n", interval);
    mod_timer(&cpu_timer, jiffies + interval * HZ);
    return 0;
}

static void __exit cpu_monitor_exit(void) {
    pr_info("CPU Monitor: Exiting\n");
    del_timer(&cpu_timer);
}

module_init(cpu_monitor_init);
module_exit(cpu_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CPU Monitor Module");
