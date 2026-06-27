#pragma once


#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(),Location, 30.f, 24, FColor::Blue, true);
#define DRAW_SPHERE_COLOR(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(),Location, 30.f, 24, Color, false, 5.f);
#define DRAW_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(),Location, 30.f, 24, FColor::Blue, false, -1.f );
#define DRAW_LINE(Location, Forward) if (GetWorld()) DrawDebugLine(GetWorld(), Location, Forward, FColor::Cyan, true, -1.f, 0, 1.f);
#define DRAW_LINE_SingleFrame(Location, Forward) if (GetWorld()) DrawDebugLine(GetWorld(), Location, Forward, FColor::Cyan, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(),Location, 15.f, FColor::Green, true);
#define DRAW_POINT_SingleFrame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(),Location, 15.f, FColor::Green, false, -1.f);
#define DRAW_VECTOR(Location, EndLocation) if (GetWorld()) \
{ \
if (GetWorld()) DrawDebugLine(GetWorld(), Location, EndLocation, FColor::Cyan, true, -1.f, 0, 1.f); \
if (GetWorld()) DrawDebugPoint(GetWorld(),EndLocation, 15.f, FColor::Green, true); \
}

#define DRAW_VECTOR_SingleFrame(Location, EndLocation) if (GetWorld()) \
{ \
	if (GetWorld()) DrawDebugLine(GetWorld(), Location, EndLocation, FColor::Cyan, false, -1.f, 0, 1.f); \
	if (GetWorld()) DrawDebugPoint(GetWorld(),EndLocation, 15.f, FColor::Green, false, -1.f); \
}
