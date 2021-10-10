from _export import lib,ffi
import unittest,BeautifulReport

# print(lib.Judge())

class AddTestCase(unittest.TestCase):
    def test_case1(self):
        ch = "\n"
        self.assertEqual(lib.space(ch.encode("ascii")),1)
    def test_case2(self):
        ch = "a"
        self.assertEqual(lib.space(ch.encode("ascii")),1)  # 实际答案0
    def test_case3(self):
        num = "1"
        self.assertEqual(lib.identify(num.encode("ascii")),1)
    def test_case4(self):
        ch = '@'
        self.assertEqual(lib.identify(ch.encode("ascii")),1)  # 实际答案0
    def test_case5(self):
        ch = 'a'
        self.assertEqual(lib.identify(ch.encode("ascii")),1) 
    def test_case6(self):
        ch = '0'
        self.assertEqual(lib.uninteger(ch.encode("ascii")),1) 
    def test_case7(self):
        ch = 'b'
        self.assertEqual(lib.uninteger(ch.encode("ascii")),1)  # 实际输出0
    def test_case8(self):
        ch = '='
        self.assertEqual(lib.twoStep(ch.encode("ascii")),1) 
    def test_case9(self):
        ch = '<'
        self.assertEqual(lib.twoStep(ch.encode("ascii")),2)   # 实际输出3
    def test_case10(self):
        ch = '!'
        self.assertEqual(lib.twoStep(ch.encode("ascii")),3)

sut = unittest.TestSuite()

sut.addTest(unittest.makeSuite(AddTestCase))

run = BeautifulReport.BeautifulReport(sut)

run.report(filename="./indx.html",description="编译原理单元测试")