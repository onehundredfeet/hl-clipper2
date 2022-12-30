#ifndef __HL_CLIP_H_
#define __HL_CLIP_H_
#pragma once

#include <clipper2/clipper.h>
using namespace Clipper2Lib;

class HLClip {
public:
    static void addPolygonD(PathsD *paths, double *p, int vertCount) {
        PathD path;
        auto elemCount = vertCount * 2;
        for (int i = 0; i < elemCount; i += 2) {
            path.push_back(PointD(p[i], p[i + 1]));
        }

        paths->push_back(path);
    }

     static void addPolygonsD(PathsD *paths, double *p, int *vertCount, int polyCount) {
        for (int i = 0; i < polyCount; i++) {
            addPolygonD(paths, p, vertCount[i]);
            p += vertCount[i] * 2;
        }
    }
};

#endif
