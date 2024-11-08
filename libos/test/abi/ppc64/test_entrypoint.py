#!/usr/bin/env python3

from graminelibos.regression import RegressionTestCase

class TC_00_Entrypoint(RegressionTestCase):
    def Xtest_050_stack(self):
        self.run_binary(['stack'])

    def Xtest_070_env(self):
        self.run_binary(['stack_env'])

    def Xtest_080_auxv(self):
        self.run_binary(['stack_auxiliary'])
