#ifndef __ECL_REGION_H__
#define __ECL_REGION_H__

#include <ecl_grid.h>
#include <stdbool.h>

typedef struct ecl_region_struct ecl_region_type; 

ecl_region_type * ecl_region_alloc( const ecl_grid_type * ecl_grid , bool preselect);
void              ecl_region_free( ecl_region_type * region );

int               ecl_region_get_active_size( ecl_region_type * region );
const int       * ecl_region_get_active_list( ecl_region_type * region );
int               ecl_region_get_global_size( ecl_region_type * region );
const int       * ecl_region_get_global_list( ecl_region_type * region );


void              ecl_region_select_all( ecl_region_type * region);
void              ecl_region_deselect_all( ecl_region_type * region );
void              ecl_region_select_in_interval( ecl_region_type * region , const ecl_kw_type * ecl_kw, float min_value , float max_value);
void              ecl_region_deselect_in_interval( ecl_region_type * region , const ecl_kw_type * ecl_kw, float min_value , float max_value);
void              ecl_region_select_equal( ecl_region_type * region , const ecl_kw_type * ecl_kw, int value);
void              ecl_region_deselect_equal( ecl_region_type * region , const ecl_kw_type * ecl_kw, int value);
void              ecl_region_select_from_box( ecl_region_type * region , const ecl_box_type * ecl_box );
void              ecl_region_deselect_from_box( ecl_region_type * region , const ecl_box_type * ecl_box );
void              ecl_region_select_i1i2( ecl_region_type * region , int i1 , int i2);
void              ecl_region_deselect_i1i2( ecl_region_type * region , int i1 , int i2);
void              ecl_region_select_j1j2( ecl_region_type * region , int j1 , int j2);
void              ecl_region_deselect_j1j2( ecl_region_type * region , int j1 , int i2);
void              ecl_region_select_k1k2( ecl_region_type * region , int k1 , int k2);
void              ecl_region_deselect_k1k2( ecl_region_type * region , int k1 , int i2);
void              ecl_region_invert_selection( ecl_region_type * region );

UTIL_IS_INSTANCE_HEADER( ecl_region );
UTIL_SAFE_CAST_HEADER( ecl_region );


#endif
