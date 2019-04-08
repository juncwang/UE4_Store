// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseMode.h"
#include "CameraPlayerController.h"
#include "CameraPawn.h"

// Sets default values
ABaseMode::ABaseMode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));

	RootComponent = SceneComp;
	StaticMeshComp->SetupAttachment(SceneComp);

	BaseModeState = EBaseModeState::BaseModeMove;

	OnClicked.AddDynamic(this, &ABaseMode::actorOnClicked);
}

// Called when the game starts or when spawned
void ABaseMode::BeginPlay()
{
	Super::BeginPlay();

	CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
	FCollisionQueryParams _params(FName(TEXT("CombatTrace")), true, NULL);
	_params.AddIgnoredActor(CameraPlayerController->GetPawn());
	_params.AddIgnoredActor(this);
	_params.bReturnPhysicalMaterial = false;

	params = _params;
	
}

// Called every frame
void ABaseMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (BaseModeState == EBaseModeState::BaseModeMove && GetWorld())
	{
		FVector ScenePoint;
		FVector Direction;
		// 获取控制器
		ACameraPlayerController* CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
		// 获取鼠标在空间内的点及方向
		CameraPlayerController->DeprojectMousePositionToWorld(ScenePoint, Direction);
		Direction = Direction * 10000.0 + ScenePoint;
		// 碰撞忽略对象
// 		FCollisionQueryParams params(FName(TEXT("CombatTrace")), true, NULL);
// 		params.AddIgnoredActor(CameraPlayerController->GetPawn());
// 		params.AddIgnoredActor(this);
// 		params.bReturnPhysicalMaterial = false;

		// 定义碰撞射线, 强制初始化力为零
		FHitResult hit(ForceInitToZero);
		// 发射射线
		GetWorld()->LineTraceSingleByChannel(hit, ScenePoint, Direction, ECC_MAX, params);
		SetActorLocation(hit.Location);
	}
}

void ABaseMode::actorOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	ACameraPawn* CameraPawn = Cast<ACameraPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (BaseModeState == EBaseModeState::BaseModeStatic && CameraPawn->WorF_Comp == nullptr && CameraPawn->bMouseController && CameraPawn->bMouseUI)
	{
		if (CameraPawn->PawnBaseMode == nullptr)
		{
			BaseModeState = EBaseModeState::BaseModeSelect;
			bMouseSelect = true;
			CameraPawn->setBaseMode(this);
			setRender(true);
			CameraPawn->ShowUImodeSelect();
		}
	}
}

void ABaseMode::initStaticMesh(UStaticMesh *mesh, int32 id)
{
	StaticMeshComp->SetStaticMesh(mesh);
	gid = id;
}

void ABaseMode::setRender(bool isShow)
{
	StaticMeshComp->SetRenderCustomDepth(isShow);
}

void ABaseMode::destroySelf()
{
	Destroy(true);
}

void ABaseMode::OutPrint(FString myString)
{
	if (GEngine)
	{
		myString = "ABaseMode::" + myString;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, myString);
	}
}

