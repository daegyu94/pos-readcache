#!/usr/bin/env python3
import subprocess
import os
import sys
sys.path.append("../")
sys.path.append("../../system/lib/")

import json_parser
import pos
import cli
import api
import DELETE_ARRAY_AFTER_UNMOUNT

ARRAYNAME = DELETE_ARRAY_AFTER_UNMOUNT.ARRAYNAME

def execute():
    out = DELETE_ARRAY_AFTER_UNMOUNT.execute()
    ret = json_parser.get_response_code(out)
    if ret == 0:
        out = cli.delete_array(ARRAYNAME)
    return out

if __name__ == "__main__":
    api.clear_result(__file__)
    out = execute()
    ret = api.set_result_by_code_ne(out, 0, __file__)
    pos.kill_pos()
    exit(ret)