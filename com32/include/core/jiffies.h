/*
 * Core fundamental timer interface
 */
#ifndef _CORE_JIFFIES_H
#define _CORE_JIFFIES_H

#define HZ		18
#define MS_PER_JIFFY	55

typedef uint32_t jiffies_t;
typedef int32_t  sjiffies_t;

extern const volatile jiffies_t __jiffies;
static inline jiffies_t jiffies(void)
{
    return __jiffies;
}

#endif
