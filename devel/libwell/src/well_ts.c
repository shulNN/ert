/*
   Copyright (C) 2011  Statoil ASA, Norway. 
    
   The file 'well_ts.c' is part of ERT - Ensemble based Reservoir Tool. 
    
   ERT is free software: you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation, either version 3 of the License, or 
   (at your option) any later version. 
    
   ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE.   
    
   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
   for more details. 
*/

/**
   The wells can typically change configuration during a simulation,
   new completions can be added, the well can be shut for a period, it
   can change purpose from injector to producer and so on. 

   The well_ts datastructure is used to hold the complete history of
   one well; for each new report step a new well_state object is added
   to to the well_ts structure. Afterwards you can use the well_ts
   object to query for the well_state at different times.

   An example timeline for one well can look like this:


                  well_state0    well_state1   well_state2   well_state3
       [-------------x---------------x-------------x--------------]
                   0030            0060          0070           0090
   
   The well in this example is added at report step 30; after that we
   have well_state information from each of the reported report steps
   60,70 and 90. If we query the well_ts object for well state
   information at a particular report step the well_ts structure will
   return the well_state info at the time at or immediately before the
   query time:

     o If we ask for the well state at step 30 we will get the
       well_state0 object; if we ask for the well state at step 75 we
       will get the well_state2 object.
       
     o If we ask for the well_state before the well has appeared the
       first time we will get NULL.
   
     o The restart files have no meta information of when the
       simulation ended, so there is no way to detect it if you ask
       for the well state way beyond the end of the simulation. If you
       ask for the well state at report step 100 (i.e. beyond the end
       of the simulation) you will just get the well_state3 object.

   The time direction can be specified by both report step and
   simulation time - your choice.
 */



#include <stdlib.h>
#include <stdbool.h>
#include <util.h>
#include <vector.h>
#include <well_ts.h>
#include <well_const.h>
#include <well_state.h>



#define WELL_TS_TYPE_ID    6613005
#define WELL_NODE_TYPE_ID  1114652

typedef struct {
  UTIL_TYPE_ID_DECLARATION;
  int                 report_nr;
  time_t              sim_time;  
  well_state_type   * well_state;  // The well_node instance owns the well_state instance.
} well_node_type;


struct well_ts_struct {
  UTIL_TYPE_ID_DECLARATION;
  time_t               min_time;
  time_t               max_time;
  int                  min_report;
  int                  max_report;
  char               * well_name;
  vector_type        * ts;    
};

/******************************************************************/

static well_node_type * well_node_alloc( well_state_type * well_state) {
  well_node_type * node = util_malloc( sizeof * node , __func__ );
  UTIL_TYPE_ID_INIT( node , WELL_NODE_TYPE_ID );
  node->report_nr  = well_state_get_report_nr( well_state );
  node->sim_time   = well_state_get_sim_time( well_state );
  node->well_state = well_state;
  return node;
}


static UTIL_SAFE_CAST_FUNCTION( well_node , WELL_NODE_TYPE_ID )


static void well_node_free( well_node_type * well_node ) {
  well_state_free( well_node->well_state );
  free( well_node );
}

static void well_node_free__( void * arg ) {
  well_node_type * node = well_node_safe_cast( arg );
  well_node_free( node );
}

/*****************************************************************/

static well_ts_type * well_ts_alloc_empty( ) {
  well_ts_type * well_ts = util_malloc( sizeof * well_ts , __func__ );
  UTIL_TYPE_ID_INIT( well_ts , WELL_TS_TYPE_ID );

  well_ts->ts          = vector_alloc_new();
  
  well_ts->min_time   = -1;
  well_ts->max_time   = -1;
  well_ts->min_report = -1;
  well_ts->max_report = -1;
  return well_ts;
}

static UTIL_SAFE_CAST_FUNCTION( well_ts , WELL_TS_TYPE_ID )


well_ts_type * well_ts_alloc( const char * well_name ) {
  well_ts_type * well_ts = well_ts_alloc_empty();
  well_ts->well_name = util_alloc_string_copy( well_name );
  return well_ts;
}


static int well_ts_get_index( const well_ts_type * well_ts , int report_step , time_t sim_time , bool use_report) {
  const int size = vector_get_size( well_ts->ts );

  if (use_report) {
    if (report_step < well_ts->min_report)
      return -1;         // Before the start

    if (report_step >= well_ts->max_report)
      return size - 1;   // After end
  } else {
    if (sim_time < well_ts->min_time)
      return -1;         // Before the start

    if (sim_time >= well_ts->max_time)
      return size - 1;   // After end
  }

  // Binary search 
  {
    int lower_index  = 0;
    int upper_index  = size - 1;

    const well_node_type * lower_node  = vector_iget_const( well_ts->ts , lower_index );
    const well_node_type * upper_node  = vector_iget_const( well_ts->ts , upper_index );

    while (true) {
      int center_index = (lower_index + upper_index) / 2;
      const well_node_type * center_node = vector_iget_const( well_ts->ts , center_index );      
      double cmp;
      if (use_report)
	cmp = center_node->report_nr - report_step;
      else
	cmp = difftime( center_node->sim_time , sim_time );
      
      if (cmp > 0) {
	if ((center_index - lower_index) == 1)    // We found an interval of length 1
	  return lower_index;
	else {
	  upper_index = center_index;
	  upper_node  = vector_iget_const( well_ts->ts , upper_index );
	}
	
      } else {
	
	if ((upper_index - center_index) == 1)    // We found an interval of length 1
	  return center_index;
	else {
	  lower_index = center_index;
	  lower_node  = vector_iget_const( well_ts->ts , lower_index );
	}

      }
    }
  }
}


void well_ts_add_well( well_ts_type * well_ts , well_state_type * well_state ) {
  well_node_type * new_node = well_node_alloc( well_state );
  if (new_node->report_nr > well_ts->max_report) 
    // This is the fast path which will apply when the well_state objects
    // are added in a time-ordered fashion.
    vector_append_owned_ref( well_ts->ts , new_node , well_node_free__ );
  else {
    int  index  = well_ts_get_index( well_ts , new_node->report_nr , -1 , true);
    if (index < 0) 
      index = 0;
    
    {
      const well_node_type * exnode = vector_iget_const( well_ts->ts , index );
      if (exnode->report_nr == new_node->report_nr)
	// Replace the exsisting node
	vector_iset_owned_ref( well_ts->ts , index , new_node , well_node_free__ );
      else
	// Insert a new node - pushing the existing to the right.
	vector_insert_owned_ref( well_ts->ts , index , new_node , well_node_free__ );	
    }
  }
  
  
  if (well_ts->min_time == -1) {
    well_ts->min_time = new_node->sim_time;
    well_ts->max_time = new_node->sim_time;

    well_ts->min_report = new_node->report_nr;
    well_ts->max_report = new_node->report_nr;
  } else {
    well_ts->min_time = util_time_t_min( well_ts->min_time , new_node->sim_time );
    well_ts->max_time = util_time_t_max( well_ts->max_time , new_node->sim_time );
							     
    well_ts->min_report = util_int_min(well_ts->min_report , new_node->report_nr);
    well_ts->max_report = util_int_max(well_ts->max_report , new_node->report_nr);
  }
}


static void well_ts_fprintf( const well_ts_type * well_ts , FILE * stream) {
  fprintf(stream,"Report step: %d - %d \n",well_ts->min_report , well_ts->max_report);
}

void well_ts_free( well_ts_type * well_ts ){
  free( well_ts->well_name );
  vector_free( well_ts->ts );
  free( well_ts );
}



void well_ts_free__( void * arg ) {
  well_ts_type * well_ts = well_ts_safe_cast( arg );
  well_ts_free( well_ts );
}


int well_ts_get_size( const well_ts_type * well_ts) {
  return vector_get_size( well_ts->ts );
}

well_state_type * well_ts_iget_state( const well_ts_type * well_ts , int index) {
  return vector_iget( well_ts->ts , index );
}




well_state_type * well_ts_get_state_from_report( const well_ts_type * well_ts , int report_step) {
  int index = well_ts_get_index( well_ts , report_step , -1 , true );
  if (index < 0)
    return NULL;
  else {
    well_node_type * node = vector_iget( well_ts->ts , index );
    return node->well_state;
  }
}


well_state_type * well_ts_get_state_from_sim_time( const well_ts_type * well_ts , time_t sim_time) {
  int index = well_ts_get_index( well_ts , -1 , sim_time , false );
  if (index < 0)
    return NULL;
  else {
    well_node_type * node = vector_iget( well_ts->ts , index );
    return node->well_state;
  }
}
