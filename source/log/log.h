#ifndef __LOG_H__
#define __LOG_H__

int log_init(void);
int log_printf(const char *fmt, ...);

#define LEVEL_ERROR     3
#define LEVEL_INFO      6
#define LEVEL_DEBUG     7
#define LEVEL_FUNC      8
#define CONFIG_DEBUG_LEVEL  6

#define CONFIG_DEBUG_PRG
#define CONFIG_DEBUG_WDT
#define CONFIG_DEBUG_NOR
#define CONFIG_DEBUG_DRV
#define CONFIG_DEBUG_DEV

#define LOG_ERROR(fmt, args...) ({ if (CONFIG_DEBUG_LEVEL >= LEVEL_ERROR) log_printf("[E|%s|%d]: " fmt "\r\n", __func__, __LINE__, ##args); })
#define LOG_INFO( fmt, args...) ({ if (CONFIG_DEBUG_LEVEL >= LEVEL_INFO ) log_printf("[I|%s|%d]: " fmt "\r\n", __func__, __LINE__, ##args); })
#define LOG_DEBUG(fmt, args...) ({ if (CONFIG_DEBUG_LEVEL >= LEVEL_DEBUG) log_printf("[D|%s|%d]: " fmt "\r\n", __func__, __LINE__, ##args); })
#define LOG_ENTER(fmt, args...) ({ if (CONFIG_DEBUG_LEVEL >= LEVEL_FUNC ) log_printf("[F|ENTER]: %s(" fmt ")\r\n", __func__, ##args); })
#define LOG_LEAVE(fmt, args...) ({ if (CONFIG_DEBUG_LEVEL >= LEVEL_FUNC ) log_printf("[F|LEAVE]: %s(" fmt ")\r\n", __func__, ##args); })

#ifdef CONFIG_DEBUG_PRG
    #define PRG_ERROR(fmt, args...)     LOG_ERROR(fmt, ##args)
    #define PRG_INFO( fmt, args...)     LOG_INFO( fmt, ##args)
    #define PRG_DEBUG(fmt, args...)     LOG_DEBUG(fmt, ##args)
    #define PRG_ENTER(fmt, args...)     LOG_ENTER(fmt, ##args)
    #define PRG_LEAVE(fmt, args...)     LOG_LEAVE(fmt, ##args)
#else
    #define PRG_ERROR(fmt, args...)
    #define PRG_INFO( fmt, args...)
    #define PRG_DEBUG(fmt, args...)
    #define PRG_ENTER(fmt, args...)
    #define PRG_LEAVE(fmt, args...)
#endif

#ifdef CONFIG_DEBUG_WDT
    #define WDT_ERROR(fmt, args...)     LOG_ERROR(fmt, ##args)
    #define WDT_INFO( fmt, args...)     LOG_INFO( fmt, ##args)
    #define WDT_DEBUG(fmt, args...)     LOG_DEBUG(fmt, ##args)
    #define WDT_ENTER(fmt, args...)     LOG_ENTER(fmt, ##args)
    #define WDT_LEAVE(fmt, args...)     LOG_LEAVE(fmt, ##args)
#else
    #define WDT_ERROR(fmt, args...)
    #define WDT_INFO( fmt, args...)
    #define WDT_DEBUG(fmt, args...)
    #define WDT_ENTER(fmt, args...)
    #define WDT_LEAVE(fmt, args...)
#endif

#ifdef CONFIG_DEBUG_NOR
    #define NOR_ERROR(fmt, args...)     LOG_ERROR(fmt, ##args)
    #define NOR_INFO( fmt, args...)     LOG_INFO( fmt, ##args)
    #define NOR_DEBUG(fmt, args...)     LOG_DEBUG(fmt, ##args)
    #define NOR_ENTER(fmt, args...)     LOG_ENTER(fmt, ##args)
    #define NOR_LEAVE(fmt, args...)     LOG_LEAVE(fmt, ##args)
#else
    #define NOR_ERROR(fmt, args...)
    #define NOR_INFO( fmt, args...)
    #define NOR_DEBUG(fmt, args...)
    #define NOR_ENTER(fmt, args...)
    #define NOR_LEAVE(fmt, args...)
#endif

#ifdef CONFIG_DEBUG_DRV
    #define DRV_ERROR(fmt, args...)     LOG_ERROR(fmt, ##args)
    #define DRV_INFO( fmt, args...)     LOG_INFO( fmt, ##args)
    #define DRV_DEBUG(fmt, args...)     LOG_DEBUG(fmt, ##args)
    #define DRV_ENTER(fmt, args...)     LOG_ENTER(fmt, ##args)
    #define DRV_LEAVE(fmt, args...)     LOG_LEAVE(fmt, ##args)
#else
    #define DRV_ERROR(fmt, args...)
    #define DRV_INFO( fmt, args...)
    #define DRV_DEBUG(fmt, args...)
    #define DRV_ENTER(fmt, args...)
    #define DRV_LEAVE(fmt, args...)
#endif

#ifdef CONFIG_DEBUG_DEV
    #define DEV_ERROR(fmt, args...)     LOG_ERROR(fmt, ##args)
    #define DEV_INFO( fmt, args...)     LOG_INFO( fmt, ##args)
    #define DEV_DEBUG(fmt, args...)     LOG_DEBUG(fmt, ##args)
    #define DEV_ENTER(fmt, args...)     LOG_ENTER(fmt, ##args)
    #define DEV_LEAVE(fmt, args...)     LOG_LEAVE(fmt, ##args)
#else
    #define DEV_ERROR(fmt, args...)
    #define DEV_INFO( fmt, args...)
    #define DEV_DEBUG(fmt, args...)
    #define DEV_ENTER(fmt, args...)
    #define DEV_LEAVE(fmt, args...)
#endif

#endif/*__LOG_H__*/
