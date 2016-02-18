// Some copyright should be here...

#include "LandscapeImporterPrivatePCH.h"
#include "LandscapeImporterCommands.h"

#define LOCTEXT_NAMESPACE "FLandscapeImporterModule"

void FLandscapeImporterCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "LandscapeImporter", "Execute LandscapeImporter action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
