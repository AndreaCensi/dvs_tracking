#include "particlefilter.h"
#include "math.h"

ParticleFilter::ParticleFilter(int numParticles, float defaultSigma, float maxSigma, float maxVelocity)
{
    size = numParticles;
    particles = new Particle[size];
    sigma_0 = defaultSigma; //default sigma for particles
    vMax = maxVelocity; // for motion model
    maxUncertainty = maxSigma;
}

ParticleFilter::~ParticleFilter(){
    delete [] particles;
}

void ParticleFilter::update(Maxima *maxima, int ts){
    for(int i = 0; i < maxima->size(); i++){
        LocalMaximum *lm = maxima->get(i);
        Particle candidate(lm->x,lm->y,sigma_0,lm->weight,ts);
        updateParticles(&candidate);
    }
}

void ParticleFilter::updateParticles(Particle *c){
    Particle *p = 0;
    bool merged = false;
    Particle *oldest = &particles[0];

    for(int i = 0; i < size; i++){
        p = &particles[i];
        // update uncertainty
        updateUncertainty(p,c->timeStamp);
        // has particle expired ( uncertainty grown too much?)
        if(hasExpired(p)){
            p->timeStamp = 0;
        }

        //search for oldest particle in array to be replaced with new one
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
    float uncertainty = p->uncertainty + float(vMax*dt);
    p->uncertainty = uncertainty;
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
    float w;
    float varP,varC,iVarP, iVarC; // variances of particle and candidate

    varP = p->uncertainty*p->uncertainty;
    varC = c->uncertainty*c->uncertainty;
    iVarP = 1/varP;
    iVarC = 1/varC;

    x = (p->x*iVarP + c->x*iVarC)/(iVarP+iVarC);
    y = (p->y*iVarP + c->y*iVarC)/(iVarP+iVarC);
    sigma = sqrt( (varP*varC)/(varP+varC) );

    float d = fabs(p->x - c->x);
    float u = fabs(p->x - x);

    w = p->weight*(1-u/d) + c->weight*(u/d);

    //set updated particle values
    p->set(x,y,sigma,w,c->timeStamp);
}
