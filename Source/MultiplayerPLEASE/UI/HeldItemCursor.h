#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "HeldItemCursor.generated.h"

UCLASS()
class MULTIPLAYERPLEASE_API UHeldItemCursor : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	APlayerController* PlayerController;
	
public:

	UPROPERTY(EditAnywhere, meta= (BindWidget))
	UTextBlock* ItemText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* ItemImage;

	void CustomInitialize(UTexture2D* Texture, int32 CurrentStack);
	
};
