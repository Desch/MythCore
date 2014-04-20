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

#ifndef TRINITY_GRIDLOADER_H
#define TRINITY_GRIDLOADER_H

/**
  @class GridLoader
  The GridLoader is working in conjuction with the Grid and responsible
  for loading and unloading object-types (one or more) when objects
  enters a grid.  Unloading is scheduled and might be canceled if
  an interested object re-enters.  GridLoader does not do the actuall
  loading and unloading but implements as a template pattern that
  delicate its loading and unloading for the actualy loader and unloader.
  GridLoader manages the grid (both local and remote).
 */

#include "Define.h"
#include "Grid.h"
#include "TypeContainerVisitor.h"

template
<
class ACTIVE_OBJECT,
class WORLD_OBJECT_TYPES,
class GRID_OBJECT_TYPES
>
class GridLoader
{
    public:
        /** Loads the grid
         */
        template<class LOADER>
            void Load(Grid<ACTIVE_OBJECT, WORLD_OBJECT_TYPES, GRID_OBJECT_TYPES> &grid, LOADER &loader)
        {
            grid.LockGrid();
            loader.Load(grid);
            grid.UnlockGrid();
        }

        /** Stop the grid
         */
        template<class STOPER>
            void Stop(Grid<ACTIVE_OBJECT, WORLD_OBJECT_TYPES, GRID_OBJECT_TYPES> &grid, STOPER &stoper)
        {
            grid.LockGrid();
            stoper.Stop(grid);
            grid.UnlockGrid();
        }
        /** Unloads the grid
         */
        template<class UNLOADER>
            void Unload(Grid<ACTIVE_OBJECT, WORLD_OBJECT_TYPES, GRID_OBJECT_TYPES> &grid, UNLOADER &unloader)
        {
            grid.LockGrid();
            unloader.Unload(grid);
            grid.UnlockGrid();
        }
};

#endif

