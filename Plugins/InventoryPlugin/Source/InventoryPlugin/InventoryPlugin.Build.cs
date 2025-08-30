//

using UnrealBuildTool;

public class InventoryPlugin : ModuleRules
{
	public InventoryPlugin(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				"NetCore",
				"GameplayTags"
			]
		);


		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"EnhancedInput",
				"UMG",
				"MessageLog",
				"InputCore"
			]
		);

		if(target.Type == TargetRules.TargetType.Editor)
		{
			PublicDependencyModuleNames.AddRange(["UnrealEd"]);
		}
	}
}
