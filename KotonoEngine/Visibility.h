#pragma once
enum class EVisibility
{
	None = 0x00000000,
	EditorOnly = 0x00000001,
	GameOnly = 0x00000002,
	EditorAndGame = EditorOnly | GameOnly
};