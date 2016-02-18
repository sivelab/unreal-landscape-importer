#pragma once

#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"


class FLandscapeImporterEdModeToolkit : public FModeToolkit
{
public:

	FLandscapeImporterEdModeToolkit();

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:

	TSharedPtr<SWidget> ToolkitWidget;
};
