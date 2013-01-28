#include "tracker.h"
#include "stdio.h"
#include "maxima.h"
#include "localmaximum.h"
#include "include/opencv2/opencv.hpp"
#include <math.h>

// Resolution of DVS
#define DVS_RES 128

// Parameteres
#define SIGMA_W 30.0f // sigma for perdiod weighting in Hz
#define MEASURING_PERIOD 0.01f // measurement interval to measure signal period in seconds

// Guassian smoothing filter
#define FILTER_SIZE 3   // kernel size
#define SIGMA_FILTER 0.75f

// Maximum detection
#define MAX_MIN_DIST 15.0f   // min distance between maxima
#define NUM_MAXIMA 3

// Particle filter parameters
#define PF_NUM_PARTICLES 8
#define PF_DEFAULT_SIGMA 2.0f
#define PF_MIN_MERGE_DISTANCE 4.0f // used to override merging threshold if uncertainty lower than this value
#define PF_MAX_SIGMA 8.0f
#define PF_V_MAX 200.0f  // pixels/s

// Combination analysis
#define CA_MIN_DIST 8.0f
#define CA_NUM_HYPOTHESIS 4


//! Constructor
/*!
    \param buffer Instance of PacketBuffer
    \param frequencies The target frequencies to search for. Duty cycle must be 50%.
    \param objectPoints The feature points of the object to track.
    \param cameraMatrix The intrinsic camera parameters.
    \param distortionCoefficients Distortion coefficients for the camera.
*/
Tracker::Tracker(PacketBuffer *buffer, std::vector<int> frequencies,
                 cv::Mat objectPoints, cv::Mat cameraMatrix,
                 cv::Mat distortionCoefficients, QObject *parent) : QThread(parent){
    //Members
    packetBuffer = buffer;
    targetFrequencies = frequencies;

    widget = 0;

    exit = false;

    // init maps
    latestEvents = new Map<Event>(DVS_RES,DVS_RES);
    npTransitions = new Map<Transition>(DVS_RES,DVS_RES);
    pnTransitions = new Map<Transition>(DVS_RES,DVS_RES);

    weightBuffers = new FrequencyAccumulator*[targetFrequencies.size()];
    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        weightBuffers[i] = new FrequencyAccumulator(
                    targetFrequencies[i],MEASURING_PERIOD,SIGMA_W,FILTER_SIZE,SIGMA_FILTER,MAX_MIN_DIST,NUM_MAXIMA,DVS_RES,DVS_RES);
    }

    particleFilters = new ParticleFilter*[targetFrequencies.size()];
    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        particleFilters[i] = new ParticleFilter(PF_NUM_PARTICLES,PF_DEFAULT_SIGMA,PF_MAX_SIGMA,PF_MIN_MERGE_DISTANCE,PF_V_MAX);
    }

    logger = new HypothesisLogger("C:/Users/giselher/Documents/uzh/hypo_log.txt");
    poseLogger = new PoseLogger("C:/Users/giselher/Documents/uzh/Master\ Projekt/code/dvs_tracking/MATLAB/pose_log.txt");
    lastEventTs = 0;
    eventCount = 0;

    combinationAnalyzer = new CombinationAnalyzer(particleFilters,targetFrequencies.size(),CA_MIN_DIST,CA_NUM_HYPOTHESIS);
    poseEstimator = new PoseEstimation(objectPoints,cameraMatrix,distortionCoefficients);
}

Tracker::~Tracker(){
    delete latestEvents;
    delete npTransitions;
    delete pnTransitions;

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        delete weightBuffers[i];
    }
    delete [] weightBuffers;

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        delete particleFilters[i];
    }
    delete [] particleFilters;
    delete poseEstimator;

    delete logger;
    delete poseLogger;
}

//! Processes all incoming events from the DVS.
/*!
    This method is responsible for all the event related processing. It measures the interspike intervals per pixel,
weights them according to their closeness to the targetfrequency and applies a particle filter on the most likely frequency detections.
    \param e DVS event.
*/
void Tracker::processEvent(Event e){
    // Record, if there is a transition
    Transition t = getTransition(e);
    if(t.timeStamp == 0){
        return;
    }

    // Get interval to last transition
    Interval dt = getInterval(t);
    if(dt.timeStamp == 0 || dt.deltaT < 0){
        return;
    }

    //stop logger/saving image output (for udp interface, since jAER loops the videos)
    if(lastEventTs > e.timeStamp && !poseLogger->done()){
        printf("#Events(tra): %d\n",eventCount);
        eventCount = 0;
        //logger->stop();
        poseLogger->stop();
        //widget->stopSaving();
    }
    else
        eventCount++;

    //Calculate importance of interval for each frequency
    for(unsigned int i = 0; i < targetFrequencies.size(); i++){
        FrequencyAccumulator *buf = weightBuffers[i];
        buf->update(dt);
        if(buf->hasExpired()){
            Maxima *maxima = buf->getMaxima();
            //process maxima here
            //for(int j = 0; j < targetFrequencies.size();j++){
            ParticleFilter *pf = particleFilters[i];
            pf->update(maxima,e.timeStamp);

            //            widget->updateMaxWeightParticle(i,pf->getMinUncertaintyParticle());
            //}

            //            if(!logger->done()){
            //                for(int j = 0; j < maxima->size();j++)
            //                    logger->log(e.timeStamp,targetFrequencies[i],maxima->size(),
            //                                j,maxima->get(j)->x,maxima->get(j)->y,maxima->get(j)->weight);
            //            }

            //updateWeightWidget(i,buf,maxima);
            buf->reset();
        }
    }
    lastEventTs = e.timeStamp;
}

//! Processing related to the whole packet.
/*!
    As some of the more costly processing is not needed per event, it is done per packet.
*/
void Tracker::processPacket(){
    //find possible combinations of LED positions
    combinationAnalyzer->evaluate();
    CombinationChoice *best = combinationAnalyzer->getBestHypothesis();

    //udpate widget with best score
    if(best != 0){
        for(int i = 0; i < best->size();i++){
            Particle *p = particleFilters[i]->get(best->get(i));
            widget->updateMaxWeightParticle(i,p);
        }
    }
    else{
        for(unsigned int i = 0; i < targetFrequencies.size();i++){
            widget->updateMaxWeightParticle(i,0);
        }
    }

    // Get pose estimation
    if(best != 0){
        cv::Mat imagePoints(best->size(),2,CV_32F);
        for( int i = 0; i < best->size();i++){
            Particle *p = particleFilters[i]->get(best->get(i));
            imagePoints.at<float>(i,0) = p->x;
            imagePoints.at<float>(i,1) = p->y;
        }
        imagePoints = imagePoints.reshape(2);
        poseEstimator->estimatePose(imagePoints);
        cv::Mat rvec = poseEstimator->getRotationVector();
        cv::Mat tvec = poseEstimator->getTranslationVector();

        double x,y,z,rx,ry,rz;

        x = tvec.at<double>(0,0);
        y = tvec.at<double>(1,0);
        z = tvec.at<double>(2,0);

        rx = rvec.at<double>(0,0);
        ry = rvec.at<double>(1,0);
        rz = rvec.at<double>(2,0);

        printf("[x y z (cm)]: %3.0f %3.0f %3.0f\t [rx ry rz]: %3.1f %3.1f %3.1f     \r",x*100,y*100,z*100,rx,ry,rz);

        poseLogger->log(x,y,z,rx,ry,rz,
                        imagePoints.at<float>(0,0), imagePoints.at<float>(0,1),
                        imagePoints.at<float>(1,0), imagePoints.at<float>(1,1),
                        imagePoints.at<float>(2,0), imagePoints.at<float>(2,1),
                        imagePoints.at<float>(3,0), imagePoints.at<float>(3,1),
                        lastEventTs);
    }
    else{
        //poseLogger->logTrackLost();
    }

    combinationAnalyzer->reset();
}

//! Returns a Transition
/*!
    Whenver a consecutive event with different polarity happens on the same pixel a transition is generated.
    \param e DVS event.
    \return Transition, denoting the time, place and kind of polarity change.
*/
Transition Tracker::getTransition(Event e){
    //Get last event at same position and overwrite with new event.
    Event last = latestEvents->get(e.x,e.y);
    latestEvents->insert(e.x,e.y,e);
    if(last.timeStamp == 0)
        return Transition(0);
    //If consecutive events differ in type, create and return a transition
    if(last.type != e.type)
        return Transition(e.timeStamp,e.x,e.y,e.type);
    else
        return Transition(0);
}

//! Returns a per pixel time interval per pixel
/*!
    The signal period is measured as the time interval between two consecutive transitions of the same type per pixel.
    \param t Transition
    \return Interval
*/
Interval Tracker::getInterval(Transition t){
    Map<Transition> *transitions = (t.type == 1) ? npTransitions : pnTransitions;
    Transition last = transitions->get(t.x,t.y);
    transitions->insert(t.x,t.y,t);

    if(last.timeStamp == 0)
        return Interval(0);
    else{
        double deltaT = t.timeStamp - last.timeStamp;
        return Interval(t.timeStamp,t.x,t.y,deltaT);
    }
}

//! Stop the thread.
void Tracker::stop(){
    exit = true;
}

//! Set a reference to CamWidget for graphical output.
void Tracker::setWidget(CamWidget *camWidget){
    widget = camWidget;
}

//! Update the image with an event
void Tracker::updateCamWidget(Event *e){
    if(widget == 0)
        return;
    widget->updateImage(e);
}

//! Update weight display on the widget
void Tracker::updateWeightWidget(int bufID, FrequencyAccumulator *buf, Maxima *m){
    for(int y = 0; y < DVS_RES;y++){
        for(int x = 0; x < DVS_RES;x++){
            float value = buf->weightMap->get(x,y);
            if(value > 0){
                int grey = int(value*100000);
                if(grey > 255)
                    grey = 255;
                if(grey > 0){
                    widget->updateImage(x,y,grey);
                }
            }
        }
    }
}

void Tracker::run(){
    while(!exit){
        if(packetBuffer->hasNewData()){
            EventPacket *packet = 0;
            while((packet = packetBuffer->getNextReadable()) != 0){
                for(int i = 0; i < packet->size(); i++){
                    // do not process if special event
                    Event *e = packet->get(i);
                    if(e->isSpecial())
                        return;

                    //process events here
                    //updateCamWidget(e);
//                    e->timeStamp += 1000;
                    processEvent(*e);
                }
                processPacket();
            }
        }
        else
            msleep(1);
    }
}
