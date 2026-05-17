using UnrealBuildTool;
using System.IO;

public class OpenPitch : ModuleRules
{
	public OpenPitch(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		string BasePath = ModuleDirectory;

		string LibraryPath = Path.Combine(
			BasePath,
			"Mac",
			"libOpenPitch.dylib"
		);

		PublicIncludePaths.Add(
			Path.Combine(BasePath, "Includes")
		);

		PublicAdditionalLibraries.Add(LibraryPath);

		RuntimeDependencies.Add(LibraryPath);

		PublicRuntimeLibraryPaths.Add(
			Path.Combine(BasePath, "Mac")
		);
	}
}