## poseidonos-cli device

Device commands for PoseidonOS.

### Synopsis

Device commands for PoseidonOS.

Syntax: 
  poseidonos-cli device [scan|list|smart]

Example (to scan devices in the system):
  poseidonos-cli device scan
	  

```
poseidonos-cli device [flags]
```

### Options

```
  -h, --help   help for device
```

### Options inherited from parent commands

```
      --debug         Print response for debug
      --fs string     Field separator for the output (default "|")
      --ip string     Set IPv4 address to PoseidonOS for this command (default "127.0.0.1")
      --json-req      Print request in JSON form
      --json-res      Print response in JSON form
      --port string   Set the port number to PoseidonOS for this command (default "18716")
      --unit          Display unit (B, KB, MB, ...) when displaying capacity
```

### SEE ALSO

* [poseidonos-cli](poseidonos-cli.md)	 - poseidonos-cli - A command-line interface for PoseidonOS [version 0.7]
* [poseidonos-cli device create](poseidonos-cli_device_create.md)	 - Create a buffer device.
* [poseidonos-cli device list](poseidonos-cli_device_list.md)	 - List all devices in the system.
* [poseidonos-cli device scan](poseidonos-cli_device_scan.md)	 - Scan devices in the system.
* [poseidonos-cli device smart](poseidonos-cli_device_smart.md)	 - Show SMART information from an NVMe log page.
