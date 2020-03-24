// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class BullCowGameEditorTarget : TargetRules
{
	public BullCowGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "BullCowGame" } );

		DefaultBuildSettings = BuildSettingsVersion.V2;

	}
}
