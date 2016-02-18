#pragma once

#include "UnrealEd.h" 
#include "Editor.h"

class FLandscapeImporterEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_LandscapeImporterEdModeId;
public:
	FLandscapeImporterEdMode();
	virtual ~FLandscapeImporterEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface
};
