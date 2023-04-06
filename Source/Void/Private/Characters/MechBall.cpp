#include "Characters/MechBall.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMechBall::AMechBall() :
CapsuleHalfHeightInMechForm(56.0f),
CapsuleHalfHeightInBallForm(36.0f),
MechBallState(EMechBallState::MBS_Ball),
bSwitchBody(false)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");

	CameraBoom->SetupAttachment(GetRootComponent());
	FollowCamera->SetupAttachment(CameraBoom);
	
	CameraBoom->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetSimulatePhysics(false);
	
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
}

void AMechBall::MechMovement(const FVector2D& InputDirection)
{
}

void AMechBall::Switch()
{
	bSwitchBody = true;
	
	if(MechBallState == EMechBallState::MBS_Mech)
			TargetHalfHeight = CapsuleHalfHeightInBallForm;
	if(MechBallState == EMechBallState::MBS_Ball)
			TargetHalfHeight = CapsuleHalfHeightInMechForm;
	
	MechBallState = EMechBallState::MBS_Transform;
}

void AMechBall::TransformBody(float DeltaTime)
{
	float interpHalfHeight = FMath::FInterpTo(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), TargetHalfHeight, DeltaTime, 5.0f);
	float deltaCapusleHeight = interpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector meshOffset = {0,0,-deltaCapusleHeight};
	
	GetMesh()->AddLocalOffset(meshOffset);
	GetCapsuleComponent()->SetCapsuleHalfHeight(interpHalfHeight);

	if(interpHalfHeight == TargetHalfHeight)
	{
		bSwitchBody = false;
		if(TargetHalfHeight == CapsuleHalfHeightInBallForm)
			MechBallState = EMechBallState::MBS_Ball;
		else
			MechBallState = EMechBallState::MBS_Mech;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Target : %f"), TargetHalfHeight);
}
