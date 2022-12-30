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

    static PathsD * unionAll( PathsD *paths, bool fillHoles ) {
        auto *x = new PathsD();

        if (fillHoles) {
            PolyTreeD result;
            ClipperD clipper(2);
            clipper.AddSubject(*paths);
            clipper.Execute(ClipType::Union, FillRule::NonZero, result);

            for (auto child = result.begin(); child != result.end(); child++) {
                auto &c = *(child->get());

                if (c.IsHole())
                    continue;

                x->push_back(c.Polygon());
            }
        } else {
            *x = Union(*paths, FillRule::NonZero, 2); // centimeters        
        }
        
        return x;
    }

    static int polygonCounts(PathsD *paths, int *counts) {
        auto count = 0;
        for (int i = 0; i < paths->size(); i++) {
            auto x = (*paths)[i].size();
            counts[i] = x;
            count += x;
        }

        return count;
    }
    static void getAllCoordinates(PathsD *paths, double *counts) {
        auto count = 0;
        for (int i = 0; i < paths->size(); i++) {
            auto &x = (*paths)[i];
            for (int j = 0; j < x.size(); j++) {
                counts[count++] = x[j].x;
                counts[count++] = x[j].y;
            }
        }

    }



};

#endif
