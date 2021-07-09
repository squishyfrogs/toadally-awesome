#ifndef TEXT_H
#define TEXT_H
#include <tonc.h>

void textsys_init();
// libtonc TTE init but without the manual color override
void tte_init_se_no_color(int bgnr, u16 bgcnt, SCR_ENTRY se0, u32 bupofs, const TFont *font, fnDrawg proc);

#endif //TEXT_H