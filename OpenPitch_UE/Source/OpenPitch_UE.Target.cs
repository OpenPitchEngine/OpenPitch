// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class OpenPitch_UETarget : TargetRules
{
	public OpenPitch_UETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "OpenPitch_UE" } );
		
		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			bOverrideBuildEnvironment = true;
			AdditionalCompilerArguments = "-Wno-error -Wno-deprecated-builtins";
		}
	}
}
