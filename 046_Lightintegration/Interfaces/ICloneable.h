#pragma once

class ICloneable
{
public:
	virtual void Clone(void **clone) = 0;
};