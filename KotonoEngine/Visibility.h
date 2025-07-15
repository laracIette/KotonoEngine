#pragma once
enum class EVisibility : char
{
	None = 0x00,
	Editor  = 0x01,
	Game = 0x02,
	EditorAndGame = Editor | Game
};