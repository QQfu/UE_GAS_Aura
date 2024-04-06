// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	//NetUpdateFrequency: How often (per second) this actor will be considered for replication, used to determine NetUpdateTime
	NetUpdateFrequency = 100.f;
}
