// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "rapidjson/document.h"
#include "CameraPlayerController.h"
#include "EngineUtils.h"
#include "BaseMode.h"
#include "Materials/Material.h"
#include "PanoActor.h"
#include "PanoPoint.h"
#include "CameraGameInstance.h"
#include "Store_VRGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

using namespace rapidjson;

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }

void ACameraPawn::OpenMouseController()
{
	bMouseController = true;
}

void ACameraPawn::CloseMouseController()
{
	bMouseController = false;
}

void ACameraPawn::CreateUImodeSelect()
{
	if (T_UImodeSelect && GetWorld())
	{
		UImodeSelect = CreateWidget<UCUI_modeSelect>(GetWorld(), T_UImodeSelect);
		if (UImodeSelect)
		{
			UImodeSelect->AddToViewport();
			UImodeSelect->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ACameraPawn::ShowUImodeSelect()
{
	if (UImodeSelect)
	{
		bUImodeSelect = true;
		UImodeSelect->SetVisibility(ESlateVisibility::Visible);
		UImodeSelect->initModeUI();
	}
}

void ACameraPawn::CloseUImodeSelect()
{
	if (bUImodeSelect)
	{
		UImodeSelect->SetVisibility(ESlateVisibility::Hidden);
		bUImodeSelect = false;
	}
}

void ACameraPawn::CreateUIMaterialSelect()
{
	if (T_UImaterialSelect && GetWorld())
	{
		UImaterialSelect = CreateWidget<UCUI_materialSelect>(GetWorld(), T_UImaterialSelect);
		if (UImaterialSelect)
		{
			UImaterialSelect->AddToViewport();
			UImaterialSelect->SetVisibility(ESlateVisibility::Hidden);
			
		}
	}
}

void ACameraPawn::ShowUIMaterialSelect()
{
	if (UImaterialSelect)
	{
		bUImaterialSelect = true;
		UImaterialSelect->SetVisibility(ESlateVisibility::Visible);
		UImaterialSelect->initMaterialUI();
	}
}

void ACameraPawn::CloseUIMaterialSelect()
{
	if (bUImaterialSelect)
	{
		UImaterialSelect->SetVisibility(ESlateVisibility::Hidden);
		bUImaterialSelect = false;
	}
}

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
 	PrimaryActorTick.bCanEverTick = true;

	sw = LoadObject<USoundWave>(NULL, TEXT("SoundWave'/Game/Sound/BackgroundSound.BackgroundSound'"));
 
 	SphereCom = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCom"));
 	RootComponent = SphereCom;
 	SphereCom->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
 	SphereCom->SetCollisionResponseToAllChannels(ECR_Block);
 	SphereCom->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
 
 	ArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ArmCom"));
 	ArmCom->SetupAttachment(SphereCom);
 	ArmCom->bUsePawnControlRotation = true;
 	ArmCom->bDoCollisionTest = false;
 
 	CameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCom"));
 	CameraCom->SetupAttachment(ArmCom);
 
 	MovementCom = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementCom"));
 
 	// 使用控制器控制 Yaw 旋转
 	bUseControllerRotationYaw = true;
 
 	// Pawn 的初始状态
 	// PawnState = EPawnState::PawnSkyRotator;
	PawnState = EPawnState::PawnSkyRotator;

	/************************************************************************/
	/* 
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	*/
	/************************************************************************/
 	
 	PawnBaseMode = nullptr;
	WorF_Actor = nullptr;
 	WorF_Comp = nullptr;
	OpenMouseController();

	bMouseUI = true;
}

void ACameraPawn::InitCameraPawnData()
{
	FString result;
	FString file = "RuntimeData/CameraPawnData.json";
	FFileHelper::LoadFileToString(result, *file);
	Document jsonDoc; 
	jsonDoc.Parse(result);

	// pawn 移动速度及鼠标旋转速度及 Rot 是否反向
	MaxMoveRate = jsonDoc["MaxMoveRate"].GetFloat();
	MaxRotRate = jsonDoc["MaxRotRate"].GetFloat();
	bPitch = jsonDoc["bPitch"].GetBool();

	// ArmCom 的控制柄长度设置
	ArmComMinLength = jsonDoc["ArmComMinLength"].GetFloat();
	ArmComMaxLength = jsonDoc["ArmComMaxLength"].GetFloat();

	// 设置控制器 SKY 状态旋转值
	SkyControllerRotator = FRotator(jsonDoc["SkyRotatorPitch"].GetFloat() , jsonDoc["SkyRotatorYaw"].GetFloat(), jsonDoc["SkyRotatorRoll"].GetFloat());
	ActorFloorYaw = 0.0;

	// Pawn 状态切换速度
	PawnChangeSpeed = jsonDoc["PawnChangeSpeed"].GetFloat();

	// ArmCom 的长度容错率
	ArmComFaultTolerance = jsonDoc["ArmComFaultTolerance"].GetFloat();

	// 鼠标中建旋转模型的速度
	PawnBaseModeRotationSpeed = jsonDoc["PawnBaseModeRotationSpeed"].GetFloat();

	bRotationCase = jsonDoc["bRotationCase"].GetBool();
	RotationalMagnification = jsonDoc["RotationalMagnification"].GetFloat();
	if (RotationalMagnification < 2.0)
	{
		RotationalMagnification = 2.0;
	}

	CameraHeight = jsonDoc["CameraHeight"].GetFloat();
	ArmCom->SetRelativeLocation(FVector(0.0f, 0.0f, CameraHeight));

	bPanoCap = false;

	InitArmLength();
	InitControlRotation();
	ActorLocation = GetActorLocation();
	ControlTargetRotation = GetControlRotation();
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

// 	ActorLocation = GetActorLocation();
// 	SetActorLocation(FVector::ZeroVector);

	// ------------------------------------ 数据初始化以后对系统进行设置的项目 -------------------------------

	

	if (GetWorld())
	{
		CGInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		gameModeBase = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	if (CGInstance->emap == EMaps::MainPage)
	{
		// ChangePawnState();
		InitCameraPawnData();
		CreateUImodeSelect();
		CreateUIMaterialSelect();
		if (sw)
		{
			print(TEXT("sw is not null"));
			sw->bLooping = true;
			print(TEXT("sw->bLooping = true"));
			UGameplayStatics::PlaySound2D(GetWorld(), sw, 1.0f);
			print(TEXT("UGameplayStatics::PlaySound2D(GetWorld(), sw, 1.0f)"));

		}
		else {
			OutPrint("Sound is null");
			print(TEXT("Sound is null"));
		}
		
	}

}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float DeltaSecond = GetWorld()->GetDeltaSeconds();

	if (CGInstance->emap == EMaps::MainPage)
	{
		if (PawnState == EPawnState::PawnChanging && GetWorld())
		{
			if (PawnTargetState == EPawnState::PawnFloorMove)
			{
				ArmCom->TargetArmLength = FMath::FInterpTo(ArmCom->TargetArmLength, ArmComMinLength, DeltaSecond, PawnChangeSpeed);
				SetActorLocation(FMath::VInterpTo(GetActorLocation(), ActorLocation, DeltaSecond, PawnChangeSpeed));
				float controllerRotationYaw = cameraPlayerController->GetControlRotation().Yaw;
				cameraPlayerController->SetControlRotation(FMath::RInterpTo(cameraPlayerController->GetControlRotation(), FRotator(0.0f, ActorFloorYaw, 0.0f), DeltaSecond, PawnChangeSpeed));
				if (ArmCom->TargetArmLength <= ArmComMinLength + ArmComFaultTolerance)
				{
					PawnState = PawnTargetState;
					cameraPlayerController->bShowMouseCursor = true;
					cameraPlayerController->bEnableClickEvents = true;
				}

			}
			else if (PawnTargetState == EPawnState::PawnSkyRotator)
			{
				ArmCom->TargetArmLength = FMath::FInterpTo(ArmCom->TargetArmLength, ArmComMaxLength, DeltaSecond, PawnChangeSpeed);
				SetActorLocation(FMath::VInterpTo(GetActorLocation(), FVector::ZeroVector, DeltaSecond, PawnChangeSpeed));
				cameraPlayerController->SetControlRotation(FMath::RInterpTo(cameraPlayerController->GetControlRotation(), SkyControllerRotator, DeltaSecond, PawnChangeSpeed));
				if (ArmCom->TargetArmLength >= ArmComMaxLength - ArmComFaultTolerance)
				{
					PawnState = PawnTargetState;
					cameraPlayerController->bShowMouseCursor = true;
					cameraPlayerController->bEnableClickEvents = false;
				}
			}
			ControlTargetRotation = GetControlRotation();
		}
		else if (bMouseRightDown && bRotationCase)
		{
			fMouseRightTime = fMouseRightTime + DeltaTime;
		}
		else if (bRotationCase)
		{
			cameraPlayerController->SetControlRotation(FMath::RInterpTo(cameraPlayerController->GetControlRotation(), ControlTargetRotation, DeltaSecond, PawnChangeSpeed));
		}
	}
	
	
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("RotYaw", this, &ACameraPawn::RotYaw);
	PlayerInputComponent->BindAxis("RotPitch", this, &ACameraPawn::RotPitch);

	PlayerInputComponent->BindAction("MouseRight", IE_Pressed, this, &ACameraPawn::MouseRightDown);
	PlayerInputComponent->BindAction("MouseRight", IE_Released, this, &ACameraPawn::MouseRightUp);
	PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &ACameraPawn::MouseLeftDown);
	PlayerInputComponent->BindAction("MouseWheelUp", IE_Pressed, this, &ACameraPawn::MouseWheelUp);
	PlayerInputComponent->BindAction("MouseWheelDown", IE_Pressed, this, &ACameraPawn::MouseWheelDown);
// 
// 	PlayerInputComponent->BindAction("TestFunction", IE_Pressed, this, &ACameraPawn::ChangePawnState);		// z
	//PlayerInputComponent->BindAction("TestFunction2", IE_Pressed, this, &ACameraPawn::SpawnNewMode);		// x
// 	PlayerInputComponent->BindAction("TestFunction3", IE_Pressed, this, &ACameraPawn::PawnBaseModeMove);	// c
// 	PlayerInputComponent->BindAction("TestFunction4", IE_Pressed, this, &ACameraPawn::RemovePawnBaseMode);	// v
	//PlayerInputComponent->BindAction("TestFunction5", IE_Pressed, this, &ACameraPawn::ChangeNewMateril);	// b
// 	PlayerInputComponent->BindAction("TestFunction6", IE_Pressed, this, &ACameraPawn::LoadAllBaseActor);	// n
// 	PlayerInputComponent->BindAction("TestFunction7", IE_Pressed, this, &ACameraPawn::PanoSimpleCap);		// f
// 	PlayerInputComponent->BindAction("TestFunction8", IE_Pressed, this, &ACameraPawn::PanoAllCap);			// g
}

void ACameraPawn::MoveForward(float val)
{
	if (val != 0 && GetController() && GetWorld() && bMouseController)
	{
		if (PawnState == EPawnState::PawnFloorMove) {
			AddMovementInput(GetActorForwardVector(), val*MaxMoveRate*GetWorld()->GetDeltaSeconds());
		}
		else if (PawnState == EPawnState::PawnSkyRotator)
		{

		}
	}
}

void ACameraPawn::MoveRight(float val)
{
	if (val != 0 && GetController() && GetWorld() && bMouseController)
	{
		if (PawnState == EPawnState::PawnFloorMove) {
			AddMovementInput(GetActorRightVector(), val*MaxMoveRate*GetWorld()->GetDeltaSeconds());
		}
		else if (PawnState == EPawnState::PawnSkyRotator)
		{

		}
	}
}

void ACameraPawn::RotYaw(float val)
{
	if (val != 0 && GetController() && GetWorld() && bMouseRightDown)
	{
		if (PawnState == EPawnState::PawnFloorMove) {
			AddControllerYawInput(val*GetWorld()->GetDeltaSeconds()*MaxRotRate);
		}
		else if (PawnState == EPawnState::PawnSkyRotator)
		{
			AddControllerYawInput(val*GetWorld()->GetDeltaSeconds()*MaxRotRate);
		}
	}
}

void ACameraPawn::RotPitch(float val)
{
	if (val != 0 && GetController() && GetWorld() && bMouseRightDown)
	{
		if (PawnState == EPawnState::PawnFloorMove) {
			if (bPitch)
			{
				AddControllerPitchInput(val*GetWorld()->GetDeltaSeconds()*MaxRotRate*-1);
			}
			else
			{
				AddControllerPitchInput(val*GetWorld()->GetDeltaSeconds()*MaxRotRate);
			}
			
		}
		else if (PawnState == EPawnState::PawnSkyRotator)
		{

		}
	}
}

void ACameraPawn::InitArmLength()
{
	if (PawnState == EPawnState::PawnSkyRotator)
	{
		ArmCom->TargetArmLength = ArmComMaxLength;
	}
	else if (PawnState == EPawnState::PawnFloorMove)
	{
		ArmCom->TargetArmLength = ArmComMinLength;
	}
}

void ACameraPawn::InitControlRotation()
{
	cameraPlayerController = Cast<ACameraPlayerController>(GetController());
	cameraPlayerController->bShowMouseCursor = true;
	cameraPlayerController->bEnableClickEvents = false;

	if (PawnState == EPawnState::PawnSkyRotator)
	{
		cameraPlayerController->SetControlRotation(SkyControllerRotator);
	} 
	else if (PawnState == EPawnState::PawnFloorMove)
	{
		cameraPlayerController->SetControlRotation(FRotator::ZeroRotator);
	}
}

void ACameraPawn::InitSceneActor()
{
	if (GetWorld())
	{
		SkyActorArr.Empty(NULL);
		FloorActorArr.Empty(NULL);
		WallActorArr.Empty(NULL);
		PanoActor = nullptr;
		PanoPointArr.Empty(NULL);
		AActor* actor;
		for (TActorIterator<AActor>It(GetWorld()); It; ++It)
		{
			actor = *It;
			if (actor)
			{
				if (actor->Tags.Num() > 0)
				{
					if (actor->Tags[0] == FName("skysphere"))
					{
						SkyActorArr.Push(actor);
					}
					else if (actor->Tags[0] == FName("floor"))
					{
						actor->OnClicked.AddDynamic(this, &ACameraPawn::FloorOnClick);
						FloorActorArr.Push(actor);
					}
					else if (actor->Tags[0] == FName("wall"))
					{
						actor->OnClicked.AddDynamic(this, &ACameraPawn::WallOnClick);
						WallActorArr.Push(actor);
					}
					else if (actor->Tags[0] == FName("pano"))
					{
						APanoActor* _panoActor = Cast<APanoActor>(actor);
						if (_panoActor)
						{
							PanoActor = _panoActor;
						}
					}
					else if (actor->Tags[0] == FName("panopoint"))
					{
						APanoPoint* _panoPoint = Cast<APanoPoint>(actor);
						if (_panoPoint)
						{
							PanoPointArr.Push(_panoPoint);
						}
					}
				}
			}
		}

		if (SkyActorArr.Num() > 0) 
		{
			if (PawnState == EPawnState::PawnSkyRotator)
			{
				SkyActorArr[0]->SetActorHiddenInGame(true);
			}
			else if (PawnState == EPawnState::PawnFloorMove)
			{
				SkyActorArr[0]->SetActorHiddenInGame(false);
			}
		}
// 		for (AActor* SkySphereActor : SkyActorArr)
// 		{
// 			if (PawnState == EPawnState::PawnSkyRotator)
// 			{
// 				SkySphereActor->SetActorHiddenInGame(true);
// 			}
// 			else if (PawnState == EPawnState::PawnFloorMove)
// 			{
// 				SkySphereActor->SetActorHiddenInGame(false);
// 			}
// 		}
	}
}

void ACameraPawn::ChangePawnState()
{
	if (PawnBaseMode == nullptr && WorF_Comp == nullptr)
	{
		cameraPlayerController->bShowMouseCursor = false;
		cameraPlayerController->bEnableClickEvents = false;
		if (PawnState == EPawnState::PawnSkyRotator)
		{
			AddControllerYawInput(0.1*GetWorld()->GetDeltaSeconds()*MaxRotRate);
			gameModeBase->isShowHouseData();
			PawnState = EPawnState::PawnChanging;
			PawnTargetState = EPawnState::PawnFloorMove;
			for (AActor* SkySphereActor : SkyActorArr)
			{
				SkySphereActor->SetActorHiddenInGame(false);
			}
		}
		else if (PawnState == EPawnState::PawnFloorMove)
		{
			AddControllerYawInput(0.1*GetWorld()->GetDeltaSeconds()*MaxRotRate);
			gameModeBase->isShowHouseData();
			PawnState = EPawnState::PawnChanging;
			PawnTargetState = EPawnState::PawnSkyRotator;
			ActorLocation = GetActorLocation();
			ActorFloorYaw = GetControlRotation().Yaw;
			for (AActor* SkySphereActor : SkyActorArr)
			{
				SkySphereActor->SetActorHiddenInGame(true);
			}
		}
		
	}
	
}

// void ACameraPawn::SpawnNewMode(FString staticMeshPath)    // 装入 UI 组件后使用
void ACameraPawn::SpawnNewMode(FString url, FString fileName, int32 gid)
{
	if (GetWorld() && PawnState == EPawnState::PawnFloorMove && PawnBaseMode == nullptr && WorF_Comp == nullptr)
	{
		
		FString staticMeshPath = "StaticMesh'/Game/" + url + "/" + fileName + "." + fileName + "'";
 		/*FString staticMeshPath = "StaticMesh'/Game/temp/Bedclothes/BedsideCabinets66VQ/BedsideCabinets66VQ.BedsideCabinets66VQ'";*/

		ABaseMode* baseMode = GetWorld()->SpawnActor<ABaseMode>(ABaseMode::StaticClass());
		UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, *staticMeshPath);
		if (baseMode && mesh)
		{
			baseMode->initStaticMesh(mesh, gid);
			baseMode->BaseModeState = EBaseModeState::BaseModeMove;
			baseMode->setRender(true);

			PawnBaseMode = baseMode;
		}
	}
}

void ACameraPawn::PawnBaseModeMove()
{
	if (PawnState == EPawnState::PawnFloorMove && PawnBaseMode != nullptr && WorF_Comp == nullptr && PawnBaseMode->BaseModeState == EBaseModeState::BaseModeSelect)
	{
		PawnBaseMode->BaseModeState = EBaseModeState::BaseModeMove;
	}
}

void ACameraPawn::RemovePawnBaseMode()
{
	if (PawnState == EPawnState::PawnFloorMove && PawnBaseMode != nullptr && WorF_Comp== nullptr && PawnBaseMode->BaseModeState == EBaseModeState::BaseModeSelect)
	{
		PawnBaseMode->destroySelf();
		PawnBaseMode = nullptr;
	}
}

// void ACameraPawn::ChangeNewMateril(FString staticMeshPath)    // 装入 UI 组件后使用
void ACameraPawn::ChangeNewMateril(FString url, FString fileName, FName gid)
{
	if (GetWorld() && PawnState == EPawnState::PawnFloorMove && PawnBaseMode == nullptr && WorF_Comp != nullptr)
	{
		FString materialPath = "Material'/Game/" + url + "/" + fileName + "." + fileName + "'";
		print(TEXT("%s"), *materialPath);

		UMaterial* material = LoadObject<UMaterial>(NULL, *materialPath);
		WorF_Actor->Tags.Push(gid);
		WorF_Comp->SetMaterial(0, material);
	}
}

void ACameraPawn::LoadAllBaseActor()
{
	if (GetWorld())
	{
		ModeActorArr.Empty(NULL);
		ABaseMode* baseMode;
		for (TActorIterator<ABaseMode>It(GetWorld()); It; ++It)
		{
			baseMode = *It;
			if (baseMode)
			{
				ModeActorArr.Push(baseMode);
			}
		}
	}
}

void ACameraPawn::PanoSimpleCap()
{
	if (PanoActor != nullptr && PawnState == EPawnState::PawnFloorMove && PawnBaseMode == nullptr && WorF_Comp == nullptr && bPanoCap == false)
	{
		PanoActor->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + CameraHeight));
		cameraPlayerController->SetViewTarget(PanoActor);
		bPanoCap = true;
		PanoActor->PanoSimpleStart();
	}
}

void ACameraPawn::PanoAllCap()
{
	if (PanoActor != nullptr && PawnState == EPawnState::PawnFloorMove && PawnBaseMode == nullptr && WorF_Comp == nullptr && bPanoCap == false)
	{
		/************************************************************************/
		/* 生成全景图每个点的信息                                               */
		/************************************************************************/
		FString _panoPointData;
		_panoPointData = "{\n";
		int32 i = 0;

		PanoActor->pointDataArray.Empty();

		PanoActor->panoPointArr.Empty(NULL);
		for (APanoPoint* _pp : PanoPointArr)
		{
			PanoActor->panoPointArr.Push(_pp->GetActorLocation());

			//================================================================================================

			FPointData fPointData;

			for (int32 x=2; x<_pp->Tags.Num(); x++)
			{
				FOtherArrowData fOtherArrowData;
				FString leftStr;
				FString rightStr;
				FString numStr;
				_pp->Tags[x].ToString().Split("_", &leftStr, &rightStr,ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				fOtherArrowData.arrowName = leftStr;
				rightStr.Split("_", &numStr, &rightStr);
				fOtherArrowData.arrowYaw = rightStr;

				fPointData.otherArrowArray.Push(fOtherArrowData);
			}

			fPointData.pointName = _pp->Tags[1].ToString();

			PanoActor->pointDataArray.Push(fPointData);

			//================================================================================================

			_panoPointData += "\"point" + FString::FromInt(i) + "\" : {\n";

			if (_pp->Tags.Num() >= 2)
			{
				_panoPointData += "\"name\" : \"" + _pp->Tags[1].ToString() + "\",\n";

				if (i < 10)
				{
					_panoPointData += "\"image\" : \"HighresScreenshot0000" + FString::FromInt(i) + ".jpg\",\n";
				}
				else if (i >= 10 && i <= 99)
				{
					_panoPointData += "\"image\" : \"HighresScreenshot000" + FString::FromInt(i) + ".jpg\",\n";
				}
			}
			_panoPointData += "\"arrow\" : [\n";
			if (_pp->Tags.Num() > 2)
			{
				_panoPointData += "{\n";
				for (int32 j = 2; j < _pp->Tags.Num(); j++)
				{
					FString leftStr;
					FString centerStr;
					FString rightStr;
					_pp->Tags[j].ToString().Split("_", &leftStr, &rightStr);
					centerStr = leftStr;
					rightStr.Split("_", &leftStr, &rightStr);
					centerStr += leftStr;
					_panoPointData += "\"name\" : \"" + centerStr + "\",\n";
					rightStr.Split("_", &leftStr, &rightStr);
					_panoPointData += "\"yaw\" : \"" + rightStr + "\",\n";
				}
				_panoPointData += "}\n";
			}
			_panoPointData += "],\n";
			_panoPointData += "},\n";
			i++;
		}
		_panoPointData += "}";
		cameraPlayerController->SetViewTarget(PanoActor);
		bPanoCap = true;
		PanoActor->panoPointData = _panoPointData;

		// OutPrint(_panoPointData);
		print(TEXT("pano all data : %s"), *_panoPointData);

		for (int32 i = 0; i<PanoActor->pointDataArray.Num(); i++)
		{
			for (int32 j = 0; j<PanoActor->pointDataArray[i].otherArrowArray.Num(); j++)
			{
				OutPrint(PanoActor->pointDataArray[i].otherArrowArray[j].arrowYaw);
				OutPrint(PanoActor->pointDataArray[i].otherArrowArray[j].arrowName);
			}
			OutPrint(PanoActor->pointDataArray[i].pointName);
			OutPrint("============================================================");
		}

		PanoActor->PanoAllStart();
	}
}

void ACameraPawn::MouseRightDown()
{
	if (bMouseController)
	{
		if (PawnState != EPawnState::PawnChanging)
		{
			bMouseRightDown = true;
			if (bRotationCase)
			{
				RotationDownYaw = GetControlRotation().Yaw;
				RotationDownPitch = GetControlRotation().Pitch;
				if (RotationDownPitch > 90.0)
				{
					RotationDownPitch = RotationDownPitch - 360.0;
				}
				fMouseRightTime = 0.0;
			}
		}
	}
	
}

void ACameraPawn::MouseRightUp()
{
	if (bMouseController)
	{
		if (PawnState != EPawnState::PawnChanging)
		{
			bMouseRightDown = false;

			if (bRotationCase)
			{
				RotationUpYaw = GetControlRotation().Yaw;
				RotationUpPitch = GetControlRotation().Pitch;

				if (RotationUpPitch > 90.0)
				{
					RotationUpPitch = RotationUpPitch - 360.0;
				}

				RotationUpPitch = RotationUpPitch - RotationDownPitch;

				RotationUpYaw = RotationUpYaw - RotationDownYaw;

				if (FMath::Abs(RotationUpYaw) >= 180.0)
				{
					if (RotationUpYaw > 0)
					{
						RotationUpYaw = RotationUpYaw - 360.0;
					}
					else if (RotationUpYaw < 0)
					{
						RotationUpYaw = 360.0 + RotationUpYaw;
					}
				}

				ControlTargetRotation.Yaw = GetControlRotation().Yaw + RotationUpYaw / fMouseRightTime / RotationalMagnification;

				ControlTargetRotation.Pitch = GetControlRotation().Pitch + RotationUpPitch / fMouseRightTime / RotationalMagnification;
				if (ControlTargetRotation.Pitch > 90.0 && ControlTargetRotation.Pitch < 180.0)
				{
					ControlTargetRotation.Pitch = 90.0;
				}
				else if (ControlTargetRotation.Pitch < 270.0 && ControlTargetRotation.Pitch > 180.0)
				{
					ControlTargetRotation.Pitch = 270.0;
				}
				ControlTargetRotation.Roll = GetControlRotation().Roll;
			}
		}
	}
	

}

void ACameraPawn::MouseLeftDown()
{
	if (bMouseController)
	{
		if (PawnBaseMode != nullptr && WorF_Comp == nullptr&& PawnState == EPawnState::PawnFloorMove && PawnBaseMode->bMouseSelect == false)
		{
			print(TEXT("=======================================================PawnBaseMode->BaseModeState = EBaseModeState::BaseModeStatic;"));
			PawnBaseMode->BaseModeState = EBaseModeState::BaseModeStatic;
			PawnBaseMode->setRender(false);
			PawnBaseMode = nullptr;
			if (bUImodeSelect)
			{
				UImodeSelect->SetVisibility(ESlateVisibility::Hidden);
				bUImodeSelect = false;
			}

// 			if (UImodeSelect != nullptr)
// 			{
// 				UImodeSelect->RemoveFromParent();
// 				UImodeSelect = nullptr;
// 			}
		}
		else if (PawnBaseMode != nullptr && WorF_Comp == nullptr && PawnState == EPawnState::PawnFloorMove && PawnBaseMode->bMouseSelect == true)
		{
			PawnBaseMode->bMouseSelect = false;
			// 		if (UImodeSelect)
			// 		{
			// 			UImodeSelect->RemoveFromParent();
			// 		}
		}
		else if (PawnBaseMode == nullptr && WorF_Comp != nullptr && bMouseSelectWorF == false)
		{
			WorF_Comp->SetRenderCustomDepth(false);
			WorF_Actor = nullptr;
			WorF_Comp = nullptr;

			if (bUImaterialSelect)
			{
				UImaterialSelect->SetVisibility(ESlateVisibility::Hidden);
				bUImaterialSelect = false;
			}

			gameModeBase->CloseAllMenu();

// 			if (UImaterialSelect != nullptr)
// 			{
// 				UImaterialSelect->RemoveFromViewport();
// 				UImaterialSelect = nullptr;
// 			}
		}
		else if (PawnBaseMode == nullptr && WorF_Comp != nullptr && bMouseSelectWorF == true)
		{
			bMouseSelectWorF = false;
		}
	}
	
}

void ACameraPawn::MouseWheelUp()
{
	if (bMouseController)
	{
		if (PawnBaseMode != nullptr && WorF_Comp == nullptr && PawnState == EPawnState::PawnFloorMove && PawnBaseMode->BaseModeState == EBaseModeState::BaseModeMove)
		{
			PawnBaseMode->AddActorLocalRotation(FRotator(0.0, PawnBaseModeRotationSpeed, 0.0));
		}
	}
}

void ACameraPawn::MouseWheelDown()
{
	if (bMouseController)
	{
		if (PawnBaseMode != nullptr && WorF_Comp == nullptr && PawnState == EPawnState::PawnFloorMove && PawnBaseMode->BaseModeState == EBaseModeState::BaseModeMove)
		{
			PawnBaseMode->AddActorLocalRotation(FRotator(0.0, -PawnBaseModeRotationSpeed, 0.0));
		}
	}
	
}

void ACameraPawn::setBaseMode( ABaseMode* mode)
{
	PawnBaseMode = mode;
}

void ACameraPawn::FloorOnClick(AActor* TouchedActor, FKey ButtonPressed)
{
	if (PawnBaseMode == nullptr && WorF_Comp == nullptr && bMouseController && bMouseUI)
	{
		TArray<UStaticMeshComponent*> StaticMeshComArr;
		TouchedActor->GetComponents(StaticMeshComArr,false);

		WorF_Actor = TouchedActor;

		if (StaticMeshComArr.Num() > 0)
		{
			WorF_Comp = StaticMeshComArr[0];
			WorF_Comp->SetRenderCustomDepth(true);
			bMouseSelectWorF = true;
			gameModeBase->SetBrandType(2);
			ShowUIMaterialSelect();
		}
	}
}

void ACameraPawn::WallOnClick(AActor* TouchedActor, FKey ButtonPressed)
{
	if (PawnBaseMode == nullptr && WorF_Comp == nullptr && bMouseController && bMouseUI)
	{
		TArray<UStaticMeshComponent*> StaticMeshComArr;
		TouchedActor->GetComponents(StaticMeshComArr, false);

		WorF_Actor = TouchedActor;

		if (StaticMeshComArr.Num() > 0)
		{
			WorF_Comp = StaticMeshComArr[0];
			WorF_Comp->SetRenderCustomDepth(true);
			bMouseSelectWorF = true;
			gameModeBase->SetBrandType(3);
			ShowUIMaterialSelect();
		}
	}
}

void ACameraPawn::OutPrint(FString myString)
{
	if (GEngine)
	{
		myString = "ACameraPawn::" + myString;
		GEngine->AddOnScreenDebugMessage(-1, 99.0f, FColor::Green, myString);
	}
}

USoundWave* ACameraPawn::GetSoundWaveFromFile()
{
	FString filePath = "Sound/BackgroundSound.wav";

	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return nullptr;

	TArray < uint8 > rawFile;

	FFileHelper::LoadFileToArray(rawFile, filePath.GetCharArray().GetData());
	FWaveModInfo WaveInfo;

	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num()))
	{
		sw->InvalidateCompressedData();

		sw->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = sw->RawData.Realloc(rawFile.Num());
		FMemory::Memcpy(LockedData, rawFile.GetData(), rawFile.Num());
		sw->RawData.Unlock();

		int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
		if (DurationDiv)
		{
			sw->Duration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
		}
		else
		{
			sw->Duration = 0.0f;
		}
		sw->SampleRate = *WaveInfo.pSamplesPerSec;
		sw->NumChannels = *WaveInfo.pChannels;
		sw->RawPCMDataSize = WaveInfo.SampleDataSize;
		sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	}
	else {
		return nullptr;
	}

	return sw;
}

