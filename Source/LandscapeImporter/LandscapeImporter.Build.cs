// Some copyright should be here...

using UnrealBuildTool;
using System.IO;

public class LandscapeImporter : ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }        
    }

	public LandscapeImporter(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"LandscapeImporter/Public",
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"LandscapeImporter/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"CoreUObject", "Engine", "Slate", "SlateCore", "Landscape", "Engine", "ImageWrapper", "Core", "AssetTools"
				
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
				// ... add any modules that your module loads dynamically here ...
			}
			);

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string LibrariesPath = Path.Combine(ThirdPartyPath, "GDAL", "Libraries/Win64");
            
            // This does not work! You must put gdal200.dll in your path.
            // RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPath, "gdal200.dll")));

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "gdal_i.lib"));
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "GDAL", "Includes"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			string LibrariesPath = Path.Combine(ThirdPartyPath, "GDAL", "Libraries/Darwin");
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libgdal.dylib"));
		}

		// Includes are the same (hopefully) for all platforms...
		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "GDAL", "Includes"));
	}

    /*public bool LoadGDALsMagic(TargetInfo Target)
    {
        bool isLibrarySupported = false;
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibrarySupported = true;

            //string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "GDAL", "Libraries");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "gdal111.dll"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "GDAL", "Includes"));
        }

        Definitions.Add(string.Format("WITH_GDALS_MAGIC_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }*/
}
