// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MechBall.generated.h"

class AGun;
struct FInputActionValue;
UENUM()
enum class EMechBallState : uint8
{
	MBS_Ball UMETA(DisplayName = "Ball"),
	MBS_Mech UMETA(DisplayName = "Mech"),
	MBS_Transform UMETA(DisplayName = "Transform")
};

UCLASS()
class VOID_API AMechBall : public APawn
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (AllowPrivateAccess = true))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	USceneComponent* GunSocket;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	class UFloatingPawnMovement* MovementComponent;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	class UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MechBallState", meta = (AllowPrivateAccess = true))
	EMechBallState MechBallState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float MechSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float BallSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float TurningHelpForceValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float TransformSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	TSubclassOf<AGun> DefaultGunClass;
	UPROPERTY()
	AGun* EquippedGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse", meta = (AllowPrivateAccess = true))
	float MouseTraceRange;
	
protected:

	void Move(const FInputActionValue& Value);
	void Switch();

	void BallMovement(const FVector2D& InputDirection);
	void MechMovement(const FVector2D& InputDirection);
	FVector ConvertToWorldDirection(FVector2D Direction);
	float CalculateTurningHelpForceImpact(FVector Direction);
	
	void TransformBody(float DeltaTime);
	
	void EquipGun(AGun* Gun);
	AGun* SpawnDefaultGun();

	FVector GetMouseHitLocation();
	void RotateGunToFollowMouse();
	
private:

	APlayerController* PlayerController;
	float BallCapsuleHalfHeight;
	float MechCapsuleHalfHeight;
	float TargetHalfHeight;

	bool bSwitchBody;
};


