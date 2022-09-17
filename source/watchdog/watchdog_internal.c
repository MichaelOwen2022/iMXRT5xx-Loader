
#include "watchdog_cfg.h"

#if defined(CONFIG_INTERNAL_WATCHDOG)

#include "log.h"

int watchdog_internal_init(void)
{
    int retval = 0;
    WDT_ENTER("");

    WDT_LEAVE("%d", retval);
    return retval;
}

int watchdog_internal_start(void)
{
    int retval = 0;
    WDT_ENTER("");

    WDT_LEAVE("%d", retval);
    return retval;
}

int watchdog_internal_feed(void)
{
    int retval = 0;
    WDT_ENTER("");

    WDT_LEAVE("%d", retval);
    return retval;
}

int watchdog_internal_stop(void)
{
    int retval = 0;
    WDT_ENTER("");

    WDT_LEAVE("%d", retval);
    return retval;
}

#endif
