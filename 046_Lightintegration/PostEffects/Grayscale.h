#pragma once

class Grayscale : public Render2D
{
public:
	Grayscale(ExecuteValues* values);
	~Grayscale();

	void Render();

private:
	class Buffer : public ShaderBuffer
	{
	public:
		Buffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Select = 0;
		}

		struct Struct
		{
			int Select;

			float Padding[3];
		} Data;
	};
	Buffer* buffer;
	
};