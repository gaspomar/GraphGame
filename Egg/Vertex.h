#pragma once

#include "Math/Float3.h"

namespace Egg {
	/*
	PNT: Position, Normal, Texture
	*/
	struct PNT_Vertex {
		Egg::Math::Float3 position;
		Egg::Math::Float3 normal;
		Egg::Math::Float2 tex;
	};

	/*
	PNTTB: Position, Normal, Texture, Tangent, Binormal
	*/
	struct PNTTB_Vertex {
		Egg::Math::Float3 position;
		Egg::Math::Float3 normal;
		Egg::Math::Float2 tex;
		Egg::Math::Float3 tangent;
		Egg::Math::Float3 binormal;
	};

}
