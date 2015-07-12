/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#ifndef TRINITY_CELL_H
#define TRINITY_CELL_H

#include <cmath>

#include "TypeContainer.h"
#include "TypeContainerVisitor.h"

#include "GridDefines.h"

class Map;
class WorldObject;

enum District
{
    UPPER_DISTRICT = 1,
    LOWER_DISTRICT = 1 << 1,
    LEFT_DISTRICT  = 1 << 2,
    RIGHT_DISTRICT = 1 << 3,
    CENTER_DISTRICT = 1 << 4,
    UPPER_LEFT_DISTRICT = (UPPER_DISTRICT | LEFT_DISTRICT),
    UPPER_RIGHT_DISTRICT = (UPPER_DISTRICT | RIGHT_DISTRICT),
    LOWER_LEFT_DISTRICT = (LOWER_DISTRICT | LEFT_DISTRICT),
    LOWER_RIGHT_DISTRICT = (LOWER_DISTRICT | RIGHT_DISTRICT),
    ALL_DISTRICT = (UPPER_DISTRICT | LOWER_DISTRICT | LEFT_DISTRICT | RIGHT_DISTRICT | CENTER_DISTRICT)
};

struct CellArea
{
    CellArea() : right_offset(0), left_offset(0), upper_offset(0), lower_offset(0) { }
    CellArea(int right, int left, int upper, int lower) : right_offset(right), left_offset(left), upper_offset(upper), lower_offset(lower) { }
    bool operator!() const { return !right_offset && !left_offset && !upper_offset && !lower_offset; }

    void ResizeBorders(CellPair& begin_cell, CellPair& end_cell) const
    {
        begin_cell << left_offset;
        begin_cell -= lower_offset;
        end_cell >> right_offset;
        end_cell += upper_offset;
    }

    int right_offset;
    int left_offset;
    int upper_offset;
    int lower_offset;
};

struct Cell
{
    Cell() { data.All = 0; }
    Cell(const Cell &cell) { data.All = cell.data.All; }
    explicit Cell(CellPair const& p);

    void operator|=(Cell &cell)
    {
        data.Part.reserved = 0;
        cell.data.Part.reserved = 0;
        uint32 x, y, old_x, old_y;
        Compute(x, y);
        cell.Compute(old_x, old_y);

        if(std::abs(int(x-old_x)) > 1 || std::abs(int(y-old_y)) > 1)
        {
            data.Part.reserved = ALL_DISTRICT;
            cell.data.Part.reserved = ALL_DISTRICT;
            return;
        }

        if(x < old_x)
        {
            data.Part.reserved |= LEFT_DISTRICT;
            cell.data.Part.reserved |= RIGHT_DISTRICT;
        }
        else if(old_x < x)
        {
            data.Part.reserved |= RIGHT_DISTRICT;
            cell.data.Part.reserved |= LEFT_DISTRICT;
        }
        if(y < old_y)
        {
            data.Part.reserved |= UPPER_DISTRICT;
            cell.data.Part.reserved |= LOWER_DISTRICT;
        }
        else if(old_y < y)
        {
            data.Part.reserved |= LOWER_DISTRICT;
            cell.data.Part.reserved |= UPPER_DISTRICT;
        }
    }

    void Compute(uint32 &x, uint32 &y) const
    {
        x = data.Part.grid_x*MAX_NUMBER_OF_CELLS + data.Part.cell_x;
        y = data.Part.grid_y*MAX_NUMBER_OF_CELLS + data.Part.cell_y;
    }

    bool DiffCell(const Cell &cell) const
    {
        return(data.Part.cell_x != cell.data.Part.cell_x ||
            data.Part.cell_y != cell.data.Part.cell_y);
    }

    bool DiffGrid(const Cell &cell) const
    {
        return(data.Part.grid_x != cell.data.Part.grid_x ||
            data.Part.grid_y != cell.data.Part.grid_y);
    }

    uint32 CellX() const { return data.Part.cell_x; }
    uint32 CellY() const { return data.Part.cell_y; }
    uint32 GridX() const { return data.Part.grid_x; }
    uint32 GridY() const { return data.Part.grid_y; }
    bool NoCreate() const { return data.Part.nocreate; }
    void SetNoCreate() { data.Part.nocreate = 1; }

    CellPair cellPair() const
    {
        return CellPair(
            data.Part.grid_x*MAX_NUMBER_OF_CELLS+data.Part.cell_x,
            data.Part.grid_y*MAX_NUMBER_OF_CELLS+data.Part.cell_y);
    }

    Cell& operator=(const Cell &cell)
    {
        this->data.All = cell.data.All;
        return *this;
    }

    bool operator == (const Cell &cell) const { return (data.All == cell.data.All); }
    bool operator != (const Cell &cell) const { return !operator == (cell); }
    union
    {
        struct
        {
            unsigned grid_x : 6;
            unsigned grid_y : 6;
            unsigned cell_x : 6;
            unsigned cell_y : 6;
            unsigned nocreate : 1;
            unsigned reserved : 7;
        } Part;
        uint32 All;
    } data;

    template<class T, class CONTAINER> void Visit(const CellPair&, TypeContainerVisitor<T, CONTAINER> &visitor, Map &) const;
    template<class T, class CONTAINER> void Visit(const CellPair&, TypeContainerVisitor<T, CONTAINER> &visitor, Map &, const WorldObject&, float) const;
    template<class T, class CONTAINER> void Visit(const CellPair&, TypeContainerVisitor<T, CONTAINER> &visitor, Map &, float, float, float) const;

    static CellArea CalculateCellArea(const WorldObject &obj, float radius);
    static CellArea CalculateCellArea(float x, float y, float radius);

private:
    template<class T, class CONTAINER> void VisitCircle(TypeContainerVisitor<T, CONTAINER> &, Map &, const CellPair&, const CellPair&) const;
};
#endif