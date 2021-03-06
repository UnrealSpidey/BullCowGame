// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBullCowGame, Log, All); //General Log
DECLARE_LOG_CATEGORY_EXTERN(LogBullCowGameInit, Log, All); //Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(LogBullCowGameCriticalErrors, Log, All);  

struct FBullCowCount
{
	int32 Bulls = 0; // could have left them uninitialized 
	int32 Cows = 0;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Word) const;
	TArray<FString> GetValidWords(const TArray<FString>& WordList) const;
	FBullCowCount GetBullCows(const FString& Guess) const;

	private:
	FString HiddenWord;
	int32 PlayerGuesses;
	bool bGameOver;
	TArray<FString> Isograms;
};
