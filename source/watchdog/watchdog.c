
#include "watchdog_cfg.h"

#if defined(CONFIG_INTERNAL_WATCHDOG)
extern int watchdog_internal_init(void);
extern int watchdog_internal_start(void);
extern int watchdog_internal_feed(void);
extern int watchdog_internal_stop(void);
#else
#define watchdog_internal_init()
#define watchdog_internal_start()
#define watchdog_internal_feed()
#define watchdog_internal_stop()
#endif

#if defined(CONFIG_EXTERNAL_WATCHDOG)
extern int watchdog_external_init(void);
extern int watchdog_external_start(void);
extern int watchdog_external_feed(void);
extern int watchdog_external_stop(void);
#else
#define watchdog_external_init()
#define watchdog_external_start()
#define watchdog_external_feed()
#define watchdog_external_stop()
#endif

int watchdog_initialize(void)
{
    watchdog_internal_init();

    watchdog_external_init();

    return 0;
}

int watchdog_start(void)
{
    watchdog_internal_start();

    watchdog_external_start();

    return 0;
}

int watchdog_feed(void)
{
    watchdog_internal_feed();

    watchdog_external_feed();

    return 0;
}

int watchdog_stop(void)
{
    watchdog_internal_stop();

    watchdog_external_stop();

    return 0;
}
