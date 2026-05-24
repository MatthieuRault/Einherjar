#include "AnimNotify_AttackHit.h"
#include "EinherjarCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_AttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (AEinherjarCharacter* Character = Cast<AEinherjarCharacter>(MeshComp->GetOwner()))
	{
		Character->PerformAttackTrace();
	}
}