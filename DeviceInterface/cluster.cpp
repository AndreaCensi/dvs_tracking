#include "cluster.h"
#include <math.h>
#include <algorithm>

#define NUM_EVENTS 128

// cluster maintanance
#define MAX_AGE_MOMENT 5000 //5000 usec
#define MAX_AGE_ACTIVITY 10000 //usec

// Activity measurements
#define NUM_TIMESLOTS 100
#define TIME_WINDOW 1000 //usec
#define MIN_SUM 70  // preventing noise from staying a cluster

// Transition history sampling
#define TRANSITION_WINDOW 200 //usec

// Moments - Position and contour
#define NUM_MOMENTS 100
#define MOMENT_RESOLUTION 100

//Path
#define PATH_LENGTH 128
#define MIN_PATH_INTERVAL 20000
#define MAX_PATH_DISTANCE 4 //squared distance


#define PI 3.14159265f

Cluster::Cluster(){
    posX = -1;
    posY = -1;
    currentState = 0;
    firstEventTS = 0;
    lastEventTS = 0;
    lastUpdateTS = 0;
    candidate = true;
    transitionHistory = 0;
    assigned = false;

    events = new RingBuffer<Event>(NUM_EVENTS);
    eventsPerInterval = new RingBuffer<int>(NUM_TIMESLOTS);
    moments = new RingBuffer<Moment>(NUM_MOMENTS);
    path = new RingBuffer<Position>(PATH_LENGTH);

    Event e;
    for(int i = 0; i < events->size; i++){
        events->set(i,e);
    }
    for(int i = 0; i < NUM_TIMESLOTS; i++){
        eventsPerInterval->set(i,0);
    }

    Moment m;
    for(int i = 0; i < NUM_MOMENTS; i++){
        moments->set(i,m);
    }

    Position p;
    for(int i = 0; i < PATH_LENGTH; i++){
        path->set(i,p);
    }
}

Cluster::~Cluster(){
    delete events;
    delete eventsPerInterval;
    delete moments;
    delete path;
    if(transitionHistory)
        delete transitionHistory;
}

void Cluster::addEvent(Event e){
    // add to activity history (eventsPerInterval)
    int lastEventIndex = (lastEventTS/TIME_WINDOW)%NUM_TIMESLOTS;
    int index = (e.timeStamp/TIME_WINDOW)%NUM_TIMESLOTS;
    if(lastEventIndex == index){    // if belonging to same interval, add up
        int sum = eventsPerInterval->at(index);
        eventsPerInterval->set(index,sum+1);
    }
    else{   //else reset to 0 + 1
        eventsPerInterval->set(index,1);
    }

    //add event to cluster
    events->add(e);
    if(firstEventTS == 0)
        firstEventTS = e.timeStamp;
    lifeTime = e.timeStamp - firstEventTS;

    //extract current central moment
    extractMoments(&e);

    //update the cluster path and velocity
    updatePath(e.timeStamp);
    //updateVelocity();

    lastEventTS = e.timeStamp;
}

void Cluster::extractMoments(Event *e){
    int lastEventIndex = (lastEventTS/MOMENT_RESOLUTION)%NUM_MOMENTS;   //last event assigned to cluster
    int currentIndex = (e->timeStamp/MOMENT_RESOLUTION)%NUM_MOMENTS;

    //reset timeslots if expired
    if(lastEventIndex!=currentIndex){
        int numSlots = 0;   //number of slots to overwrite
        if(lastEventIndex > currentIndex)
            numSlots = NUM_TIMESLOTS - lastEventIndex + currentIndex;
        else{
            numSlots = currentIndex - lastEventIndex;
        }
        int index = lastEventIndex + 1;
        for(int i = 0; i < numSlots;i++){
            if(index > NUM_TIMESLOTS - 1)
                index = 0;
            //            moments->at(index).reset();

            Moment m;
            m.m10 = 0;
            m.m01 = 0;
            m.m00 = 0;
            m.m20 = 0;
            m.m02 = 0;

            moments->set(index,m);

            index++;
        }
    }

    //add event
    Moment m;
    m.m10 = moments->at(currentIndex).m10 + e->posX;
    m.m01 = moments->at(currentIndex).m01 + e->posY;
    m.m00 = moments->at(currentIndex).m00 + 1;
    m.m20 = moments->at(currentIndex).m20 + pow(float(e->posX),2);
    m.m02 = moments->at(currentIndex).m02 + pow(float(e->posY),2);

    moments->set(currentIndex,m);

    int  m00,m10,m01;
    m00 = m10 = m01 = 0;
    float m20,m02;
    m20 = m02 = 0;

    for(int i = 0; i < NUM_MOMENTS;i++){
        m10 += moments->at(i).m10;
        m01 += moments->at(i).m01;
        m20 += moments->at(i).m20;
        m02 += moments->at(i).m02;
        m00 += moments->at(i).m00;
    }

    posX = float(m10)/float(m00);
    posY = float(m01)/float(m00);

    //contour -------------------------------- DRAW!
    float varianceX = m20 - posX*m10;
    float varianceY = m02 - posY*m01;
    float sqrtM00 = sqrt(float(m00));

    float tmp = sqrt(varianceX)/sqrtM00 * 2;
    contourX = (1 > tmp) ? 1 : tmp;

    tmp = sqrt(varianceY)/sqrtM00 * 2;
    contourY = (1 > tmp) ? 1 : tmp;
}

float Cluster::getActivity(){
    int sum = 0;
    for(int i = 0; i < eventsPerInterval->size; i++){
        sum += eventsPerInterval->at(i);
    }
    if (sum < MIN_SUM)
        return 0;
    else{
        float activity = sum/(contourX*contourY*PI);
        return activity;
    }
}

void Cluster::update(int ts){
    // update lifetime
    lifeTime = ts - firstEventTS;

    //update activity timewindow
    int lastEventIndex = (lastEventTS/TIME_WINDOW)%NUM_TIMESLOTS;   //last event assigned to cluster
    int tsIndex = (ts/TIME_WINDOW)%NUM_TIMESLOTS;
    if(lastEventIndex!=tsIndex){
        int numSlots = 0;   //number of slots to overwrite
        if(lastEventIndex > tsIndex)
            numSlots = NUM_TIMESLOTS - lastEventIndex + tsIndex;
        else{
            numSlots = tsIndex - lastEventIndex;
        }
        int index = lastEventIndex + 1;
        for(int i = 0; i < numSlots;i++){
            if(index > NUM_TIMESLOTS - 1)
                index = 0;
            eventsPerInterval->set(index,0);
            index++;
        }
    }

    // update cluster state for transitions history
    updateState(ts);

    // set last update time
    lastUpdateTS = ts;
}

bool Cluster::isCandidate(){
    return candidate;
}

void Cluster::updateState(int ts){
    if(!transitionHistory || assigned)    //return if temporal pattern has already been assigned
        return;

    if(transitionHistory->isFull()){    //if enough data is available, calculate transitions
        transitionHistory->compileTransitions();
        this->assigned = true;
        return;
    }

    int sumOff = 0;
    int sumOn = 0;

    int i = events->latestIndex();
    while((events->latest()->timeStamp - events->at(i).timeStamp) < TRANSITION_WINDOW && (sumOff + sumOn) < events->size){
        if(events->at(i).polarity == 1)
            sumOn++;
        else if(events->at(i).polarity == 0)
            sumOff++;
        i--; // go back in time through ringbuffer
        if(i < 0){
            i = events->size-1;
        }
    }

    if(sumOn > sumOff && currentState == 0){
        currentState = 1;

        Transition t;
        t.timeStamp = ts - TRANSITION_WINDOW/2;
        t.polarity = 1;
        transitionHistory->add(t);
    }
    else if( sumOff > sumOn && currentState == 1){
        currentState = 0;

        Transition t;
        t.timeStamp = ts - TRANSITION_WINDOW/2;
        t.polarity = 0;
        transitionHistory->add(t);
    }
}

void Cluster::updatePath(int ts){
    Position *p = path->latest();

    int deltaT = ts - p->timestamp;
    float deltaD = pow(float(posX-p->x),2) + pow(float(posY-p->y),2);

    if(deltaT > MIN_PATH_INTERVAL || deltaD > MAX_PATH_DISTANCE){
        Position p;
        p.x = posX;
        p.y = posY;
        p.timestamp = ts;
        path->add(p);
    }
}

void Cluster::updateVelocity(){
    //set last velocity
    Position *current = path->latest();
    int i = path->latestIndex()-1;
    if(i < 0)
        i = path->size-1;
    Position *last = &path->at(i);

    if(last->timestamp == 0)    // do not overestimate velocity if cluster new.
        return;

    int deltaT = current->timestamp - last->timestamp;
    float deltaDx = current->x-last->x;
    float deltaDy = current->y-last->y;

    velocity.x = deltaDx/float(deltaT);
    velocity.y = deltaDy/float(deltaT);
}

void Cluster::predictRelativePosition(){
    if(estimatedVelocity.x == 0.0f && estimatedVelocity.y == 0.0f)  //virgin state
        estimatedVelocity = velocity;
    else{       // else estimate new velocity from acceleration...
        if(velocity.normalizedDot(estimatedVelocity) <= -20.7){
            acceleration.reset();
        }
        else{
            // calculate new relative velocity
            float angle = 0;  //calc rotation angle to align velocity vector to x-axis
            Vector2D xAxis(1,0);
            if(velocity.norm() > 0)
                angle = acos(velocity.normalizedDot(xAxis));
            if(velocity.y > 0)
                angle = 2*PI - angle;

            Vector2D forward = estimatedVelocity.rotate(angle);
            Vector2D target = velocity.rotate(angle);

            float m = target.norm()-forward.norm();
            //filter and set acceleration magnitude...
            float a = 0;
            if(target.norm() > 0)
                a = 1-target.normalizedDot(xAxis);
            if(target.y < 0)
                a = -a;
            //filter and set acceleration angle...
        }

        //estimate new position
        float angle = 0;
        Vector2D xAxis(1,0);
        if(velocity.norm() > 0)
            angle = acos(velocity.normalizedDot(xAxis));
        if(velocity.y > 0)
            angle = 2*PI - angle;
        Vector2D forward = estimatedVelocity.rotate(angle);

        float delta = 1.0;

        float m = forward.norm() + acceleration.m * delta;
        float a = acceleration.a * delta;

        Vector2D newVelocity;
        newVelocity.x = m * cos(abs(a));
        int sgn = 0;
        if(a != 0.0f)
            sgn = a < 0 ? -1 : 1;
        newVelocity.y = m * sin(abs(a)) * sgn;

        newVelocity = newVelocity.rotate(-angle);

        // filter - synthetic velocity
        Vector2D measured;
        Vector2D predicted;
        if(measured.normalizedDot(predicted) < 0.5){
            acceleration.reset();
            estimatedVelocity = velocity;
        }

    }

    // compute new forward velocity... => jaer code: SyntheticVelocityPredictor
}

void Cluster::convert(){
    candidate = false;
    transitionHistory = new TransitionHistory();
}
