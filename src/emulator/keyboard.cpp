#include "keyboard.hpp"


uint16_t pollKeyboard()
{
	PollInputEvents();
	uint16_t keys = 0;

	if (IsKeyDown(KEY_X))
		keys |= 1;
	if (IsKeyDown(KEY_ONE))
		keys |= (1 << 1);
	if (IsKeyDown(KEY_TWO))
		keys |= (1 << 2);
	if (IsKeyDown(KEY_THREE))
		keys |= (1 << 3);
	if (IsKeyDown(KEY_Q))
		keys |= (1 << 4);
	if (IsKeyDown(KEY_W))
		keys |= (1 << 5);
	if (IsKeyDown(KEY_E))
		keys |= (1 << 6);
	if (IsKeyDown(KEY_A))
		keys |= (1 << 7);
	if (IsKeyDown(KEY_S))
		keys |= (1 << 8);
	if (IsKeyDown(KEY_D))
		keys |= (1 << 9);
	if (IsKeyDown(KEY_Z))
		keys |= (1 << 0xA);
	if (IsKeyDown(KEY_C))
		keys |= (1 << 0xB);
	if (IsKeyDown(KEY_FOUR))
		keys |= (1 << 0xC);
	if (IsKeyDown(KEY_R))
		keys |= (1 << 0xD);
	if (IsKeyDown(KEY_F))
		keys |= (1 << 0xE);
	if (IsKeyDown(KEY_V))
		keys |= (1 << 0xF);

	return keys;
}
