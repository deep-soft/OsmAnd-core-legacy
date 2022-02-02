#ifndef _OSMAND_ROUTE_SEGMENT_H
#define _OSMAND_ROUTE_SEGMENT_H
#include "CommonCollections.h"
#include "commonOsmAndCore.h"
#include "routeSegmentResult.h"
#include "Logging.h"

struct RouteSegment {
    uint16_t segmentStart;
    SHARED_PTR<RouteDataObject> road;
    // needed to store intersection of routes
    SHARED_PTR<RouteSegment> next;
    SHARED_PTR<RouteSegment>  oppositeDirection ;
    
    // search context (needed for searching route)
    // Initially it should be null (!) because it checks was it segment visited before
    std::weak_ptr<RouteSegment> parentRoute;
    uint16_t parentSegmentEnd;
    
    
    // 1 - positive , -1 - negative, 0 not assigned
    int8_t directionAssgn;
    
    // final route segment
    int8_t reverseWaySearch;
    SHARED_PTR<RouteSegment> opposite;
    
    // distance measured in time (seconds)
    float distanceFromStart;
    float distanceToEnd;
    
    inline bool isFinal() {
        return reverseWaySearch != 0;
    }
    
    inline bool isReverseWaySearch() {
        return reverseWaySearch == 1;
    }
    
    inline uint16_t getSegmentStart() {
        return segmentStart;
    }
    
    inline bool isPositive() {
        return directionAssgn == 1;
    }
    
    inline SHARED_PTR<RouteDataObject>& getRoad() {
        return road;
    }
    
    static SHARED_PTR<RouteSegment> initRouteSegment(SHARED_PTR<RouteSegment>& th, bool positiveDirection) {
        if(th->segmentStart == 0 && !positiveDirection) {
            return SHARED_PTR<RouteSegment>();
        }
        if(th->segmentStart == th->road->getPointsLength() - 1 && positiveDirection) {
            return SHARED_PTR<RouteSegment>();
        }
        SHARED_PTR<RouteSegment> rs = th;
        if(th->directionAssgn == 0) {
            rs->directionAssgn = positiveDirection ? 1 : -1;
        } else {
            if(positiveDirection != (th->directionAssgn == 1)) {
                if(th->oppositeDirection.get() == NULL) {
                    th->oppositeDirection = std::make_shared<RouteSegment>(th->road, th->segmentStart);
                    th->oppositeDirection->directionAssgn = positiveDirection ? 1 : -1;
                }
                if ((th->oppositeDirection->directionAssgn == 1) != positiveDirection) {
                    OsmAnd::LogPrintf(OsmAnd::LogSeverityLevel::Debug, "Alert failed - directionAssgn wrongly");
                }
                rs = th->oppositeDirection;
            }
        }
        return rs;
    }
    
    RouteSegment()
        : segmentStart(0)
        , road(nullptr)
        , next()
        , oppositeDirection()
        , parentRoute()
        , parentSegmentEnd(0)
        , directionAssgn(0)
        , reverseWaySearch(0)
        , opposite()
        , distanceFromStart(0)
        , distanceToEnd(0) {
    }
    
    RouteSegment(SHARED_PTR<RouteDataObject>& road, int segmentStart)
        : segmentStart(segmentStart)
        , road(road)
        , next()
        , oppositeDirection()
        , parentRoute()
        , parentSegmentEnd(0)
        , directionAssgn(0)
        , reverseWaySearch(0)
        , opposite()
        , distanceFromStart(0)
        , distanceToEnd(0) {
    }
};

struct RouteSegmentPoint : RouteSegment {
	
    RouteSegmentPoint(SHARED_PTR<RouteDataObject>& road, int segmentStart, double distSquare) : RouteSegment(road, segmentStart), dist(distSquare) {
		this->preciseX = road->pointsX[segmentStart];
		this->preciseY = road->pointsY[segmentStart];
	}

	RouteSegmentPoint(SHARED_PTR<RouteSegmentPoint>& pnt) : RouteSegment(pnt->road, pnt->segmentStart),
		/*distSquare(pnt->distSquare),*/ preciseX(pnt->preciseX), preciseY(pnt->preciseY) {}
	double dist;
	int preciseX;
	int preciseY;
	vector<SHARED_PTR<RouteSegmentPoint>> others;

	LatLon getPreciseLatLon() { return LatLon(get31LatitudeY(preciseY), get31LongitudeX(preciseX)); }
};

struct FinalRouteSegment : RouteSegment {
	FinalRouteSegment(SHARED_PTR<RouteDataObject>& road, int segmentStart) : RouteSegment(road, segmentStart) {}
	bool reverseWaySearch;
	SHARED_PTR<RouteSegment> opposite;
};

struct GpxPoint {
	int32_t ind;
	double lat;
	double lon;
	double cumDist;
	SHARED_PTR<RouteSegmentPoint> pnt;
	vector<SHARED_PTR<RouteSegmentResult>> routeToTarget;
	vector<SHARED_PTR<RouteSegmentResult>> stepBackRoute;
	int targetInd = -1;
	bool straightLine = false;

	GpxPoint(int32_t ind, double lat, double lon, double cumDist)
		: ind(ind), lat(lat), lon(lon), cumDist(cumDist){};
	
	GpxPoint(const SHARED_PTR<GpxPoint>& p)
		: ind(p->ind), lat(p->lat), lon(p->lon), cumDist(p->cumDist){};
};

#endif /*_OSMAND_ROUTE_SEGMENT_H*/
