using UnrealBuildTool;

public class UI : ModuleRules
{
    public UI(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Echoes_of_Decay", "UMG", "AkAudio" });
    }
}