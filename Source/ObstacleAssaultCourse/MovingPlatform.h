// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class OBSTACLEASSAULTCOURSE_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	//Platform speed in cm per second
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector PlatformSpeed = FVector(0,0,0);
	
	//Max Distance in cm
	UPROPERTY(EditAnywhere, Category = "Movement")
	int MaxDistance = 200;

	//Current distance traveled
	int CurrentDistance = 0;
	//Saving the start location
	FVector StartLocation;

	/**	check if you want the platform to travel the distance in both directions from the starting point
	 * uncheck if you want the platform to travel the distance and then return to the start point
	*/
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool IsAmbiDirectional = false;

	UPROPERTY(EditAnywhere, Category = "ActivateMovement")
	bool IsActive = true;

	void MovePlatform(float DeltaTime);

	bool DistanceTraveled = false;

	//code for rotating the actor

	UPROPERTY(EditAnywhere,Category = "Rotation")
	bool CanRotate = false;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationSpeed = FRotator(0,0,0);

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator MaxAngle = FRotator(90,90,90);

	//Only returns to initial rotation after the distance was traveled.
	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool ReturnAfterDistanceTraveled = false;

	FRotator StartRotation;

	void RotatePlatform(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool SmoothRotStart;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool SmoothRotEnd;


};
