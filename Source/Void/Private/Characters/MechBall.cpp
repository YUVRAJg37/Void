#include "Characters/MechBall.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

AMechBall::AMechBall() :
MechCapsuleHalfHeight(56.0f),
BallCapsuleHalfHeight(36.0f),
MechBallState(EMechBallState::MBS_Ball),
bSwitchBody(false),
MovementSpeed(20.0f),
ForceValue(30.0f),
StabilizeForceValue(1.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("Movement Component");

	RootComponent = CapsuleComponent;
	Mesh->SetupAttachment(CapsuleComponent);
	CameraBoom->SetupAttachment(GetRootComponent());
	FollowCamera->SetupAttachment(CameraBoom);
	
	CameraBoom->bUsePawnControlRotation = true;

	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetCapsuleHalfHeight(36.0f);
	CapsuleComponent->SetCapsuleRadius(36.0f);
	
}

void AMechBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMechBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if(bSwitchBody)
		TransformBody(DeltaTime);
}

void AMechBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &AMechBall::Switch);
	}
}

void AMechBall::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller!=nullptr)
	{
		switch (MechBallState)
		{
		case EMechBallState::MBS_Mech :
			{
				MechMovement(MovementVector);
				break;
			}
		case EMechBallState::MBS_Ball :
			{
				BallMovement(MovementVector);
				break;
			}
		default :
			{
				UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!!!!!!!!"));
			}
		}
	}
}

void AMechBall::BallMovement(const FVector2D& InputDirection)
{
	const FVector forceValue = ConvertToWorldDirection(InputDirection)*ForceValue;
	float dot = CalculateDirectionalForceEffect(ConvertToWorldDirection(InputDirection)*ForceValue);
	const FVector stabilizeForceValue = dot*forceValue*StabilizeForceValue;
	
	CapsuleComponent->AddTorqueInRadians(FVector{-InputDirection.Y*ForceValue, -InputDirection.X*ForceValue, 0}, NAME_None, true);
	CapsuleComponent->AddForce(stabilizeForceValue, NAME_None, true);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, -1, FColor::Red,FString::Printf(TEXT("Dot : %f"), dot));
		GEngine->AddOnScreenDebugMessage(2, -1, FColor::Red,FString::Printf(TEXT("Force : %s"), *stabilizeForceValue.ToString()));
		GEngine->AddOnScreenDebugMessage(3, -1, FColor::Red,FString::Printf(TEXT("Torque : %s"), *forceValue.ToString()));
	}
}

void AMechBall::MechMovement(const FVector2D& InputDirection)
{
	AddMovementInput(ConvertToWorldDirection(InputDirection)*MovementSpeed, 1, true);
}

float AMechBall::CalculateDirectionalForceEffect(FVector Direction)
{
	Direction.Normalize();
	FVector normalizedVelocity = GetVelocity();
	normalizedVelocity.Normalize();

	Direction.Z = 0;
	normalizedVelocity.Z = 0;

	float dot = FVector::DotProduct(normalizedVelocity, Direction);

	//Comverting from -1 to 1 => 0 to 1
	dot*=0.5;
	dot+=0.5;
	dot = 1 - dot;

	return dot;
}

FVector AMechBall::ConvertToWorldDirection(FVector2D Direction)
{
	FVector worldDirection = {-Direction.X, Direction.Y, 0};
	return worldDirection;
}

void AMechBall::Switch()
{
	bSwitchBody = true;
	
	switch (MechBallState)
	{
	case EMechBallState::MBS_Mech :
		{
			CapsuleComponent->SetSimulatePhysics(true);
			TargetHalfHeight = BallCapsuleHalfHeight;
			break;
		}
	case EMechBallState::MBS_Ball :
		{
			CapsuleComponent->SetSimulatePhysics(false);
			TargetHalfHeight = MechCapsuleHalfHeight;
			break;
		}
	default :
		{
			UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!!!!!!!!"));
		}
	}
	
	MechBallState = EMechBallState::MBS_Transform;
}

void AMechBall::TransformBody(float DeltaTime)
{
	float interpHalfHeight = FMath::FInterpTo(CapsuleComponent->GetScaledCapsuleHalfHeight(), TargetHalfHeight, DeltaTime, 5.0f);
	float deltaCapusleHeight = interpHalfHeight - CapsuleComponent->GetScaledCapsuleHalfHeight();
	FVector meshOffset = {0,0,-deltaCapusleHeight};
	
	Mesh->AddLocalOffset(meshOffset);
	CapsuleComponent->SetCapsuleHalfHeight(interpHalfHeight);

	if(interpHalfHeight == TargetHalfHeight)
	{
		bSwitchBody = false;
		if(TargetHalfHeight == BallCapsuleHalfHeight)
			MechBallState = EMechBallState::MBS_Ball;
		else
			MechBallState = EMechBallState::MBS_Mech;
	}
}