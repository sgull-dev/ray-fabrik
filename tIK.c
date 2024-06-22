#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    Vector2 startPos;
    Vector2 rot;
    float len;
} IKSeg;

IKSeg* initArr(int n, Vector2 r, float len);
IKSeg* IKSeg_new(Vector2 r, float len);
void IKSeg_init(IKSeg* this, Vector2 r, float len);
void IK(Vector2 mouse, IKSeg* t, int n);

int main(void) {
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    
    const Vector2 tRoot =  {960.0f, 540.0f};
    int n = 200;

    IKSeg* tentacle = initArr(n, tRoot, 5.0f);
    //Create Window
    InitWindow(screenWidth, screenHeight, "raylib tentacle IK");
    SetTargetFPS(60);

    Vector2 laggedMousePos = GetMousePosition();

    while (!WindowShouldClose()) {
	BeginDrawing();
	    ClearBackground(BLACK);
	    //Testing for mouseposition
	    Vector2 mousePos = GetMousePosition();
	    laggedMousePos = Vector2Lerp(laggedMousePos, mousePos, 0.05f);
	    //Handle IK
	    IK(laggedMousePos, tentacle, n);
	    //Draw Tentacle
	    for (int i=0; i < n; i++) {
		Vector2 s = tentacle[i].startPos;
		Vector2 e = Vector2Add(s, Vector2Scale(tentacle[i].rot, tentacle[i].len));
		DrawLineV(s, e, RAYWHITE);
	    }
	EndDrawing();
    }
    CloseWindow();
    free(tentacle);
    return 0;
}

IKSeg* initArr(int n, Vector2 r, float len) {
    IKSeg* arr = (IKSeg*)malloc(n * sizeof(IKSeg));

    Vector2 currentStartPos = r;
    for (int i = 0; i < n; i++) {
	IKSeg* segment = IKSeg_new(currentStartPos, len);
	arr[i] = *segment;
	free(segment);
	currentStartPos.y += len;
    }
    return arr;
}

IKSeg* IKSeg_new(Vector2 r, float len) {
    IKSeg* p = (IKSeg*)malloc(sizeof(IKSeg));
    if (p != NULL) {
	IKSeg_init(p, r, len);
    }
    return p;
}

void IKSeg_init(IKSeg* this, Vector2 r, float len) {
    this->startPos = r;
    this->rot = (Vector2){0.0f, 1.0f};
    this->len = len;
}

void IK(Vector2 mouse, IKSeg* t, int n) {
    Vector2 curTarget = mouse;
    Vector2 root = t[0].startPos;
    // Forwards
    for (int i = n - 1; i >= 0; i--) {
        Vector2 vectorToTarget = Vector2Subtract(curTarget, t[i].startPos);
        Vector2 nV = Vector2Normalize(vectorToTarget);
        t[i].rot = nV;
        Vector2 vectorFromTarget = Vector2Scale(nV, t[i].len);
        t[i].startPos = Vector2Subtract(curTarget, vectorFromTarget);
        curTarget = t[i].startPos;
    }
    // Backwards
    curTarget = root;
    for (int j = 0; j < n; j++) {
	Vector2 curEndPos = Vector2Add( Vector2Scale(t[j].rot, t[j].len), t[j].startPos);
	Vector2 vectorToTarget = Vector2Subtract(curEndPos, curTarget);
	Vector2 nV = Vector2Normalize(vectorToTarget);
	t[j].rot = nV;
        t[j].startPos = curTarget;
        Vector2 endPos = Vector2Add(curTarget, Vector2Scale(t[j].rot, t[j].len));
        curTarget = endPos;
    }
}
