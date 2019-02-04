#pragma once
#include "Texture.h"

class FullscreenQuad
{
public:
	static void RenderTexture(const Texture& texture) { RenderTexture(texture, Vec2::zero, Vec2(2.f, 2.f)); }
	static void RenderTexture(const Texture& texture, const Vec2& position, const Vec2& size);
};