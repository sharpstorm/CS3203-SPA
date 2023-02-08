import xml.etree.ElementTree as ET
import sys
import argparse

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

def main():
    parser = argparse.ArgumentParser(
        prog='auto-autotester.py'
    )
    parser.add_argument('filename', help='The output XML file from autotester')
    parser.add_argument('-t', '--show-time', help='Show Time', action='store_true')
    parser.add_argument('-c', '--show-comment', help='Print comment', action='store_true')
    parser.add_argument('-r', '--show-answer', help='Print answer information', action='store_true')
    parser.add_argument('-q', '--show-query', help='Print test queries', action='store_true')
    parser.add_argument('-f', '--show-fail-data', help='Print test queries', action='store_true')
    parser.add_argument('-s', '--summary', help='Prints only a 1-line summary', action='store_true')

    args = parser.parse_args()
    data = parseXML(args.filename)
    exit(printResult(data, args))

main()
