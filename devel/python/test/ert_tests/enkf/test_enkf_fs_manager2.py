import os
from ert.enkf import EnkfFs
from ert.enkf import EnKFMain
from ert.enkf import EnkfFsManager
from ert.enkf import ErtTestContext
from ert_tests import ExtendedTestCase


class EnKFFSManagerTest2(ExtendedTestCase):
    def setUp(self):
        self.config_file = self.createTestPath("Statoil/config/with_data/config")


    def test_rotate(self):

        # We are indirectly testing the create through the create
        # already in the enkf_main object. In principle we could
        # create a separate manager instance from the ground up, but
        # then the reference count will be weird.
        with ErtTestContext("enkf_fs_manager_rotate_test", self.config_file) as testContext:
            ert = testContext.getErt()
            fsm = ert.getEnkfFsManager()
            self.assertEqual(1, fsm.getFileSystemCount())

            fs1 = fsm.getFileSystem("FSA")
            fs2 = fsm.getFileSystem("FSB")
            self.assertEqual(EnkfFsManager.DEFAULT_CAPACITY, fsm.getFileSystemCount())

            fs_list = []
            for i in range(10):
                fs = "FS%d" % i
                print("Mounting: %s" % fs)
                fs_list.append(fsm.getFileSystem(fs))
                self.assertEqual(EnkfFsManager.DEFAULT_CAPACITY, fsm.getFileSystemCount())

            
