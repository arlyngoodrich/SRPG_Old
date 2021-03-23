// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPM_Character.h"

#define LOCTEXT_NAMESPACE "FTPM_CharacterModule"

DEFINE_LOG_CATEGORY(LogTPCharacter);

void FTPM_CharacterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FTPM_CharacterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTPM_CharacterModule, TPM_Character)