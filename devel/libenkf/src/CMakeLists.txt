set( source_files rng_config.c trans_func.c enkf_types.c enkf_obs.c obs_data.c field_obs.c enkf_config_node.c field_config.c field.c ecl_static_kw.c enkf_state.c enkf_util.c enkf_node.c gen_kw_config.c gen_kw.c enkf_fs.c fs_driver.c meas_data.c summary_obs.c summary.c summary_config.c gen_data_config.c gen_data.c gen_common.c gen_obs.c enkf_sched.c enkf_serialize.c plain_driver_common.c ecl_config.c enkf_defaults.c ensemble_config.c model_config.c site_config.c active_list.c obs_vector.c field_trans.c plain_driver_index.c plain_driver.c config_parser.c enkf_analysis.c local_ministep.c local_updatestep.c local_context.c local_config.c analysis_config.c misfit_table.c fs_types.c block_fs_driver.c  block_fs_driver_index.c plot_config.c ert_template.c member_config.c enkf_main.c local_dataset.c local_obsset.c surface.c surface_config.c)

set( header_files rng_config.h enkf_fs_type.h trans_func.h enkf_obs.h obs_data.h enkf_config_node.h field_obs.h field_config.h field.h enkf_macros.h ecl_static_kw.h enkf_state.h enkf_util.h enkf_main.h enkf_node.h enkf_fs.h gen_kw_config.h gen_kw.h enkf_types.h fs_driver.h  meas_data.h summary_obs.h summary_config.h summary_config.h gen_data_config.h gen_data.h gen_common.h gen_obs.h enkf_sched.h fs_types.h enkf_serialize.h plain_driver_common.h ecl_config.h ensemble_config.h model_config.h site_config.h active_list.h obs_vector.h field_trans.h plain_driver_index.h plain_driver.h config_parser.h enkf_analysis.h local_ministep.h local_updatestep.h local_config.h analysis_config.h misfit_table.h block_fs_driver.h block_fs_driver_index.h field_common.h gen_kw_common.h gen_data_common.h plot_config.h ert_template.h member_config.h enkf_defaults.h ert_build_info.h local_dataset.h local_obsset.h surface.h surface_config.h local_context.h)

add_library( enkf_static  STATIC ${source_files} )
add_library( enkf_shared  SHARED ${source_files} )

set_target_properties( enkf_static PROPERTIES OUTPUT_NAME enkf )
set_target_properties( enkf_shared PROPERTIES OUTPUT_NAME enkf )

exec_program(svnversion ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE SVN_VERSION)
exec_program(date OUTPUT_VARIABLE COMPILE_TIME_STAMP)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/CMake/config/ert_build_info.h.in
                ${CMAKE_CURRENT_BINARY_DIR}/ert_build_info.h )

#list( APPEND src_files ${CMAKE_CURRENT_BINARY_DIR}/ert_build_info.h )

target_link_libraries( enkf_static ecl_static geometry_static sched_static analysis_static rms_static plot_static config_static job_queue_static util_static )
target_link_libraries( enkf_shared ecl_shared geometry_shared sched_shared analysis_shared rms_shared plot_shared config_shared job_queue_shared util_shared )

#-----------------------------------------------------------------
if (ENKF_INSTALL_PREFIX)
   install(TARGETS enkf_static enkf_shared DESTINATION ${INSTALL_ROOT}/${ENKF_INSTALL_PREFIX}/lib)
   install(FILES ${header_files} DESTINATION ${INSTALL_ROOT}/${ENKF_INSTALL_PREFIX}/include)
else()
   install(TARGETS enkf_static enkf_shared DESTINATION ${INSTALL_ROOT}/lib)
   install(FILES ${header_files} DESTINATION ${INSTALL_ROOT}/include)
endif()        


add_dependencies( enkf_static ecl_static )
add_dependencies( enkf_shared ecl_shared )