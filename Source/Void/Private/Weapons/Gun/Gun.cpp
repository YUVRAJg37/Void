#include "Weapons/Gun/Gun.h"

#include "Components/CapsuleComponent.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Gun Mesh");

	RootComponent = GunMesh;
	GunMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::Shoot()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, -1, FColor::Red, FString::Printf(TEXT("Gun Class Fire Function Called!!!")));
	}
}

