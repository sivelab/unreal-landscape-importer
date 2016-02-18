// Some copyright should be here...

#include "LandscapeImporterPrivatePCH.h"
#include "LandscapeImporterEdMode.h"

// Landscape Data
#include "Runtime/Landscape/Classes/Landscape.h"
// To get the world
#include "Engine.h"
// To read Image data
#include "Developer/ImageWrapper/Public/Interfaces/IImageWrapper.h"
#include "Developer/ImageWrapper/Public/Interfaces/IImageWrapperModule.h"

#include "AssetToolsModule.h"
#include "Materials/MaterialInstanceConstant.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "LandscapeImporterStyle.h"
#include "LandscapeImporterCommands.h"

#include "LevelEditor.h"

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "tinydir.h"


static const FName LandscapeImporterTabName("LandscapeImporter");

#define LOCTEXT_NAMESPACE "FLandscapeImporterModule"

void FLandscapeImporterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLandscapeImporterStyle::Initialize();
	FLandscapeImporterStyle::ReloadTextures();

	FLandscapeImporterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLandscapeImporterCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FLandscapeImporterModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FLandscapeImporterModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FLandscapeImporterModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FEditorModeRegistry::Get().RegisterMode<FLandscapeImporterEdMode>(FLandscapeImporterEdMode::EM_LandscapeImporterEdModeId, LOCTEXT("LandscapeImporterEdModeName", "LandscapeImporterEdMode"), FSlateIcon(), true);
	
	GDALAllRegister();
}

void FLandscapeImporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	
	
	FLandscapeImporterStyle::Shutdown();

	FLandscapeImporterCommands::Unregister();

	FEditorModeRegistry::Get().UnregisterMode(FLandscapeImporterEdMode::EM_LandscapeImporterEdModeId);
}

void FLandscapeImporterModule::ImportLandscapeTextures(std::string texturedir)
{
	//TODO: Set Mip Gen Settings to NoMipmaps.
	tinydir_dir dir;
	tinydir_open_sorted(&dir, texturedir.c_str());
	TArray<FString> files;

	for (int i = 0; i < dir.n_files; i++){
		tinydir_file file;
		tinydir_readfile_n(&dir, &file, i);

		std::string filename = file.name;

		if (!file.is_dir && filename.substr(0, 11) == "T_Landscape") {
			std::string absPath = texturedir + "\\" + filename;
			files.Add(absPath.c_str());
		}
	}

	FAssetToolsModule & AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ImportAssets(files, "/Game/Landscape/Textures", NULL, false);


	for (int i = 0; i < dir.n_files; i++){
		tinydir_file file;
		tinydir_readfile_n(&dir, &file, i);

		std::string filename = file.name;

		if (!file.is_dir && filename.substr(0, 11) == "T_Landscape") {
			
			std::string texture_path = "Texture2D'/Game/Landscape/Textures/" + filename.substr(0, filename.length() - 4) + "." + filename.substr(0, filename.length() - 4) + "'";
			static UTexture2D *texture;
			texture = NULL;
			texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString(texture_path.c_str())));
			if (texture == NULL){
				FText DialogText = FText::FromString("Error: Texture was not found.");
				FMessageDialog::Open(EAppMsgType::Ok, DialogText);
				return;
			}
			else{
				texture->MipGenSettings = TMGS_NoMipmaps;
				texture->PostEditChange();
				texture->MarkPackageDirty();
			}
		}
	}
}

//static void CreateMaterialInstance(int i, int j, ALandscape* land)
//{	
//	std::string texture_path = "/Game/Landscape/Textures/T_Landscape" + std::to_string(i) + "_" + std::to_string(j);
//	static UMaterial *material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Landscape.Landscape'")));
//	UMaterialInstanceDynamic *dynam = UMaterialInstanceDynamic::Create(material, NULL);
//
//	if (material != NULL){
//		land->LandscapeMaterial = material;
//	}
//}

void FLandscapeImporterModule::ImportLandscapeFromDEM(std::string demfilename)
{
	int add;
	int mult;
	float max;
	float min;
	bool set_min_to_zero;
	FText DialogText;

	std::string m_filename = demfilename;
	GDALDataset* m_poDataset = (GDALDataset *)GDALOpen(m_filename.c_str(), GA_ReadOnly);

	if (m_poDataset == NULL) {
		DialogText = FText::FromString(TEXT("Couldn't open file"));
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}

	// Put your "OnButtonClicked" stuff here
	DialogText = FText::FromString(TEXT("This is a test. 12"));
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Size: {0}x{1}x{2}"),
		FText::FromString(FString::FromInt(m_poDataset->GetRasterXSize())),
		FText::FromString(FString::FromInt(m_poDataset->GetRasterYSize())),
		FText::FromString(FString::FromInt(m_poDataset->GetRasterCount()))
		);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	GDALRasterBand *poBand;
	int nBlockXSize, nBlockYSize;
	poBand = m_poDataset->GetRasterBand(1);
	poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);

	float *pafScanline;

	int m_nXSize = poBand->GetXSize();
	int m_nYSize = poBand->GetYSize();

	pafScanline = (float *)CPLMalloc(sizeof(float)*m_nXSize*m_nYSize);

	poBand->RasterIO(GF_Read, 0, 0,
		m_nXSize, m_nYSize,
		pafScanline,
		m_nXSize, m_nYSize, GDT_Float32,
		0, 0);


	max = pafScanline[0];
	min = pafScanline[0];
	float min_2 = pafScanline[0];

	for (int i = 0; i < m_nXSize*m_nYSize; i++) {
		if (pafScanline[i]>max) {
			max = pafScanline[i];
		}
		if (pafScanline[i]<min) {
			min = pafScanline[i];
		}
		if (pafScanline[i] > min) {
			if (min_2 == min) {
				min_2 = pafScanline[i];
			}
			else if (pafScanline[i]<min_2) {
				min_2 = pafScanline[i];
			}
		}
	}

	/*int add = 0;
	int mult = 0;
	bool set_min_to_zero;*/

	if (min < 0) {
		set_min_to_zero = true;
		add = min_2 * -1;
		mult = 65535 / (max - min_2);
	}
	else {
		set_min_to_zero = false;
		add = min * -1;
		mult = 65535 / (max - min);
	}



	int block_size = 2048;
	int needed_overlap = block_size / 512;

	//BuildLanscape(pafScanline, m_nXSize, m_nYSize, 0, 0, block_size, block_size, add, mult, min, max, set_min_to_zero);

	//for (int i = 0; i+block_size < m_nXSize; i += block_size) {
	//	for (int j = 0; j + block_size < m_nYSize; j += block_size) {
	/*int c = 0;*/
	for (int i = 0; i*block_size < m_nXSize; i++) {
		for (int j = 0; j*block_size < m_nYSize; j++) {
	//for (int i = 1; i < 2; i++) {
	//	for (int j = 0; j < 2; j++) {
			int startx = (i*block_size) - (i * needed_overlap);
			int starty = (j*block_size) - (j * needed_overlap);
			BuildLanscape(pafScanline, m_nXSize, m_nYSize, startx, starty, block_size, block_size, add, mult, min, max, set_min_to_zero, i, j);

			/*DialogText = FText::Format(
				LOCTEXT("PluginButtonDialogText", "Making Block {0}: Starting at ({1}, {2}) Size: {3}"),
				FText::FromString(FString::FromInt(c)),
				FText::FromString(FString::FromInt(i)),
				FText::FromString(FString::FromInt(j)),
				FText::FromString(FString::FromInt(block_size))
				);
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			c++;*/
		}
	}

	// BuildLanscape(pafScanline, m_nXSize, 0, 0, 1024, 1024);

	/*float max = pafScanline[0];
	float min = pafScanline[0];
	float min_2 = pafScanline[0];

	for (int i = 0; i < m_nXSize*m_nYSize; i++) {
	if (pafScanline[i]>max) {
	max = pafScanline[i];
	}
	if (pafScanline[i]<min) {
	min = pafScanline[i];
	}
	if (pafScanline[i] > min) {
	if (min_2 == min) {
	min_2 = pafScanline[i];
	}
	else if (pafScanline[i]<min_2) {
	min_2 = pafScanline[i];
	}
	}
	}

	int add = 0;
	int mult = 0;

	bool set_min_to_zero;
	if (min < 0) {
	set_min_to_zero = true;
	add = min_2 * -1;
	mult = 65535/(max - min_2);
	}
	else {
	set_min_to_zero = false;
	add = min * -1;
	mult = 65535 / (max - min);

	}



	DialogText = FText::Format(
	LOCTEXT("PluginButtonDialogText", "Max: {0} Min:{1} Min_2:{2}"),
	FText::FromString(FString::FromInt(max)),
	FText::FromString(FString::FromInt(min)),
	FText::FromString(FString::FromInt(min_2))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);


	DialogText = FText::Format(
	LOCTEXT("PluginButtonDialogText", "Add: {0} Mult:{1} SizeZ:{2}"),
	FText::FromString(FString::FromInt(add)),
	FText::FromString(FString::FromInt(mult)),
	FText::FromString(FString::FromInt((max+add)*mult))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);



	const int32 SizeX = m_nXSize;
	const int32 SizeY = m_nYSize;
	const int32 SizeZ = (max + add)*mult*(100.0f / mult);
	const int32 SectionsPerComponent = 1;
	const int32 QuadsPerSection = 63;
	const int32 QuadsPerComponent = SectionsPerComponent*QuadsPerSection;
	FVector Location = FVector(SizeX*-0.5f*100.0f, SizeY*-0.5f*100.0f, SizeZ*0.5f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector Scale = FVector(100.0f, 100.0f, (100.0f / mult) * 100);

	// GEditor->GetEditorWorldContext().World()
	ALandscape* Landscape = GEditor->GetEditorWorldContext().World()->SpawnActor<ALandscape>(Location, Rotation);
	Landscape->SetActorRelativeScale3D(Scale);

	//const TArray<uint16>& Data = GetImportLandscapeData();
	TArray<uint16> Data;
	Data.AddUninitialized(SizeX * SizeY);
	uint16* WordData = Data.GetData();
	//uint16* ImportWordData = ImportLandscape_Data.GetData();
	// Initialize blank heightmap data
	for (int32 i = 0; i < SizeX * SizeY; i++)
	{
	uint16 height = pafScanline[i];
	if (pafScanline[i] == min && set_min_to_zero) {
	WordData[i] = 1;
	}
	else {
	WordData[i] = (height + add)*mult;
	}

	// WordData[i] = ImportWordData[i];
	// WordData[i] = 32768; //max: 65535
	}

	TArray<FLandscapeImportLayerInfo> LayerInfos;


	DialogText = FText::Format(
	LOCTEXT("PluginButtonDialogText", "i:{0} j:{1} p:{2}"),
	FText::FromString(FString::FromInt(i)),
	FText::FromString(FString::FromInt(j)),
	FText::FromString(FString::FromInt(p))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	Landscape->Import(FGuid::NewGuid(), SizeX, SizeY, QuadsPerComponent, SectionsPerComponent, QuadsPerSection, Data.GetData(), NULL, LayerInfos);
	//Landscape->Import(FGuid::NewGuid(), ImportLandscape_Width, ImportLandscape_Height, QuadsPerComponent, SectionsPerComponent, QuadsPerSection, , NULL, LayerInfos);*/
}

void FLandscapeImporterModule::PluginButtonClicked()
{
	std::string m_filename = "E:\\Unreal\\Terrain Converter\\in\\DTEEC_003910_1685_005400_1685_U01.IMG";
	std::string t_filedir = "E:\\Unreal\\Terrain Converter\\out";
	ImportLandscapeTextures(t_filedir);
	ImportLandscapeFromDEM(m_filename);
}

void FLandscapeImporterModule::BuildLanscape(float *pafScanline, int x_size, int y_size, int o_x, int o_y, int s_x, int s_y, int add, int mult, float min, float max, bool set_min_to_zero, int x, int y)
{
	FText DialogText;

	if (o_y > y_size) {
		DialogText = FText::FromString("Error out of bounds");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}
	if (o_x > x_size) {
		DialogText = FText::FromString("Error out of bounds");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}

	if (s_y + o_y > y_size) {
		s_y = y_size - o_y;
	}
	if (s_x + o_x > x_size) {
		s_x = x_size - o_x;
	}


	/*float max = pafScanline[o_x+(o_y*x_size)];
	float min = pafScanline[o_x + (o_y*x_size)];
	float min_2 = pafScanline[o_x + (o_y*x_size)];

	for (int j = 0; j < s_y; j++) {
		for (int i = 0; i < s_x; i++) {
			int p = i + o_x + ((j + o_y)*x_size);
			if (pafScanline[p]>max) {
				max = pafScanline[p];
			}
			if (pafScanline[p]<min) {
				min = pafScanline[p];
			}
			if (pafScanline[p] > min) {
				if (min_2 == min) {
					min_2 = pafScanline[p];
				}
				else if (pafScanline[p]<min_2) {
					min_2 = pafScanline[p];
				}
			}
		}
	}

	int add = 0;
	int mult = 0;

	bool set_min_to_zero;
	if (min < 0) {
		set_min_to_zero = true;
		add = min_2 * -1;
		mult = 65535 / (max - min_2);
	}
	else {
		set_min_to_zero = false;
		add = min * -1;
		mult = 65535 / (max - min);

	}*/



	/*DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Max: {0} Min:{1} Min_2:{2}"),
		FText::FromString(FString::FromInt(max)),
		FText::FromString(FString::FromInt(min)),
		FText::FromString(FString::FromInt(min_2))
		);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);


	DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Add: {0} Mult:{1} SizeZ:{2}"),
		FText::FromString(FString::FromInt(add)),
		FText::FromString(FString::FromInt(mult)),
		FText::FromString(FString::FromInt((max + add)*mult))
		);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);*/

	const int32 SizeX = s_x;
	const int32 SizeY = s_y;
	const int32 SizeZ = (max + add)*mult*(100.0f / mult);
	const int32 SectionsPerComponent = 1;
	const int32 QuadsPerSection = 511;
	const int32 QuadsPerComponent = SectionsPerComponent*QuadsPerSection;
	// FVector Location = FVector(SizeX*-0.5f*100.0f, SizeY*-0.5f*100.0f, SizeZ*0.5f);
	FVector Location = FVector(o_x*100.0f, o_y*100.0f, SizeZ*0.5f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector Scale = FVector(100.0f, 100.0f, (100.0f / mult) * 100);

	// GEditor->GetEditorWorldContext().World()
	ALandscape* Landscape = GEditor->GetEditorWorldContext().World()->SpawnActor<ALandscape>(Location, Rotation);
	Landscape->SetActorRelativeScale3D(Scale);

	//const TArray<uint16>& Data = GetImportLandscapeData();
	TArray<uint16> Data;
	Data.AddUninitialized(SizeX * SizeY);
	uint16* WordData = Data.GetData();
	//uint16* ImportWordData = ImportLandscape_Data.GetData();
	// Initialize blank heightmap data

	for (int j = 0; j < s_y; j++) {
		for (int i = 0; i < s_x; i++) {
			int p = i + o_x + ((j + o_y)*x_size);
			uint16 height = ((pafScanline[p]+add)*mult);
			if (pafScanline[p] == min && set_min_to_zero) {
				WordData[i+(j*s_x)] = 0;
			}
			else {
				WordData[i+ (j*s_x)] = height;
			}
		}
	}
	/*for (int32 i = 0; i < SizeX * SizeY; i++)
	{
		uint16 height = pafScanline[i];
		if (pafScanline[i] == min && set_min_to_zero) {
			WordData[i] = 1;
		}
		else {
			WordData[i] = (height + add)*mult;
		}

		// WordData[i] = ImportWordData[i];
		// WordData[i] = 32768; //max: 65535
	}*/

	//Set material before importing in order to avoid blank meshes.....

	std::string texture_path = "Texture2D'/Game/Landscape/Textures/T_Landscape_" + std::to_string(x) + "_" + std::to_string(y) + ".T_Landscape_" + std::to_string(x) + "_" + std::to_string(y) + "'";
	static UTexture2D *texture;
	texture = NULL;
	texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString(texture_path.c_str())));
	//texture->MipGenSettings = TMGS_NoMipmaps;
	//texture->MarkPackageDirty();
	static UMaterial *material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Landscape.Landscape'")));

	UMaterialInstanceConstant* NewMaterialInstance = NULL;
	UClass* FactoryClass = UMaterialInstanceConstantFactoryNew::StaticClass();
	UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
	if (Factory->ConfigureProperties()) {
		std::string AssetName = "test" + std::to_string(x) + "_" + std::to_string(y);
		FString PackagePath = "/Game/Landscape/Materials";
		FAssetToolsModule & AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		NewMaterialInstance = CastChecked<UMaterialInstanceConstant>(AssetToolsModule.Get().CreateAsset(AssetName.c_str(), PackagePath, UMaterialInstanceConstant::StaticClass(), Factory));
	}
	else {
		DialogText = FText::FromString("Error: Factory could not Configure Properties.");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}

	if (!texture ) {
		DialogText = FText::FromString("");
		DialogText = FText::Format(
			LOCTEXT("PluginButtonDialogText", "Error: Texture was not found: {0}"),
			FText::FromString(texture_path.c_str())
			);
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}

	if (!NewMaterialInstance) {
		DialogText = FText::FromString("Error: Material Instance was not created.");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	} else {
		NewMaterialInstance->SetFlags(RF_Standalone);
		NewMaterialInstance->SetParentEditorOnly(material);

		NewMaterialInstance->SetTextureParameterValueEditorOnly("Texture", texture);
		//Scale = Block Size (8192) * (DEM_Width/JP2_WIDTH)
		NewMaterialInstance->SetVectorParameterValueEditorOnly("Scale", FLinearColor(2049.082031, 2049.082031, 0, 0));

		NewMaterialInstance->MarkPackageDirty();
		NewMaterialInstance->PostEditChange();

		Landscape->LandscapeMaterial = NewMaterialInstance;
		Landscape->MarkPackageDirty();
	}

	//UMaterialInstanceConstant *test = (UMaterialInstanceConstant*)InstanceFactory->;
	//UMaterialInstanceDynamic *dynam = UMaterialInstanceDynamic::Create(material, Landscape);

	//CreateMaterialInstance(0, 0, Landscape);



	TArray<FLandscapeImportLayerInfo> LayerInfos;
	Landscape->Import(FGuid::NewGuid(), SizeX, SizeY, QuadsPerComponent, SectionsPerComponent, QuadsPerSection, Data.GetData(), NULL, LayerInfos);	
}

void FLandscapeImporterModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FLandscapeImporterCommands::Get().PluginAction);
}

void FLandscapeImporterModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FLandscapeImporterCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLandscapeImporterModule, LandscapeImporter)