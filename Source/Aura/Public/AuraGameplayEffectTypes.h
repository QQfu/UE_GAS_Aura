#pragma once
#include "GameplayEffectTypes.h"
#include "AuraGameplayEffectTypes.generated.h"

USTRUCT()
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockedHit() const {return bIsBlockedHit;}

	void SetIsCriticalHit(const bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetIsBlockedHit(const bool bInIsBlockedHit) {bIsBlockedHit = bInIsBlockedHit;}

protected:
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	bool bIsBlockedHit = false;

private:
};