#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

int watchdog_initialize(void);

int watchdog_start(void);

int watchdog_feed(void);

int watchdog_stop(void);

#endif/*__WATCHDOG_H__*/
