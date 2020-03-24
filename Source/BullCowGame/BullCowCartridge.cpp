// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordlist.h"


DEFINE_LOG_CATEGORY(LogBullCowGame);
DEFINE_LOG_CATEGORY(LogBullCowGameInit);
DEFINE_LOG_CATEGORY(LogBullCowGameCriticalErrors);

void UBullCowCartridge::BeginPlay() // When the game starts
{
    GetValidWords(Words);
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
   HiddenWord = "cakes"; 
        UE_LOG(LogBullCowGame, Log, TEXT("HiddenWord is %s"), *HiddenWord);
        UE_LOG(LogBullCowGame, Log, TEXT("HiddenWord has %d characters"), HiddenWord.Len());
    PlayerGuesses = HiddenWord.Len();
        UE_LOG(LogBullCowGame, Log, TEXT("PlayerGuess is %d"), PlayerGuesses);
    bGameOver = false;
        UE_LOG(LogBullCowGame, Log, TEXT("bGameOver is %d"), bGameOver);

    // Welcome The Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the hidden word to Win!"));
    PrintLine(TEXT("The hidden word is %d letters long and \nthe first letter is: %c"), HiddenWord.Len(), HiddenWord[0]);
    PrintLine(TEXT("You have %d guesses"), PlayerGuesses);
    PrintLine(TEXT("Type in your guess and press enter to contiue")); 
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
        PrintLine(TEXT("That is an invalid guess!\nRemember there can be no repeating letters."), HiddenWord.Len());
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

    // Prompt to guess again and tell how many guesses are left
    /*if (PlayerGuesses == 1)
    {
        PrintLine(TEXT("Guess again, you have %d guess left!"), PlayerGuesses);
    }*/
    
    PrintLine(TEXT("Guess again, you have %d guesses left!"), PlayerGuesses);
}


bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    //UE_LOG(LogBullCowGame, Log, TEXT("Word is %s"), *Word);

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            //UE_LOG(LogBullCowGame, Log, TEXT("Index is %d : %c"), Index, Word[Index]);
            //UE_LOG(LogBullCowGame, Log, TEXT("Comparison is %d : %c"), Comparison, Word[Comparison]);

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

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && !IsIsogram(Word))
        {
            //UE_LOG(LogBullCowGame, Log, TEXT("WordList %d : %s"), Index, *WordList[Index]);
            ValidWords.Emplace(Word);
        }
    }

    UE_LOG(LogBullCowGame, Log, TEXT("# of Words available: %d"), WordList.Num());
    UE_LOG(LogBullCowGame, Log, TEXT("# of ValidWords available: %d"), ValidWords.Num());
        
    return ValidWords;

    // Debugging code , look if can use this as an example of console commands
    //for (int32 Index = 0; Index < ValidWords.Num(); Index++)
    //{
    //   UE_LOG(LogBullCowGame, Log, TEXT("ValidWordList %d : %s"), Index, *ValidWords[Index]);
    //}
}