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

#ifndef DETOURNODE_H
#define DETOURNODE_H

#include "DetourNavMesh.h"

enum dtNodeFlags
{
    DT_NODE_OPEN = 0x01,
    DT_NODE_CLOSED = 0x02,
};

static const unsigned short DT_NULL_IDX = 0xffff;

struct dtNode
{
    float pos[3];                // Position of the node.
    float cost;                    // Cost from previous node to current node.
    float total;                // Cost up to the node.
    unsigned int pidx : 30;        // Index to parent node.
    unsigned int flags : 2;        // Node flags 0/open/closed.
    dtPolyRef id;                // Polygon ref the node corresponds to.
};

class dtNodePool
{
public:
    dtNodePool(int maxNodes, int hashSize);
    ~dtNodePool();
    inline void operator=(const dtNodePool&) { }
    void clear();
    dtNode* getNode(dtPolyRef id);
    dtNode* findNode(dtPolyRef id);

    inline unsigned int getNodeIdx(const dtNode* node) const
    {
        if(!node) return 0;
        return (unsigned int)(node - m_nodes)+1;
    }

    inline dtNode* getNodeAtIdx(unsigned int idx)
    {
        if(!idx) return 0;
        return &m_nodes[idx-1];
    }

    inline const dtNode* getNodeAtIdx(unsigned int idx) const
    {
        if(!idx) return 0;
        return &m_nodes[idx-1];
    }

    inline int getMemUsed() const
    {
        return sizeof(*this) +
        sizeof(dtNode)*m_maxNodes +
        sizeof(unsigned short)*m_maxNodes +
        sizeof(unsigned short)*m_hashSize;
    }

    inline int getMaxNodes() const { return m_maxNodes; }

    inline int getHashSize() const { return m_hashSize; }
    inline unsigned short getFirst(int bucket) const { return m_first[bucket]; }
    inline unsigned short getNext(int i) const { return m_next[i]; }

private:
    dtNode* m_nodes;
    unsigned short* m_first;
    unsigned short* m_next;
    const int m_maxNodes;
    const int m_hashSize;
    int m_nodeCount;
};

class dtNodeQueue
{
public:
    dtNodeQueue(int n);
    ~dtNodeQueue();
    inline void operator=(dtNodeQueue&) { }

    inline void clear()
    {
        m_size = 0;
    }

    inline dtNode* top()
    {
        return m_heap[0];
    }

    inline dtNode* pop()
    {
        dtNode* result = m_heap[0];
        m_size--;
        trickleDown(0, m_heap[m_size]);
        return result;
    }

    inline void push(dtNode* node)
    {
        m_size++;
        bubbleUp(m_size-1, node);
    }

    inline void modify(dtNode* node)
    {
        for(int i = 0; i < m_size; ++i)
        {
            if(m_heap[i] == node)
            {
                bubbleUp(i, node);
                return;
            }
        }
    }

    inline bool empty() const { return m_size == 0; }

    inline int getMemUsed() const
    {
        return sizeof(*this) +
        sizeof(dtNode*)*(m_capacity+1);
    }

    inline int getCapacity() const { return m_capacity; }

private:
    void bubbleUp(int i, dtNode* node);
    void trickleDown(int i, dtNode* node);

    dtNode** m_heap;
    const int m_capacity;
    int m_size;
};

#endif // DETOURNODE_H