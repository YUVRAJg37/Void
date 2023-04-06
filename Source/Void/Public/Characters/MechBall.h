// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MechBall.generated.h"

struct FInputActionValue;
UENUM()
enum class EMechBallState : uint8
{
	MBS_Ball UMETA(DisplayName = "Ball"),
	MBS_Mech UMETA(DisplayName = "Mech"),
	MBS_Transform UMETA(DisplayName = "Transform")
};

UCLASS()
class VOID_API AMechBall : public ACharacter
{
	GENERATED_BODY()

public:
	AMechBall();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	class UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MechBallState", meta = (AllowPrivateAccess = true))
	EMechBallState MechBallState;
	
protected:

	void Move(const FInputActionValue& Value);
	void Switch();

	void BallMovement(const FVector2D& InputDirection);
	void MechMovement(const FVector2D& InputDirection);
	
	void TransformBody(float DeltaTime);
	
private:

	float CapsuleHalfHeightInBallForm;
	float CapsuleRadiusInBallForm;
	float CapsuleHalfHeightInMechForm;
	float CapsuleRadiusInMechForm;
	float TargetHalfHeight;

	bool bSwitchBody;
};


