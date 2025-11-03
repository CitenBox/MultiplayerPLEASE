#include "HeldItemCursor.h"

#include "Rendering/RenderingSpatialHash.h"

void UHeldItemCursor::CustomInitialize(UTexture2D* Texture, int32 CurrentStack)
{
	ItemImage->SetBrushFromTexture(Texture);
	
	if(CurrentStack > 1)
	{
		ItemText->SetText(FText::FromString(FString::FromInt(CurrentStack)));
	}
}
