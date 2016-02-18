#include "LandscapeImporterPrivatePCH.h"
#include "LandscapeImporterEdMode.h"
#include "LandscapeImporterEdModeToolkit.h"
#include "LandscapeImporter.h"

#define TEXTURES_DIRECTORY "C:\\Users\\sivelab\\Desktop\\Landscapes\\003910\\out"
#define DEM_FILEPATH "C:\\Users\\sivelab\\Desktop\\Landscapes\\003910\\in\\DTEEC_003910_1685_005400_1685_U01.IMG"

#define LOCTEXT_NAMESPACE "FLandscapeImporterEdModeToolkit"

FLandscapeImporterEdModeToolkit::FLandscapeImporterEdModeToolkit()
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			//return GEditor->GetSelectedActors()->Num() != 0;
			return true;
		}

		static FReply OnImportTexturesClick()
		{
			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("ImportingLandscapeTexturesActionName", "Importing Landscape Textures"));


			FLandscapeImporterModule::ImportLandscapeTextures(TEXTURES_DIRECTORY);

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static FReply OnCreateMaterialsClick()
		{
			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("CreatingMaterialInstances", "Creating Material Instances"));

			FText DialogText;
			DialogText = FText::FromString(TEXT("This does not do anything"));
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			//FLandscapeImporterModule::CreateMaterialInstance("\\Game\\Landscape");

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static FReply OnImportDEMClick()
		{
			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("ImportingLandscapeActionName", "Importing Landscape"));


			FLandscapeImporterModule::ImportLandscapeFromDEM(DEM_FILEPATH);

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Enter thew filepath to the DEM then click Import"))
			]
			/*+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
				]*/
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SEditableTextBox)
					.HintText(LOCTEXT("DEMFileInputLabel", "Enter DEM Filepath(This does not work yet.)"))
					.IsEnabled(false)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					SNew(SButton)
					.Text(LOCTEXT("ImportTexturesButtonLabel", "Import Textures"))
					.OnClicked_Static(&Locals::OnImportTexturesClick)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					SNew(SButton)
					.Text(LOCTEXT("CreateMaterialButtonLabel", "Create Material Instances"))
					.OnClicked_Static(&Locals::OnCreateMaterialsClick)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					SNew(SButton)
					.Text(LOCTEXT("ImportButtonLabel", "Import DEM"))
					.OnClicked_Static(&Locals::OnImportDEMClick)
				]

		];
}

FName FLandscapeImporterEdModeToolkit::GetToolkitFName() const
{
	return FName("Landscape Importer");
}

FText FLandscapeImporterEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("LandscapeImporterEdModeToolkit", "DisplayName", "Landscape Importer Tool");
}

class FEdMode* FLandscapeImporterEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FLandscapeImporterEdMode::EM_LandscapeImporterEdModeId);
}

#undef LOCTEXT_NAMESPACE
