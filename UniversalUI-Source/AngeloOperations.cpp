//  AngeloOperations.cpp    first written by sebhall in March 2023

#include <UniversalUI/Angelo/AngeloOperations.h>

#include <UniversalUI/Angelo/aRenderOperation.h>
#include <UniversalUI/Core/CoreGeometry.h>
#include <UniversalUI/Core/CoreColour.h>

//  set the drawing colour for subsequent operations
aRenderOperation UUI AngeloColour(uColour colour) {
    aRenderOperation operation;
    operation.code = 2;
    operation.data = { colour.r, colour.g, colour.b, colour.a };
    return operation;
}

//  set the drawing weight / stroke thickness for subsequent operations
aRenderOperation UUI AngeloWeight(float weight) {
    aRenderOperation operation;
    operation.code = 3;
    operation.data = { weight };
    return operation;
}

//  draw a line between first and second points
aRenderOperation UUI AngeloLine(uPoint first, uPoint second) {
    aRenderOperation operation;
    operation.code = 10;
    operation.data = { first.x, first.y, second.x, second.y };
    return operation;
}

//  draw a circle with centre and radius
aRenderOperation UUI AngeloCircle(uPoint centre, float radius) {
    aRenderOperation operation;
    operation.code = 11;
    operation.data = { centre.x, centre.y, radius };
    return operation;
}

//  draw an arc with centre and radius, starting angle and duration of angle
aRenderOperation UUI AngeloArc(uPoint centre, float radius, float startAngle, float duration) {
    aRenderOperation operation;
    operation.code = 12;
    operation.data = { centre.x, centre.y, radius, startAngle, duration };
    return operation;
}

//  draw an aligned rectangle between two points
aRenderOperation UUI AngeloRectangle(uPoint first, uPoint second) {
    aRenderOperation operation;
    operation.code = 13;
    operation.data = { first.x, first.y, second.x, second.y };
    return operation;
}

//  draw an aligned rectangle between two points and with corner radius
aRenderOperation UUI AngeloRoundedRectangle(uPoint first, uPoint second, float cornerRadius) {
    aRenderOperation operation;
    operation.code = 14;
    operation.data = { first.x, first.y, second.x, second.y, cornerRadius };
    return operation;
}