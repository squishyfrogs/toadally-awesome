#include "timer.h"


void timer_init(Timer *timer, int duration, void(*end_callback)(), u8 flags)
{
	//if(!(timer->flags & TIMERFLAG_ENABLED) && timer->end_callback != NULL)
	//	timer->end_callback();
	
	timer->duration = duration;
	timer->time = duration;
	timer->end_callback = end_callback;
	timer->flags = flags;
}

void timer_update(Timer *timer)
{
	if(!(timer->flags & TIMERFLAG_ENABLED))
		return;
	timer->time--;
	if(timer->time <= 0)
	{
		timer->time = 0;
		timer->end_callback();
		if(timer->flags & TIMERFLAG_LOOP)
			timer_reset(timer);
		else
			timer_disable(timer);
	}
}

void timer_clear(Timer *timer)
{
	timer->duration = 0;
	timer->time = 0;
	timer->end_callback = NULL;
	timer->flags = 0;
}