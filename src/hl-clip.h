#ifndef __HL_CLIP_H_
#define __HL_CLIP_H_
#pragma once

#include <clipper2/clipper.h>
#include <polypartition.h>
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

struct PartitionPolyIt {
    public:
    PartitionPolyIt(TPPLPolyList *list) {
        this->list = list;
        it = list->begin();
    }
    
    TPPLPolyList::iterator it;
    TPPLPolyList *list;
    inline bool IsValid() {
        return it != list->end();
    }
    inline bool Next() {
        it++;
        return it != list->end();
    }
    inline TPPLPoly &Get() {
        return *it;
    }

      inline int GetNumPoints() {
        return Get().GetNumPoints();
    }

      inline void GetPoints( double *points) {
        auto &poly = Get();
        auto count =  poly.GetNumPoints();
        for (int i = 0; i < count; i++) {
            auto &pt = poly.GetPoint(i);
            points[i * 2] = pt.x;
            points[i * 2 + 1] = pt.y;
        }
    }
};


class HLPartition {
public:
    static inline void InitD(TPPLPoly *poly, double *coordinates, int vertCount) {
       poly->Init(vertCount);
        for (int i = 0; i < vertCount; i++) {
            TPPLPoint &pt = poly->GetPoint(i);
            pt.x = coordinates[i * 2];
            pt.y = coordinates[i * 2 + 1];
        }
    }

    static inline bool ConvexPartitionOptimal(TPPLPoly *poly, TPPLPolyList *result) {
          TPPLPartition pp;

        return pp.ConvexPartition_OPT(poly, result) != 0;

    }

    static inline int NumPolys(TPPLPolyList *list) {
        return list->size();
    }


    static inline PartitionPolyIt *GetPolyIt(TPPLPolyList *list) {
        auto *x = new PartitionPolyIt (list);
        return x;
    }


};


#endif
