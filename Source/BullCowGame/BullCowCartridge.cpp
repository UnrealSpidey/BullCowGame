// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordlist.h"

DEFINE_LOG_CATEGORY(LogBullCowGame); //General Log
DEFINE_LOG_CATEGORY(LogBullCowGameInit); //Logging during game startup
DEFINE_LOG_CATEGORY(LogBullCowGameCriticalErrors);

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    Isograms = GetValidWords(Words);
    
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver) // can do this instead of bGameOver == true becasue we are checking if true to start with.
    {
        ClearScreen();
        SetupGame();
    }  
    else // Checkking Players Guess
    {
        UE_LOG(LogBullCowGame, Log, TEXT("Input is %s"), *PlayerInput);

        ProcessGuess(PlayerInput);   
    }   
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //forgot the - 1 when setting this up.
        UE_LOG(LogBullCowGame, Log, TEXT("HiddenWord is %s"), *HiddenWord);
        UE_LOG(LogBullCowGame, Log, TEXT("HiddenWord has %d characters"), HiddenWord.Len());
    PlayerGuesses = HiddenWord.Len();
        UE_LOG(LogBullCowGame, Log, TEXT("PlayerGuess is %d"), PlayerGuesses);
    bGameOver = false;
        UE_LOG(LogBullCowGame, Log, TEXT("bGameOver is %d"), bGameOver);

    // Welcome The Player
    PrintLine(TEXT("Welcome to Bulls & Cows!"));
    PrintLine(TEXT("Can you guess the %d letter hidden word?"), HiddenWord.Len());
    PrintLine(TEXT("You have %d guesses"), PlayerGuesses);
    PrintLine(TEXT("Hint the hidden words first letter is %c"), HiddenWord[0]);
    PrintLine(TEXT("Type in your guess and press enter!"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again!"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("You guessed the hidden word!")); // look at adding how many guesses it took
        EndGame();
        return; 
    }

    // Check if guess has the right number of characters
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("That is an invalid guess!\nRemember the Hidden word is %d characters."), HiddenWord.Len());
        return;
    }

    // Check if guess is not an Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("That is an invalid guess! Remember there can be no repeating letters."), HiddenWord.Len());
        return;
    }

    // Remove a Guess
    --PlayerGuesses;
        UE_LOG(LogBullCowGame, Log, TEXT("PlayerGuess is now %d"), PlayerGuesses);

    if (PlayerGuesses <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have run out of guesses! \nThe hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    //Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);  

    PrintLine(TEXT("You have %d Bulls and %d Cows"), Score.Bulls, Score.Cows);
    UE_LOG(LogBullCowGame, Log, TEXT("Bulls %d | Cows %d"), Score.Bulls, Score.Cows);
    
    // Prompt to guess again and tell player how many guesses are left
    if (PlayerGuesses == 1)
    {
        PrintLine(TEXT("Guess again, you have %d guess left!"), PlayerGuesses); // 1 Guess
    }
    else
    {
        PrintLine(TEXT("Guess again, you have %d guesses left!"), PlayerGuesses); // # Guesses
    }
}


bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    //UE_LOG(LogBullCowGame, Log, TEXT("Word is %s"), *Word);

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            UE_LOG(LogBullCowGame, Verbose, TEXT("Index is %d : %c"), Index, Word[Index]);
            UE_LOG(LogBullCowGame, Verbose, TEXT("Comparison is %d : %c"), Comparison, Word[Comparison]);

            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    
    return true;
}


TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList) //test
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    UE_LOG(LogBullCowGame, Log, TEXT("# of Words available: %d"), WordList.Num());
    UE_LOG(LogBullCowGame, Log, TEXT("# of ValidWords available: %d"), ValidWords.Num());
    UE_LOG(LogBullCowGame, Log, TEXT("ValidWords - 1: %d"),ValidWords.Num() - 1);
            
    return ValidWords;
}


FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    // Bulls are given for every correct letter in correct location
    // Cows are given for every correct letter guessed but is not in the right location
    // eg Word is cakes, and guess kaces, 3 bulls and 2 cows
    // This is case sensitive 

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}