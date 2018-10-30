#pragma once

class Blur : public Render2D
{
public:
	Blur(ExecuteValues* values);
	~Blur();

	void Render();

private:
	class Buffer : public ShaderBuffer
	{
	public:
		Buffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Select = 0;
			Data.Count = 0;
		}

		struct Struct
		{
			int Select;
			int Width;
			int Height;
			int Count;
		} Data;
	};
	Buffer* buffer;
};