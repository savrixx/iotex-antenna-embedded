#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys
import json


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("{} <json log file>".format(sys.argv[0]))
        sys.exit(0)

    with open(sys.argv[1]) as fp:
        records = json.load(fp)

    funcs = set(records[0].keys())
    failed = dict(zip(funcs, [0] * len(funcs)))
    malloc_failed = dict(zip(funcs, [0] * len(funcs)))

    for record in records:
        for func in funcs:
            if record[func] != 0:
                failed[func] = failed[func] + 1
                if record[func] == -2:
                    malloc_failed[func] = malloc_failed[func] + 1

    for func in funcs:
        failed[func] = failed[func] / len(records) * 100
        malloc_failed[func] = malloc_failed[func] / len(records) * 100

    print("Total: {}".format(len(records)))
    print("F", failed)
    print("M", malloc_failed)