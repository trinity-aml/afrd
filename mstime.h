/*
 * Millisecond timers
 * Copyright (c) 2017 Andrew Zabolotny
 *
 * This code can be freely redistributed under the terms of
 * GNU Less General Public License version 3 or later.
 */

#ifndef __MSTIME_H__
#define __MSTIME_H__

/*
 * This simple library provides a way to work with time intervals
 * with millisecond accuracy (well, it all depends on operating system
 * scheduler, of course).
 */

#include <stdint.h>

#ifndef __cplusplus
typedef enum
{
	false = 0,
	true = !0,
} bool;
#endif

/// Keep same behaviour on 32- and 64-bit machines
typedef uint32_t mstime_t;

/// The global current time variable; call mstime_update() to refresh.
extern mstime_t g_mstime;

/**
 * Get the current millisecond time.
 * This value has no real sense, it's only mea is to count time intervals.
 */
extern mstime_t mstime_get ();

/**
 * Update the global g_mstime variable.
 * Must be called on every millisecond before you're going to work with timers.
 */
static inline void mstime_update ()
{
	g_mstime = mstime_get ();
}

/**
 * Arm a one-shot timer. After the timer is armed, the ost_expired() function
 * will return true after given amount of milliseconds.
 *
 * Uses the g_mstime global variable.
 *
 * @arg timer
 *      A pointer to the variable that holds the timer.
 * @arg ms
 *      Number of milliseconds to wait.
 */
static void mstime_arm (mstime_t *timer, uint32_t ms)
{
	uint32_t t = g_mstime + ms;
	if (!t) t = 1;
	*timer = t;
}

/**
 * Check if timer is enabled
 */
static inline bool mstime_enabled (mstime_t *timer)
{
	return (*timer != 0);
}

/**
 * Disable the timer
 */
static inline void mstime_disable (mstime_t *timer)
{
	*timer = 0;
}

/**
 * Return number of milliseconds until mstime_expired() will return true.
 *
 * Uses the g_mstime global variable.
 *
 * @arg timer
 *      A pointer to the variable that holds the timer.
 * @return
 *      Returns 0 if timer is disabled, or number of milliseconds until it expires.
 */
static inline mstime_t mstime_left (mstime_t *timer)
{
	if (!mstime_enabled (timer))
		return 0;

	mstime_t diff = *timer - g_mstime;
	if (((int32_t)diff) >= 0)
		return diff;

	return 0;
}

/**
 * Check if timer has expired.
 * If timer expires, it is disabled.
 *
 * Uses the g_mstime global variable.
 *
 * @arg timer
 *      A pointer to the variable that holds the timer.
 * @return
 *      true if timer expired or is disabled.
 */
static inline bool mstime_expired (mstime_t *timer)
{
	if (mstime_left (timer) > 0)
		return false;

	mstime_disable (timer);
	return true;
}

#endif /* __MSTIME_H__ */
