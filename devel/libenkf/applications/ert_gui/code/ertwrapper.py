
from ctypes import *
import ctypes.util


class ErtWrapper:

    def __init__(self, site_config="/project/res/etc/ERT/Config/site-config", enkf_config="/private/jpb/EnKF/Testcases/SimpleEnKF/enkf_config", enkf_so="/private/jpb/EnKF/"):
        self.__loadLibraries__(enkf_so)

        #bootstrap
        self.main = self.enkf.enkf_main_bootstrap(site_config, enkf_config)
        self.plot_config = self.enkf.enkf_main_get_plot_config(self.main)


#        self.plot_path = "somepath"
#        self.driver = "PLPLOT"
#        self.errorbar = 1256
#        self.width = 1920
#        self.height = 1080
#        self.image_viewer = "another path"
#        self.image_type = "png"

        self.eclbase="eclpath"
        self.data_file="eclpath"
        self.grid="eclpath"
        self.schedule_file="eclpath"
        self.init_section="init_section"
        self.add_fixed_length_schedule_kw = ["item1", "item2"]
        self.add_static_kw = ["item1", "item2", "item3"]
        self.equil_init_file = "some new path"
        self.refcase = "some new path"
        self.schedule_prediction_file = "some new path"
        self.data_kw = {"INCLUDE_PATH" : "<CWD>/../Common/ECLIPSE2", "INCLUDE_PATH2" : "<CWD>/../Common/ECLIPSE2", "INCLUDE_PATH3" : "<CWD>/../Common/ECLIPSE2"}

        self.enkf_rerun = False
        self.rerun_start = 0
        self.enkf_sched_file = "..."
        self.local_config = "..."
        self.enkf_merge_observations = False
        self.enkf_mode = "SQRT"
        self.enkf_alpha = 2.5
        self.enkf_truncation = 0.99

        self.queue_system = "RSH"
        self.lsf_queue = "NORMAL"
        self.max_running_lsf = 10
        self.lsf_resources = "magic string"
        self.rsh_command = "ssh"
        self.max_running_rsh = 55
        self.max_running_local = 4
        self.rsh_host_list = {"host1" : '', "host2" : '6'}

        self.job_script = "..."
        self.setenv = {"LSF_BINDIR" : "/prog/LSF/7.0/linux2.6-glibc2.3-x86_64/bin", "LSF_LIBDIR" : "/prog/LSF/7.0/linux2.6-glibc2.3-x86_64/lib"}
        self.update_path = {"PATH" : "/prog/LSF/7.0/linux2.6-glibc2.3-x86_64/bin", "LD_LIBRARY_PATH" : "/prog/LSF/7.0/linux2.6-glibc2.3-x86_64/lib"}
        self.install_job = {"ECHO" : "/prog/LSF/7.0/linux2.6-glibc2.3-x86_64/bin", "ADJUSTGRID" : "/prog/LSF/7.0/linux2.6-glibc2.3-x86_64/lib"}


        self.dbase_type = "BLOCK_FS"
        self.enspath = "storage"
        self.select_case = "some_case"

        self.log_file = "log"
        self.log_level = 1
        self.update_log_path = "one path"

        self.history_source = "REFCASE_HISTORY"
        self.obs_config = "..."

        self.runpath = "simulations/realization%d"
        self.pre_clear_runpath = True
        self.delete_runpath = "0 - 10, 12, 15, 20"
        self.keep_runpath = "0-15, 18, 20"

        self.license_path = "/usr"
        self.max_submit = 2
        self.max_resample = 16
        self.case_table = "..."

        self.run_template = [["...", ".....", "asdf:asdf asdfasdf:asdfasdf"], ["other", "sdtsdf", ".as.asdfsdf"]]
#        self.run_template = "..."
#        self.target_file = "..."
#        self.template_arguments = {"HABBA":"sdfkjskdf/sdf"}
        self.forward_model = {"MY_RELPERM_SCRIPT":"Arg1<some> COPY(asdfdf)"}

        self.field_dynamic = [["PRESSURE", "", ""], ["SWAT", 0.1, 0.95], ["SGAS", "", 0.25]]
        self.field_parameter = [["PRESSURE", "", "", "RANDINT", "EXP", "...", "/path/%d"], ["SWAT", 0.1, 0.95, "", "", ".", "..."]]
        self.field_general = [["PERMEABILITY", "", "", "RANDINT", "EXP", "...", "/path/%d", "path to somewhere", "another path"]]

        self.gen_data = [["5DWOC", "SimulatedWOCCA.txt", "ASCII", "BINARY_DOUBLE", "---", "/path/%d"]]
        self.gen_kw = [["MULTFLT", "Templates/MULTFLT_TEMPLATE", "MULTFLT.INC", "Parameters/MULTFLT.txt"]]
        self.gen_param = [["DRIBBLE", "ASCII", "ASCII_TEMPLATE", "...", "/path/%d", "/some/file Magic123"]]

        self.num_realizations = 100
        self.summary = ["WPOR:MY_WELL", "RPR:8", "F*"]





    def __loadLibraries__(self, prefix):
        libraries = ["libutil/slib/libutil.so",
                     "libecl/slib/libecl.so",
                     "libsched/slib/libsched.so",
                     "librms/slib/librms.so",
                     "libconfig/slib/libconfig.so",
                     "libjob_queue/slib/libjob_queue.so"]

        CDLL("libblas.so", RTLD_GLOBAL)
        CDLL("liblapack.so", RTLD_GLOBAL)
        CDLL("libz.so", RTLD_GLOBAL)

        for lib in libraries:
            CDLL(prefix + lib, RTLD_GLOBAL)

        self.enkf = CDLL(prefix + "libenkf/slib/libenkf.so", RTLD_GLOBAL)


    def setRestype(self, attribute, restype):
        getattr(self.enkf, attribute).restype = restype     

    def setAttribute(self, attribute, value):
        print "set " + attribute + ": " + str(getattr(self, attribute)) + " -> " + str(value)
        setattr(self, attribute, value)

    def getAttribute(self, attribute):
        print "get " + attribute + ": " + str(getattr(self, attribute))
        return getattr(self, attribute)
