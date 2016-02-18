#include "LandscapeImporterPrivatePCH.h"
#include "LandscapeImporterEdMode.h"


#include "LandscapeImporterEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"


const FEditorModeID FLandscapeImporterEdMode::EM_LandscapeImporterEdModeId = TEXT("EM_LandscapeImporterEdMode");


FLandscapeImporterEdMode::FLandscapeImporterEdMode()
{

}

FLandscapeImporterEdMode::~FLandscapeImporterEdMode()
{

}

void FLandscapeImporterEdMode::Enter()
{
	FEdMode::Enter();


	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FLandscapeImporterEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}

}

void FLandscapeImporterEdMode::Exit()
{

	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}


	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FLandscapeImporterEdMode::UsesToolkits() const
{
	return true;
}




