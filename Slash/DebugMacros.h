#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 6.f, 6, FColor::Red, false, 5.f);
#define DRAW_SPHERE_SingleFrame(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 30, FColor::Red, false, -1.f);
#define DRAW_LINE(Location, ToLocation) if(GetWorld()) DrawDebugLine(GetWorld(), Location, Location + ToLocation, FColor::Red, true, 60.f); 
#define DRAW_LINE_singleFrame(Location, ToLocation) if(GetWorld()) DrawDebugLine(GetWorld(), Location, Location + ToLocation, FColor::Red, false, -1f.); 
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 12.f, FColor::Red, true, 7.f, 0);
#define DRAW_POINT_singleFrame(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 12.f, FColor::Red, false, -1.f);
#define DRAW_VECTOR(Location, ToLocation) if(GetWorld()) \
			{\
				DrawDebugLine(GetWorld(), Location, ToLocation, FColor::Red, true, 5.f, 0, 1.f); \
				DrawDebugPoint(GetWorld(), ToLocation, 12.f, FColor::Yellow, true, 5.f, 0); \
				DrawDebugPoint(GetWorld(), Location, 12.f, FColor::Yellow, true, 5.f, 0); \
			}
#define DRAW_VECTOR_singleFrame(Location, ToLocation) if(GetWorld()) \
			{\
				DrawDebugLine(GetWorld(), Location, ToLocation, FColor::Red, false, -1.f); \
				DrawDebugPoint(GetWorld(), ToLocation, 12.f, FColor::Red, false, -1.f); \
			}

#define SCREEN_MSG(Format, ...) if(GEngine)\
			{\
				GEngine->AddOnScreenDebugMessage(\
					-1,\
					15.f,\
					FColor::Cyan,\
					FString::Printf(TEXT(Format), ##__VA_ARGS__)\
					);\
			}