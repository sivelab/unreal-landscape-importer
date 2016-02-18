// Some copyright should be here...

#pragma once

#include "SlateBasics.h"
#include "LandscapeImporterStyle.h"

class FLandscapeImporterCommands : public TCommands<FLandscapeImporterCommands>
{
public:

	FLandscapeImporterCommands()
		: TCommands<FLandscapeImporterCommands>(TEXT("LandscapeImporter"), NSLOCTEXT("Contexts", "LandscapeImporter", "LandscapeImporter Plugin"), NAME_None, FLandscapeImporterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
