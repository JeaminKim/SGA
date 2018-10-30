#pragma once

class GaussianBlur : public Render2D
{
public:
	GaussianBlur(ExecuteValues* values, string psName);
	~GaussianBlur();

	void Render();

private:
	class Buffer : public ShaderBuffer
	{
	public:
		Buffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Count = 1;
		}

		struct Struct
		{
			int Width;
			int Height;
			int Count;

			float Padding;
		} Data;
	};
	Buffer* buffer;
};