import graph
import iogen
import json
import lib
import node
import rsfmt
import traceback


def play(tgts, inits, scenario, timestamp, data):
    try:  # Prepare sequence
        node_manager = node.NodeManager(tgts, inits)
        targets, initiators = node_manager.initialize()

        target_name = tgts[0]["NAME"]
        target_object = targets[target_name]
        target_object.cli.wbt_bypass("ARR0", 1)
        target_object.cli.wbt_bypass("ARR1", 1)

        test_case_list = [
            {"name": "1_sw", "rw": "write", "bs": "128k", "iodepth": "4", "io_size": "100%",
                "time_based": "0", "runtime": "0", "log_avg_msec": "30000"},
            {"name": "3_rw", "rw": "randwrite", "bs": "4k", "iodepth": "128",
                "io_size": "196t", "time_based": "0", "runtime": "0", "log_avg_msec": "2000"}
        ]

        grapher = graph.manager.Grapher(scenario, timestamp)
        result_fmt = rsfmt.manager.Formatter(scenario, timestamp)
        result_fmt.add_test_cases([tc['name'] for tc in test_case_list])
    except Exception as e:
        lib.printer.red(traceback.format_exc())
        return data

    try:  # Test sequence
        for test_case in test_case_list:
            # setup fio_cmd
            fio_cmd_list = []
            for key in initiators:
                fio_cmd = iogen.fio.Fio(initiators[key], timestamp)
                fio_cmd.initialize()
                fio_cmd.update(test_case)
                fio_cmd_list.append(fio_cmd.stringify())

            # run fio
            lib.printer.green(f" run -> {timestamp} {test_case['name']}")
            result_fmt.start_test(test_case["name"])
            lib.subproc.sync_parallel_run(fio_cmd_list, True)

            # copy output
            for key in initiators:
                initiators[key].copy_output(
                    timestamp, test_case["name"], scenario["OUTPUT_DIR"])

            # get result
            for key in initiators:
                file = (
                    f"{scenario['OUTPUT_DIR']}/{timestamp}_"
                    f"{test_case['name']}_{key}"
                )
                fio_result = lib.parser.parse_json_file(file)
                print(json.dumps(fio_result, indent=2))

            # set result status (& message)
            result_fmt.end_test(test_case["name"], "pass")

            # draw graph
            for key in initiators:
                grapher.draw(initiators[key], test_case["name"])
    except Exception as e:
        lib.printer.red(traceback.format_exc())

    target_name = tgts[0]["NAME"]
    target_object = targets[target_name]
    target_object.cli.wbt_bypass("ARR0", 0)
    target_object.cli.wbt_bypass("ARR1", 0)

    try:  # Wrapup sequence
        result_fmt.write_file()
        node_manager.finalize()
    except Exception as e:
        lib.printer.red(traceback.format_exc())

    return data
