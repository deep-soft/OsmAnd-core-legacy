#ifndef _OSMAND_TRANSPORT_ROUTE_RESULT_H
#define _OSMAND_TRANSPORT_ROUTE_RESULT_H
#include "CommonCollections.h"
#include "commonOsmAndCore.h"

struct TransportRouteResultSegment;
struct TransportRoutingConfiguration;
struct TransportRoutingContext;

struct TransportRouteResult {

    vector<SHARED_PTR<TransportRouteResultSegment>> segments;
    double finishWalkDist;
    double routeTime;
    SHARED_PTR<TransportRoutingConfiguration> config;

    TransportRouteResult(TransportRoutingContext* ctx);
    
    double getWalkDist();
    float getWalkSpeed();
    int getStops();
    //bool isRouteStop(TransportStop stop);
    double getTravelDist();
    double getTravelTime();
    double getWalkTime();
    double getChangeTime();
    double getBoardingTime();
    int getChanges();
    void to_string();
};

#endif /*_OSMAND_TRANSPORT_ROUTE_RESULT_H*/
