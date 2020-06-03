// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    InitGame(); //Setting Up the Game
    //PrintLine(TEXT("The number of possible words is: %i"), Words.Num()); //Debug Line
    //PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord); //Debug Line
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    //if game is over... ClearScreen() and GameInit() the game
    //else continue checking the player's guess
    if (bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else
    {
        
        {
            GuessCheck(Input);
        }
    }    
}

void UBullCowCartridge::InitGame()
{
    PrintLine(TEXT("Howdy!\nWeclome to Bull Cows!\n"));
    PrintLine(TEXT("Guess the hidden word!\n"));
    PrintLine(TEXT("There are no repeating letters\n"));
    PrintLine(TEXT("X = Cow, * = Bull\n"));

    //i32t32 RandomWordSelector = rand() % (Words.Num()-1);
    int32 RandomWordSelector = FMath::RandRange(0, Words.Num()-1);
    //PrintLine(TEXT("The HiddenWordindex is: %i"), RandomWordSelector); //Debug Line
    while(!IsogramCheck(*Words[RandomWordSelector]))
    {   
        RandomWordSelector = FMath::RandRange(0, Words.Num()-1);
    }
    HiddenWord = *Words[RandomWordSelector];

    Lives = HiddenWord.Len();
    bGameOver = false;
    
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type your guess of %i letters\nPress Enter to continue...\n"), HiddenWord.Len());
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again"));
}

void UBullCowCartridge::GuessCheck(const FString& Guess)
{
    //PrintLine("Hello %s", WordList[1]);
    if (HiddenWord.ToUpper() == Guess.ToUpper())
    {
        PrintLine(TEXT("You guessed the word! You win!"));
        EndGame();
        return;
    }
    if (Guess.Len() > HiddenWord.Len())
    {
        PrintLine(TEXT("Your guess is too long\n"));
    }
    else 
    {
        FBullCowCount Count = GetBullCows(*Guess);
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);
    }

    if (HiddenWord.Len() != Guess.Len()) {
       int32 LengthDiff = HiddenWord.Len() - Guess.Len();
       if (HiddenWord.Len() < Guess.Len())
       {
          LengthDiff *= -1;
       }
       PrintLine(TEXT("Your guess was off by: %i characters\n"), LengthDiff);
       PrintLine(TEXT("Try guessing again\n"), LengthDiff);
       //return;
    }
    //Check If Isogram
    bool IsIsogram = IsogramCheck(Guess);
    if (!IsIsogram)
    {
        PrintLine(TEXT("No repeating letters, guess again\n"));
        return;
    }
    --Lives;
    if (Lives <= 0) {
        ClearScreen();
        PrintLine(TEXT("No Lives Remain\nYou Lost!\nThe Hidden Word was: %s"), *HiddenWord);
        EndGame();
        return;
    }
    else
        PrintLine(TEXT("You have %i lives remaining\nTry Again!"), Lives);
}

bool UBullCowCartridge::IsogramCheck(FString Word) const
{
    const TCHAR* InputCheck = *Word;
    for (int32 i = 0; i < Word.Len(); i++) {
        for(int32 j = i+1; j< Word.Len(); j++)
        if (InputCheck[i] == InputCheck[j])
        {
            return false;
        }
    }
    return true;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& PlayerGuess) const
{
    FBullCowCount Count;
    FString BullSymbol = TEXT("*");
    FString CowSymbol = TEXT("x");
    TCHAR NoCowOrBull = '_';
    FString BullCowsString = *PlayerGuess;
    //For every index PlayerGuess is the same as index Hiddenword, BullCount ++
    // if not a bull, was it a cow? if yes CowCount++
    for (int32 PlayerGuessIndex = 0; PlayerGuessIndex < PlayerGuess.Len(); PlayerGuessIndex++)
    {
        if (PlayerGuess[PlayerGuessIndex] == HiddenWord[PlayerGuessIndex])
        {
            BullCowsString[PlayerGuessIndex] = BullSymbol[0];
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
        {
            if (PlayerGuess[PlayerGuessIndex] == HiddenWord[HiddenWordIndex])
            {
                BullCowsString[PlayerGuessIndex] = CowSymbol[0];
                Count.Cows++;
                break;
            }
            else
                BullCowsString[PlayerGuessIndex] = NoCowOrBull;
        }
    }
    PrintLine(TEXT("%s"), *BullCowsString);
    return Count;
}