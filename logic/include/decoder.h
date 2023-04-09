#pragma once
#include <GenericCode.h>
#include <engine.h>

class Decoder
{
public:
	Decoder() {};
	~Decoder() {};

	void render(Engine* e);
	void setCode(GenericCode* c);

private:
	GenericCode* myCode = 0;
};

