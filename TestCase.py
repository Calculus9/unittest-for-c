import unittest, importlib , BeautifulReport, cffi


def load():
    src = open("add.c").read()
    inc = open(("add.h")).read()

    builder = cffi.FFI()#生成cffi实例

    builder.cdef(inc)
    builder.set_source("_export",src)
    builder.compile()

    # md = importlib.import_module("addLib")

    # return md.lib


md = load()



class AddTestCase(unittest.TestCase):

    def test_case1(self):

        '''
        这是第一个CASE
        '''
        self.assertEqual(md.add(1,2),1+2)

        print("md.multi(1,2),1+2")

    def test_case2(self):

        '''
        这是第二个CASE
        '''
        self.assertEqual(md.multi(1,5),1+2)

        print("md.multi(1,2),1+2")


sut = unittest.TestSuite()

sut.addTest(unittest.makeSuite(AddTestCase))

run = BeautifulReport.BeautifulReport(sut)

run.report(filename="./test.html",description="add单元测试")