// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CanMove){
		MovePlatform(DeltaTime);
	}

	if(CanRotate){
		RotatePlatform(DeltaTime);
	}
}



void AMovingPlatform::MovePlatform(float DeltaTime){
	//declare local variable for new location using starting location

	FVector CurrentLocation = GetActorLocation();

	FVector NewLocation = CurrentLocation + (PlatformSpeed * DeltaTime); //not using pointer so it should not affect StartLocation
	
	double Distance = (NewLocation - StartLocation).Length();

	 if(Distance >= MaxDistance){
		//if the max distance has been reached, set the NewLocation to exactly where it's supposed to be to avoid creep over time.
		NewLocation = StartLocation + PlatformSpeed.GetSafeNormal() * (MaxDistance > 0 ? MaxDistance : 0);
		//if actor is not ambi directional,  set the stored start location to this end location. This way the actor will only travel the distance once more, reaching the original start location
		if(!this->IsAmbiDirectional){
			this->StartLocation = NewLocation;
		}
		DistanceTraveled = true;	//used in other functions that might wait for this distance to be traveled before doing something
		PlatformSpeed *= -1;
	 }else{
		DistanceTraveled = false;
	 }

	SetActorLocation(NewLocation);
}

void AMovingPlatform::RotatePlatform(float DeltaTime){
	FRotator CurrentRotation = GetActorRotation();

	FVector2D rotStartMultiVector = FVector2D(RotStartMulti, RotMidMulti);
	FVector2D rotEndMultiVector = FVector2D(RotMidMulti, RotEndMulti);

	float PitchAmplifA = FMath::GetMappedRangeValueClamped(FVector2D(StartRotation.Pitch, StartRotation.Pitch + MaxAngle.Pitch / 2), rotStartMultiVector, CurrentRotation.Pitch);
	float PitchAmplifB = FMath::GetMappedRangeValueClamped(FVector2D(StartRotation.Pitch + MaxAngle.Pitch / 2, StartRotation.Pitch + MaxAngle.Pitch), rotEndMultiVector, CurrentRotation.Pitch);

	float YawAmplifA = FMath::GetMappedRangeValueClamped(FVector2D(StartRotation.Yaw, StartRotation.Yaw + MaxAngle.Yaw / 2), rotStartMultiVector, CurrentRotation.Yaw);
	float YawAmplifB = FMath::GetMappedRangeValueClamped(FVector2D(StartRotation.Yaw + MaxAngle.Yaw / 2, StartRotation.Yaw + MaxAngle.Yaw), rotEndMultiVector, CurrentRotation.Yaw);

	float RollAmplifA = FMath::GetMappedRangeValueClamped(FVector2D(StartRotation.Roll, StartRotation.Roll + MaxAngle.Roll / 2), rotStartMultiVector, CurrentRotation.Roll);
	float RollAmplifB = FMath::GetMappedRangeValueClamped(FVector2D(StartRotation.Roll + MaxAngle.Roll / 2, StartRotation.Roll + MaxAngle.Roll), rotEndMultiVector, CurrentRotation.Roll);

	FRotator TempSpeed = RotationSpeed;

	if(CurrentRotation.Pitch >= StartRotation.Pitch + MaxAngle.Pitch / 2){
		TempSpeed.Pitch *= PitchAmplifB;
	}else{
		TempSpeed.Pitch *= PitchAmplifA;
	}

	if(CurrentRotation.Yaw >= StartRotation.Yaw + MaxAngle.Yaw / 2){
		TempSpeed.Yaw *= YawAmplifB;
	}else{
		TempSpeed.Yaw *= YawAmplifA;
	}

	if(CurrentRotation.Roll >= StartRotation.Roll + MaxAngle.Roll / 2){
		TempSpeed.Roll *= RollAmplifB;
	}else{
		TempSpeed.Roll *= RollAmplifA;
	}

	FRotator NewRotation = CurrentRotation + (TempSpeed * DeltaTime * 10);

	if(UseMaxAngle){
		if(NewRotation.Pitch >= StartRotation.Pitch + MaxAngle.Pitch || NewRotation.Pitch <= StartRotation.Pitch){
			NewRotation.Pitch = StartRotation.Pitch + MaxAngle.Pitch * (NewRotation.Pitch >= MaxAngle.Pitch);

			if(ReturnAfterDistanceTraveled && DistanceTraveled || !ReturnAfterDistanceTraveled){
				RotationSpeed.Pitch = -1 * RotationSpeed.Pitch;
			}
		}

		if(NewRotation.Yaw >= StartRotation.Yaw + MaxAngle.Yaw || NewRotation.Yaw <= StartRotation.Yaw){
			NewRotation.Yaw = StartRotation.Yaw + MaxAngle.Yaw * (NewRotation.Yaw >= MaxAngle.Yaw);

			if(ReturnAfterDistanceTraveled && DistanceTraveled || !ReturnAfterDistanceTraveled){
				RotationSpeed.Yaw = -1 * RotationSpeed.Yaw;
			}
		}

		if(NewRotation.Roll >= StartRotation.Roll + MaxAngle.Roll || NewRotation.Roll <= StartRotation.Roll){
			NewRotation.Roll = StartRotation.Roll + MaxAngle.Roll * (NewRotation.Roll >= MaxAngle.Roll);

			if(ReturnAfterDistanceTraveled && DistanceTraveled || !ReturnAfterDistanceTraveled){
				RotationSpeed.Roll = -1 * RotationSpeed.Roll;
			}
		}
	}

	SetActorRotation(NewRotation);
}

