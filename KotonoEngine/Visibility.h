#pragma once
enum class EVisibility
{
	None = 0x00000000,
	Editor  = 0x00000001,
	Game = 0x00000002,
	EditorAndGame = Editor | Game
};