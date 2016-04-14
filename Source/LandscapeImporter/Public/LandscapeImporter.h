// Some copyright should be here...

#pragma once

#include "ModuleManager.h"
#include <string>
#include "Runtime/Landscape/Classes/Landscape.h"


class FToolBarBuilder;
class FMenuBuilder;

class FLandscapeImporterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	static void ImportLandscapeTextures(std::string texturedir);
	static void ImportLandscapeFromDEM(std::string demfilename, int textureSize);
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	//static UMaterialInterface* CreateMaterialInstance(int i, int j);
	//static void CreateMaterialInstance(int i, int j, ALandscape* land);
	void AddMenuExtension(FMenuBuilder& Builder);
	static void BuildLanscape(float *pafScanline, int x_size, int y_size, int o_x, int o_y, int s_x, int s_y, int add, int mult, float min, float max, bool set_min_to_zero, int x, int y, float textureSize);
	TArray<uint16> ImportLandscape_Data;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};