## Author: ZH (sharpstorm)

import xml.etree.ElementTree as ET
import argparse
from os.path import exists
import glob
import os
import subprocess

class style:
    MAGENTA = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    RESET = '\033[0m'

class Result:
    def __init__(self, tId, comment, timeTaken, query, expected, actual):
        self._testId = tId
        self._comment = comment
        self._timeTaken = timeTaken
        self._query = query
        self._expected = expected
        self._actual = actual
        self._isPass = False
        self._failData = None

    def setPass(self):
        self._isPass = True
        return self

    def setFail(self, failData):
        self._isPass = False
        self._failData = failData
        return self

    def isPass(self):
        return self._isPass

    def getAdditional(self):
        return self._failData['additional']

    def getMissing(self):
        return self._failData['missing']

    def getInfo(self, isShowComment, isShowTime, isShowAnswer, isShowQuery, isShowFailData):
        s = f'Test Case {self._testId}: ' + \
            (f'{style.GREEN}Pass{style.RESET}' if self.isPass() else f'{style.RED}Fail{style.RESET}')

        if isShowTime:
            s += f' ({self._timeTaken})'

        if isShowComment:
            s += f'\n[{self._comment}]'

        if isShowQuery:
            s += f'\n{style.BLUE}Query: {style.RESET}{self._query}'

        if isShowAnswer and not self.isPass():
            s += f'\n{style.BLUE}Expected: {style.RESET}{self._expected}'
            s += f'\n{style.BLUE}Got: {style.RESET}{self._actual}'

        if isShowFailData and not self.isPass():
            s += f'{style.YELLOW}\n' + \
                 f'Additional: {self.getAdditional()}\n' + \
                 f'Missing: {self.getMissing()}\n' + \
                 f'{style.RESET}'

        return s

    def __str__(self):
        return f'Test Case {self.testId}'

def processResult(node):
    testId = None
    comment = ''
    timeTaken = None
    isPass = False
    failData = None

    query = None
    expected = None
    actual = None
    
    for x in node.iterfind('./'):
        if x.tag == 'id':
            testId = int(x.text)
            comment = x.get('comment')
        
        elif x.tag == 'querystr':
            query = x.text
        
        elif x.tag == 'time_taken':
            timeTaken = float(x.text)
        
        elif x.tag == 'correct':
            expected = '' if x.text is None else x.text
        
        elif x.tag == 'stuans':
            actual = '' if x.text is None else x.text
        
        elif x.tag == 'passed':
            isPass = True
        
        elif x.tag == 'failed':
            failData = {}
            failData['missing'] = x.find('./missing').text
            failData['additional'] = x.find('./additional').text

    if testId is None or \
       timeTaken is None or \
       query is None or \
       expected is None or \
       actual is None or \
       (not isPass and failData is None):
        print(actual is None)
        print(expected is None)
        return None

    res = Result(testId, comment, timeTaken, query, expected, actual)
    if isPass:
        res.setPass()
    else:
        res.setFail(failData)

    return res

def parseXML(fileName):
    tree = ET.parse(fileName)
    result = []
    passCount = 0
    for x in tree.getroot().iterfind('./queries/query'):
        record = processResult(x)
        result.append(record)
        passCount += (1 if record.isPass() else 0)
    
    return { 'result': result, 'passCount': passCount }

def printResult(data, args):
    isShowComment = args.show_comment
    isShowTime = args.show_time
    isShowAnswer = args.show_answer
    isShowQuery = args.show_query
    isShowFailData = args.show_fail_data
    isSummary = args.summary

    resultArr = data['result']
    passCount = data['passCount']
    totalCount = len(resultArr)

    if not isSummary:
        print(f'{style.MAGENTA}-------- Test Summary: {totalCount} Tests --------{style.RESET}')
        print()
        for x in resultArr:
            print(x.getInfo(isShowComment, isShowTime, isShowAnswer, isShowQuery, isShowFailData))
            print()
        print()

    if passCount == totalCount:
        print(f'{style.GREEN}All {passCount} Tests Passed.${style.RESET}')
    else:
        print(f'{style.GREEN}{passCount}/{len(resultArr)} Tests Passed, {style.RED}{len(resultArr) - passCount} Failed.{style.RESET}')

def execTests(args, jobs):
    tempFileName = 'out.xml'
    autotesterBinary = args.autotester_binary
    isFastFail = not args.slow_fail

    isOverallFail = False
    passCount = 0
    totalCount = 0
    for j in jobs:
        query = j[0]
        source = j[1]
        isFailure = False

        print(f'{style.MAGENTA}-------- Run: {query} --------{style.RESET}')
        exitCode = subprocess.run([autotesterBinary, source, query, tempFileName],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.STDOUT).returncode
        if exitCode != 0:
            print(f'{style.RED} !!!!!! Non-Zero Exit Code: {exitCode} !!!!!!! {style.RED}')
            isFailure = True

        if exists(tempFileName):
            result = parseXML(tempFileName)
            printResult(result, args)
            passCount += result['passCount']
            totalCount += len(result['result'])
            os.remove(tempFileName)

            if passCount != totalCount:
                isFailure = True

        print(f'{style.MAGENTA}------------------------------{style.RESET}')
        print()
        
        isOverallFail = isOverallFail or isFailure
        if isOverallFail and isFastFail:
            print(f'{style.RED} Batch Testing Failed{style.RESET}')
            return 1

    if isOverallFail:
        print(f'{style.RED} Slow-Fail Batch Testing Failed{style.RESET}')
        return 1

    if passCount == totalCount:
        print(f'{style.GREEN}All {passCount} Tests Passed.${style.RESET}')
    else:
        print(f'{style.GREEN}{passCount}/{len(resultArr)} Tests Passed, {style.RED}{len(resultArr) - passCount} Failed.{style.RESET}')
    

def runTests(args):
    testDir = args.test_folder
    jobs = []
    queryFiles = glob.glob(f'{testDir}/**/*-queries.txt', recursive=True)
    print(testDir)
    for x in queryFiles:
        expectedSource = f'{x[:-12]}-source.txt'
        print(expectedSource)
        if exists(expectedSource):
            jobs.append((x, expectedSource))
    
    print(f'{style.MAGENTA}-------- Auto Autotester Found {len(jobs)} Jobs --------{style.RESET}')
    print()
    return execTests(args, jobs)

def main():
    parser = argparse.ArgumentParser(
        prog='auto-autotester.py'
    )
    parser.add_argument('autotester_binary', help='The autotester binary')
    parser.add_argument('test_folder', help='The folder containing all the tests')
    parser.add_argument('-t', '--show-time', help='Show Time', action='store_true')
    parser.add_argument('-c', '--show-comment', help='Print comment', action='store_true')
    parser.add_argument('-r', '--show-answer', help='Print answer information', action='store_true')
    parser.add_argument('-q', '--show-query', help='Print test queries', action='store_true')
    parser.add_argument('-f', '--show-fail-data', help='Print test queries', action='store_true')
    parser.add_argument('-s', '--summary', help='Prints only a 1-line summary', action='store_true')

    # Testing Behaviour
    parser.add_argument('-sf', '--slow-fail', help='Do not stop upon encountering an error', action='store_true')

    args = parser.parse_args()
    exit(runTests(args))

main()
