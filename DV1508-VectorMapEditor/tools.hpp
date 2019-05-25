#pragma once

#include "texture.hpp"
#include "shader.hpp"

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
private:
	virtual void vInit() override;
	virtual std::string iconFile() override
	{
		return "icon_tool_up.png";
	}

	ShaderProgram addHeightShader;
};