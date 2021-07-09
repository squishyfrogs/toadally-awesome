#ifndef TIMER_H
#define TIMER_H
#include <tonc.h>


typedef struct Timer_T{
	int duration;
	int time;
	u8 flags;
	void (*end_callback)();
}Timer;

#define TIMERFLAG_ENABLED	0x01
#define TIMERFLAG_LOOP		0x02

void timer_init(Timer *timer, int duration, void(*end_callback)(), u8 flags);
void timer_update(Timer *timer);
void timer_clear(Timer *timer);

// enable the timer
inline void timer_enable(Timer *timer)
{
	timer->flags |= TIMERFLAG_ENABLED;
}

// turns off the timer and resets its time
inline void timer_disable(Timer *timer)
{
	timer->flags &= ~TIMERFLAG_ENABLED;
	timer->time = timer->duration;
}

// turns off the timer but does not reset its time
inline void timer_pause(Timer *timer)
{
	timer->flags &= ~TIMERFLAG_ENABLED;
}

inline void timer_reset(Timer *timer)
{
	timer->time = timer->duration;
}

inline bool check_timer_active(Timer *timer)
{
	return (timer->flags & TIMERFLAG_ENABLED); 
}

#endif //TIMER_H