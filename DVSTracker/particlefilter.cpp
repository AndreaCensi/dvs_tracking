#include "particlefilter.h"
#include "math.h"
#include "stdio.h"

ParticleFilter::ParticleFilter(int numParticles, float defaultSigma, float maxSigma, float maxVelocity)
{
    length = numParticles;
    particles = new Particle[length];
    sigma_0 = defaultSigma; //default sigma for particles
    vMax = maxVelocity; // for motion model
    maxUncertainty = maxSigma;
}

ParticleFilter::~ParticleFilter(){
    delete [] particles;
}

void ParticleFilter::update(Maxima *maxima, double ts){
    Particle *p;
    for(int i = 0; i < length; i++){
        p = &particles[i];

        if(p->timeStamp == 0)
            continue;
        // update uncertainty
        updateUncertainty(p,ts);
        // has particle expired ( uncertainty grown too much?)
        if(hasExpired(p)){
            p->timeStamp = 0;
        }
    }

    for(int i = 0; i < maxima->size(); i++){
        LocalMaximum *lm = maxima->get(i);
        Particle candidate(float(lm->x),float(lm->y),sigma_0,lm->weight,ts);
        updateParticles(&candidate);
    }
}

Particle* ParticleFilter::get(int i){
    return &particles[i];
}

int ParticleFilter::size(){
    return length;
}

void ParticleFilter::updateParticles(Particle *c){
    Particle *p = 0;
    bool merged = false;
    Particle *oldest = &particles[0];

    for(int i = 0; i < length; i++){
        p = &particles[i];

        //look for oldest particle to replace
        if(oldest->timeStamp > p->timeStamp)
            oldest = p;

        //if particle reset ( timestamp == 0) continue with next particle
        if(p->timeStamp == 0)
            continue;
        //merge candidate with particle if inside covariance of particle
        if(insideCovariance(p,c)){
            merge(p,c);
            merged = true;
        }
    }
    // create new particle if unmergable
    if(!merged){
        oldest->set(c->x,c->y,c->uncertainty,c->weight,c->timeStamp);
    }
}

void ParticleFilter::updateUncertainty(Particle *p, double ts){
    double dt = ts-p->timeStamp;
    p->uncertainty += float(vMax*dt);
    p->timeStamp = ts;
}

bool ParticleFilter::hasExpired(Particle *p){
    if(p->uncertainty > maxUncertainty)
        return true;
    else
        return false;
}

bool ParticleFilter::insideCovariance(Particle *p, Particle *c){
    float distance = sqrt((pow(float(p->x-c->x),2.0f) + pow(float(p->y-c->y),2.0f)));
    if(distance < p->uncertainty)
        return true;
    else
        return false;
}

void ParticleFilter::merge(Particle *p, Particle *c){
    float x,y;
    float sigma;
    int w;
    float varP,varC,iVarP, iVarC; // variances of particle and candidate

    varP = p->uncertainty*p->uncertainty;
    varC = c->uncertainty*c->uncertainty;
    iVarP = 1/varP;
    iVarC = 1/varC;

    x = (p->x*iVarP + c->x*iVarC)/(iVarP+iVarC);
    y = (p->y*iVarP + c->y*iVarC)/(iVarP+iVarC);
    sigma = sqrt( (varP*varC)/(varP+varC) );

//    float d = fabs(p->x - c->x);
//    float u = fabs(p->x - x);
//    w = int(p->weight*(1-u/d) + c->weight*(u/d));

    //let the weight grow, the more updates come in
    w = int( (sigma_0/p->uncertainty)*p->weight + c->weight );

    //set updated particle values
    p->set(x,y,sigma,w,c->timeStamp);
}
