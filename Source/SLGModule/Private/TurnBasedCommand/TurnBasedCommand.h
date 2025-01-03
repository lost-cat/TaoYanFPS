// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

class ATurnBasedCharactor;

struct FTurnBasedCommandBase
{

protected:
	~FTurnBasedCommandBase() = default;

public:
	virtual void Execute() = 0;
	virtual void Undo() = 0;

private:
	bool bCanUndo = false;
};


struct FTurnBasedCommandMoveto : public FTurnBasedCommandBase
{
protected:
	FTurnBasedCommandMoveto(ATurnBasedCharactor* TargetCharacter, const FVector& TargetLocation);

	~FTurnBasedCommandMoveto() = default;

public:
	virtual void Execute() override;

private:
	ATurnBasedCharactor* Character;
	FVector TargetLocation;
	FVector StartLocation;
};
