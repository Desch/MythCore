/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#ifndef DETOUROBSTACLEAVOIDANCE_H
#define DETOUROBSTACLEAVOIDANCE_H

struct dtObstacleCircle
{
    float p[3];                // Position of the obstacle
    float vel[3];            // Velocity of the obstacle
    float dvel[3];            // Velocity of the obstacle
    float rad;                // Radius of the obstacle
    float dp[3], np[3];        // Use for side selection during sampling.
};

struct dtObstacleSegment
{
    float p[3], q[3];        // End points of the obstacle segment
    bool touch;
};

static const int RVO_SAMPLE_RAD = 15;
static const int MAX_RVO_SAMPLES = (RVO_SAMPLE_RAD*2+1)*(RVO_SAMPLE_RAD*2+1) + 100;

class dtObstacleAvoidanceDebugData
{
public:
    dtObstacleAvoidanceDebugData();
    ~dtObstacleAvoidanceDebugData();

    bool init(const int maxSamples);
    void reset();
    void addSample(const float* vel, const float ssize, const float pen,
                   const float vpen, const float vcpen, const float spen, const float tpen);

    void normalizeSamples();

    inline int getSampleCount() const { return m_nsamples; }
    inline const float* getSampleVelocity(const int i) const { return &m_vel[i*3]; }
    inline float getSampleSize(const int i) const { return m_ssize[i]; }
    inline float getSamplePenalty(const int i) const { return m_pen[i]; }
    inline float getSampleDesiredVelocityPenalty(const int i) const { return m_vpen[i]; }
    inline float getSampleCurrentVelocityPenalty(const int i) const { return m_vcpen[i]; }
    inline float getSamplePreferredSidePenalty(const int i) const { return m_spen[i]; }
    inline float getSampleCollisionTimePenalty(const int i) const { return m_tpen[i]; }

private:
    int m_nsamples;
    int m_maxSamples;
    float* m_vel;
    float* m_ssize;
    float* m_pen;
    float* m_vpen;
    float* m_vcpen;
    float* m_spen;
    float* m_tpen;
};

dtObstacleAvoidanceDebugData* dtAllocObstacleAvoidanceDebugData();
void dtFreeObstacleAvoidanceDebugData(dtObstacleAvoidanceDebugData* ptr);

class dtObstacleAvoidanceQuery
{
public:
    dtObstacleAvoidanceQuery();
    ~dtObstacleAvoidanceQuery();

    bool init(const int maxCircles, const int maxSegments);

    void reset();

    void addCircle(const float* pos, const float rad,
                   const float* vel, const float* dvel);

    void addSegment(const float* p, const float* q);

    inline void setVelocitySelectionBias(float v) { m_velBias = v; }
    inline void setDesiredVelocityWeight(float w) { m_weightDesVel = w; }
    inline void setCurrentVelocityWeight(float w) { m_weightCurVel = w; }
    inline void setPreferredSideWeight(float w) { m_weightSide = w; }
    inline void setCollisionTimeWeight(float w) { m_weightToi = w; }
    inline void setTimeHorizon(float t) { m_horizTime = t; }

    void sampleVelocityGrid(const float* pos, const float rad, const float vmax,
                            const float* vel, const float* dvel, float* nvel,
                            const int gsize,
                            dtObstacleAvoidanceDebugData* debug = 0);

    void sampleVelocityAdaptive(const float* pos, const float rad, const float vmax,
                                const float* vel, const float* dvel, float* nvel,
                                const int ndivs, const int nrings, const int depth,
                                dtObstacleAvoidanceDebugData* debug = 0);

    inline int getObstacleCircleCount() const { return m_ncircles; }
    const dtObstacleCircle* getObstacleCircle(const int i) { return &m_circles[i]; }

    inline int getObstacleSegmentCount() const { return m_nsegments; }
    const dtObstacleSegment* getObstacleSegment(const int i) { return &m_segments[i]; }

private:
    void prepare(const float* pos, const float* dvel);

    float processSample(const float* vcand, const float cs,
                        const float* pos, const float rad,
                        const float vmax, const float* vel, const float* dvel,
                        dtObstacleAvoidanceDebugData* debug);

    dtObstacleCircle* insertCircle(const float dist);
    dtObstacleSegment* insertSegment(const float dist);

    float m_velBias;
    float m_weightDesVel;
    float m_weightCurVel;
    float m_weightSide;
    float m_weightToi;
    float m_horizTime;

    int m_maxCircles;
    dtObstacleCircle* m_circles;
    int m_ncircles;

    int m_maxSegments;
    dtObstacleSegment* m_segments;
    int m_nsegments;
};

dtObstacleAvoidanceQuery* dtAllocObstacleAvoidanceQuery();
void dtFreeObstacleAvoidanceQuery(dtObstacleAvoidanceQuery* ptr);

#endif // DETOUROBSTACLEAVOIDANCE_H