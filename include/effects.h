#ifndef EFFECTS_H
#define EFFECTS_H


typedef enum EffectType_T {
	ET_DUST,
	ET_SMOKE,
	ET_COUNT							// NOT an actual effect, used to get the number of different effect types
} EffectType;



void create_effect_at_tile(EffectType eff_type, int tile_id);
void create_effect_at_position(EffectType eff_type, int tile_x, int tile_y);

#endif //EFFECTS_H