#pragma once

struct Block
{
	enum class EType
	{
		None,
		Dirt,
		Grass,
		Stone,
		Bedrock
	};

	EType Type = EType::None;
};
