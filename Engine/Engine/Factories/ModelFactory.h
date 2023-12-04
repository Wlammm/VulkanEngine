#pragma once

class Model;

class ModelFactory
{
public:
	static Model* LoadModelFromFbx(const std::filesystem::path& inPath);
};