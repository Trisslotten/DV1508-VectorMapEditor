#pragma once

#include "texture.hpp"
#include "shader.hpp"
#include <sstream>

class VectorMap;

class Tool
{
public:
	virtual ~Tool() {}

	void init()
	{
		icon.loadTexture("assets/" + iconFile());
		vInit();
	}

	virtual void use(GLuint mouseUVSSBO, float radius, float strength) = 0;

	virtual bool hasGUI() = 0;
	virtual void showGUI() = 0;

	virtual bool hasSpecialGUI() = 0;
	virtual void showSpecialGUI() = 0;

	virtual std::string name() = 0;

	virtual GLuint getIconID()
	{
		return icon.getID();
	}

	void setVectorMap(VectorMap* vectorMap)
	{
		this->vectorMap = vectorMap;
	}

protected:
	VectorMap* vectorMap = nullptr;

private:
	virtual void vInit() = 0;
	virtual std::string iconFile() = 0;

	Texture icon;

};


class ToolAddHeight : public Tool
{
public:
	ToolAddHeight() {}
	~ToolAddHeight() {}

	virtual void use(GLuint mouseUVSSBO, float radius, float strength) override;
	virtual bool hasGUI() override;
	virtual void showGUI() override;
	virtual bool hasSpecialGUI() override;
	virtual void showSpecialGUI() override;

	virtual std::string name()
	{
		return "Change Height";
	}
private:
	virtual void vInit() override;
	virtual std::string iconFile() override
	{
		return "icon_tool_up.png";
	}

	ShaderProgram addHeightShader;
};

class ToolSmoothen : public Tool
{
public:
	virtual void use(GLuint mouseUVSSBO, float radius, float strength) override;
	virtual bool hasGUI() override;
	virtual void showGUI() override;
	virtual bool hasSpecialGUI() override;
	virtual void showSpecialGUI() override;

	virtual std::string name()
	{
		return "Smoothen";
	}
private:
	virtual void vInit() override;
	virtual std::string iconFile() override
	{
		return "icon_tool_smoothen.png";
	}
};

class ToolExpand : public Tool
{
public:
	virtual void use(GLuint mouseUVSSBO, float radius, float strength) override;
	virtual bool hasGUI() override;
	virtual void showGUI() override;
	virtual bool hasSpecialGUI() override;
	virtual void showSpecialGUI() override;

	virtual std::string name()
	{
		return "Expand";
	}
private:
	virtual void vInit() override;
	virtual std::string iconFile() override;

	ShaderProgram expandShader;
};

class ToolCurve : public Tool
{
public:
	virtual void use(GLuint mouseUVSSBO, float radius, float strength) override;
	virtual bool hasGUI() override;
	virtual void showGUI() override;
	virtual bool hasSpecialGUI() override;
	virtual void showSpecialGUI() override;
	virtual std::string name() override;
private:
	virtual void vInit() override;
	virtual std::string iconFile() override;

	ShaderProgram curveShader;

	float* currBezier;

	const char* currBezierName;

	struct bezierData {
		float* data;
		std::string name;
	};
	std::vector<bezierData> savedBeziers;
};