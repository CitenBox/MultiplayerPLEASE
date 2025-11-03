#pragma once

#define PRINT(DebugMessage, ...) {GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, FString::Printf(TEXT(DebugMessage), __VA_ARGS__));}