#include "Base.h"

Base::Base()
	:pos_(0.0f, 0.0f), vel_(0.0f, 0.0f), Color_(0xffffffff), objType(UNKNOWN)
{
	//SetObjType(UNKNOWN);
}

Base::Base(const Vector2D& pos, const Vector2D& vel,
	  unsigned int color)
	:pos_(pos), vel_(vel), Color_(color), objType(UNKNOWN)
{
	//SetObjType(UNKNOWN);
}

Base::~Base()
{
}

void Base::Update()
{
}

void Base::Draw()
{
}
