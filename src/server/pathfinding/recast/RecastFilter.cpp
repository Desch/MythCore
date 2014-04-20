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

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "Recast.h"
#include "RecastAssert.h"

void rcFilterLowHangingWalkableObstacles(rcContext* ctx, const int walkableClimb, rcHeightfield& solid)
{
    rcAssert(ctx);

    ctx->startTimer(RC_TIMER_FILTER_LOW_OBSTACLES);

    const int w = solid.width;
    const int h = solid.height;

    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            rcSpan* ps = 0;
            bool previousWalkable = false;

            for(rcSpan* s = solid.spans[x + y*w]; s; ps = s, s = s->next)
            {
                const bool walkable = s->area != RC_NULL_AREA;
                // If current span is not walkable, but there is walkable
                // span just below it, mark the span above it walkable too.
                if(!walkable && previousWalkable)
                {
                    if(rcAbs((int)s->smax - (int)ps->smax) <= walkableClimb)
                        s->area = RC_NULL_AREA;
                }
                // Copy walkable flag so that it cannot propagate
                // past multiple non-walkable objects.
                previousWalkable = walkable;
            }
        }
    }

    ctx->stopTimer(RC_TIMER_FILTER_LOW_OBSTACLES);
}

void rcFilterLedgeSpans(rcContext* ctx, const int walkableHeight, const int walkableClimb,
                        rcHeightfield& solid)
{
    rcAssert(ctx);

    ctx->startTimer(RC_TIMER_FILTER_BORDER);

    const int w = solid.width;
    const int h = solid.height;
    const int MAX_HEIGHT = 0xffff;

    // Mark border spans.
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            for(rcSpan* s = solid.spans[x + y*w]; s; s = s->next)
            {
                // Skip non walkable spans.
                if(s->area == RC_NULL_AREA)
                    continue;

                const int bot = (int)(s->smax);
                const int top = s->next ? (int)(s->next->smin) : MAX_HEIGHT;

                // Find neighbours minimum height.
                int minh = MAX_HEIGHT;

                // Min and max height of accessible neighbours.
                int asmin = s->smax;
                int asmax = s->smax;

                for(int dir = 0; dir < 4; ++dir)
                {
                    int dx = x + rcGetDirOffsetX(dir);
                    int dy = y + rcGetDirOffsetY(dir);
                    // Skip neighbours which are out of bounds.
                    if(dx < 0 || dy < 0 || dx >= w || dy >= h)
                    {
                        minh = rcMin(minh, -walkableClimb - bot);
                        continue;
                    }

                    // From minus infinity to the first span.
                    rcSpan* ns = solid.spans[dx + dy*w];
                    int nbot = -walkableClimb;
                    int ntop = ns ? (int)ns->smin : MAX_HEIGHT;
                    // Skip neightbour if the gap between the spans is too small.
                    if(rcMin(top,ntop) - rcMax(bot,nbot) > walkableHeight)
                        minh = rcMin(minh, nbot - bot);

                    // Rest of the spans.
                    for(ns = solid.spans[dx + dy*w]; ns; ns = ns->next)
                    {
                        nbot = (int)ns->smax;
                        ntop = ns->next ? (int)ns->next->smin : MAX_HEIGHT;
                        // Skip neightbour if the gap between the spans is too small.
                        if(rcMin(top,ntop) - rcMax(bot,nbot) > walkableHeight)
                        {
                            minh = rcMin(minh, nbot - bot);

                            // Find min/max accessible neighbour height.
                            if(rcAbs(nbot - bot) <= walkableClimb)
                            {
                                if(nbot < asmin) asmin = nbot;
                                if(nbot > asmax) asmax = nbot;
                            }

                        }
                    }
                }

                // The current span is close to a ledge if the drop to any
                // neighbour span is less than the walkableClimb.
                if(minh < -walkableClimb)
                    s->area = RC_NULL_AREA;

                // If the difference between all neighbours is too large,
                // we are at steep slope, mark the span as ledge.
                if((asmax - asmin) > walkableClimb)
                {
                    s->area = RC_NULL_AREA;
                }
            }
        }
    }

    ctx->stopTimer(RC_TIMER_FILTER_BORDER);
}

void rcFilterWalkableLowHeightSpans(rcContext* ctx, int walkableHeight, rcHeightfield& solid)
{
    rcAssert(ctx);

    ctx->startTimer(RC_TIMER_FILTER_WALKABLE);

    const int w = solid.width;
    const int h = solid.height;
    const int MAX_HEIGHT = 0xffff;

    // Remove walkable flag from spans which do not have enough
    // space above them for the agent to stand there.
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            for(rcSpan* s = solid.spans[x + y*w]; s; s = s->next)
            {
                const int bot = (int)(s->smax);
                const int top = s->next ? (int)(s->next->smin) : MAX_HEIGHT;
                if((top - bot) <= walkableHeight)
                    s->area = RC_NULL_AREA;
            }
        }
    }

    ctx->stopTimer(RC_TIMER_FILTER_WALKABLE);
}
