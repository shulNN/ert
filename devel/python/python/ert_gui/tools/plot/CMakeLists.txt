set(PYTHON_SOURCES
    __init__.py
    color_chooser.py
    customize_plot_widget.py
    data_type_keys_list_model.py
    data_type_proxy_model.py
    data_type_keys_widget.py
    export_plot.py
    export_plot_widget.py
    filter_popup.py
    plot_bridge.py
    plot_case_model.py
    plot_case_selection_widget.py
    plot_metrics_widget.py
    plot_scale_widget.py
    plot_panel.py
    plot_tool.py
    plot_window.py
    report_step_widget.py
    scale_tracker.py
)

add_python_package("python.ert_gui.tools.plot"  ${PYTHON_INSTALL_PREFIX}/ert_gui/tools/plot "${PYTHON_SOURCES}" True)

add_subdirectory(data)
