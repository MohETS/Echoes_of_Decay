using UnrealBuildTool;

public class UI : ModuleRules

{

    public UI(ReadOnlyTargetRules Target) : base(Target)
    {

        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "Echoes_of_Decay", "AkAudio" });

    }

}