U-Boot 2024.10-01035-g98a36deb9ab7 (Nov 02 2024 - 03:02:54 +0300)

CPU  : AM335X-GP rev 2.1
Model: TI AM335x BeagleBone Black
DRAM:  512 MiB
Core:  161 devices, 18 uclasses, devicetree: separate
WDT:   Started wdt@44e35000 with servicing every 1000ms (60s timeout)
NAND:  0 MiB
MMC:   OMAP SD/MMC: 0, OMAP SD/MMC: 1
Loading Environment from FAT... OK
Net:   eth2: ethernet@4a100000using musb-hdrc, OUT ep1out IN ep1in STATUS ep2in
MAC de:ad:be:ef:00:01
HOST MAC de:ad:be:ef:00:00
RNDIS ready
, eth3: usb_ether
Hit any key to stop autoboot:  0 
220 bytes read in 2 ms (107.4 KiB/s)
## Warning: Input data exceeds 1048576 bytes - truncated
## Info: input data size = 1048578 = 0x100002
4863920 bytes read in 322 ms (14.4 MiB/s)
69970 bytes read in 8 ms (8.3 MiB/s)
Kernel image @ 0x82000000 [ 0x000000 - 0x4a37b0 ]
## Flattened Device Tree blob at 88000000
   Booting using the fdt blob at 0x88000000
Working FDT set to 88000000
   Loading Device Tree to 8ffeb000, end 8ffff151 ... OK
Working FDT set to 8ffeb000

Starting kernel ...

[    0.000000] Booting Linux on physical CPU 0x0
[    0.000000] Linux version 6.1.134-cip41-rt22-cutom-dirty (root@heval) (arm-none-linux-gnueabihf-g5
[    0.000000] CPU: ARMv7 Processor [413fc082] revision 2 (ARMv7), cr=10c5387d
[    0.000000] CPU: PIPT / VIPT nonaliasing data cache, VIPT aliasing instruction cache
[    0.000000] OF: fdt: Machine model: TI AM335x BeagleBone Black
[    0.000000] Memory policy: Data cache writeback
[    0.000000] cma: Reserved 16 MiB at 0x9e800000
[    0.000000] Zone ranges:
[    0.000000]   Normal   [mem 0x0000000080000000-0x000000009fdfffff]
[    0.000000]   HighMem  empty
[    0.000000] Movable zone start for each node
[    0.000000] Early memory node ranges
[    0.000000]   node   0: [mem 0x0000000080000000-0x000000009fdfffff]
[    0.000000] Initmem setup node 0 [mem 0x0000000080000000-0x000000009fdfffff]
[    0.000000] CPU: All CPU(s) started in SVC mode.
[    0.000000] AM335X ES2.1 (sgx neon)
[    0.000000] percpu: Embedded 16 pages/cpu s34132 r8192 d23212 u65536
[    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 129412
[    0.000000] Kernel command line: console=ttyS0,115200n8 root=/dev/mmcblk0p2 ro rootfstype=ext4 roc
[    0.000000] Unknown kernel command line parameters "fixrtc", will be passed to user space.
[    0.000000] Dentry cache hash table entries: 65536 (order: 6, 262144 bytes, linear)
[    0.000000] Inode-cache hash table entries: 32768 (order: 5, 131072 bytes, linear)
[    0.000000] mem auto-init: stack:all(zero), heap alloc:off, heap free:off
[    0.000000] Memory: 484820K/522240K available (10240K kernel code, 1060K rwdata, 2316K rodata, 10)
[    0.000000] trace event string verifier disabled
[    0.000000] rcu: Hierarchical RCU implementation.
[    0.000000] rcu:     RCU event tracing is enabled.
[    0.000000] rcu:     RCU restricting CPUs from NR_CPUS=2 to nr_cpu_ids=1.
[    0.000000] rcu: RCU calculated value of scheduler-enlistment delay is 10 jiffies.
[    0.000000] rcu: Adjusting geometry for rcu_fanout_leaf=16, nr_cpu_ids=1
[    0.000000] NR_IRQS: 16, nr_irqs: 16, preallocated irqs: 16
[    0.000000] IRQ: Found an INTC at 0x(ptrval) (revision 5.0) with 128 interrupts
[    0.000000] rcu: srcu_init: Setting srcu_struct sizes based on contention.
[    0.000000] TI gptimer clocksource: always-on /ocp/interconnect@44c00000/segment@200000/target-mo0
[    0.000002] sched_clock: 32 bits at 24MHz, resolution 41ns, wraps every 89478484971ns
[    0.000027] clocksource: dmtimer: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 796358519s
[    0.000426] TI gptimer clockevent: 24000000 Hz at /ocp/interconnect@48000000/segment@0/target-mod0
[    0.002045] Console: colour dummy device 80x30
[    0.002128] Calibrating delay loop... 996.14 BogoMIPS (lpj=4980736)
[    0.090469] CPU: Testing write buffer coherency: ok
[    0.090570] CPU0: Spectre v2: using BPIALL workaround
[    0.090581] pid_max: default: 32768 minimum: 301
[    0.090712] LSM: Security Framework initializing
[    0.090894] Mount-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.090914] Mountpoint-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.092539] CPU0: thread -1, cpu 0, socket -1, mpidr 0
[    0.093714] Setting up static identity map for 0x80100000 - 0x80100078
[    0.093988] rcu: Hierarchical SRCU implementation.
[    0.093995] rcu:     Max phase no-delay instances is 1000.
[    0.094664] smp: Bringing up secondary CPUs ...
[    0.094673] smp: Brought up 1 node, 1 CPU
[    0.094682] SMP: Total of 1 processors activated (996.14 BogoMIPS).
[    0.094690] CPU: All CPU(s) started in SVC mode.
[    0.095506] devtmpfs: initialized
[    0.109254] VFP support v0.3: implementor 41 architecture 3 part 30 variant c rev 3
[    0.109581] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 191126044s
[    0.109615] futex hash table entries: 256 (order: 2, 16384 bytes, linear)
[    0.111240] pinctrl core: initialized pinctrl subsystem
[    0.113184] NET: Registered PF_NETLINK/PF_ROUTE protocol family
[    0.116062] DMA: preallocated 256 KiB pool for atomic coherent allocations
[    0.116627] audit: initializing netlink subsys (disabled)
[    0.117828] thermal_sys: Registered thermal governor 'fair_share'
[    0.117845] thermal_sys: Registered thermal governor 'step_wise'
[    0.117850] thermal_sys: Registered thermal governor 'user_space'
[    0.117993] audit: type=2000 audit(0.110:1): state=initialized audit_enabled=0 res=1
[    0.118024] cpuidle: using governor menu
[    0.135998] /ocp/interconnect@44c00000/segment@200000/target-module@b000/i2c@0/tda19988@70: Fixed0
[    0.136879] /ocp/interconnect@48000000/segment@300000/target-module@e000/lcdc@0: Fixed dependency0
[    0.138737] No ATAGs?
[    0.138751] hw-breakpoint: debug architecture 0x4 unsupported.
[    0.147240] kprobes: kprobe jump-optimization is enabled. All kprobes are optimized if possible.
[    0.164547] iommu: Default domain type: Translated 
[    0.164556] iommu: DMA domain TLB invalidation policy: strict mode 
[    0.165976] SCSI subsystem initialized
[    0.166364] pps_core: LinuxPPS API ver. 1 registered
[    0.166374] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giometti <giometti@linux.>
[    0.166397] PTP clock support registered
[    0.167887] clocksource: Switched to clocksource dmtimer
[    0.168490] VFS: Disk quotas dquot_6.6.0
[    0.168562] VFS: Dquot-cache hash table entries: 1024 (order 0, 4096 bytes)
[    0.188028] NET: Registered PF_INET protocol family
[    0.188344] IP idents hash table entries: 8192 (order: 4, 65536 bytes, linear)
[    0.189700] tcp_listen_portaddr_hash hash table entries: 512 (order: 0, 4096 bytes, linear)
[    0.189740] Table-perturb hash table entries: 65536 (order: 6, 262144 bytes, linear)
[    0.189764] TCP established hash table entries: 4096 (order: 2, 16384 bytes, linear)
[    0.189809] TCP bind hash table entries: 4096 (order: 4, 65536 bytes, linear)
[    0.189909] TCP: Hash tables configured (established 4096 bind 4096)
[    0.190033] UDP hash table entries: 256 (order: 1, 8192 bytes, linear)
[    0.190058] UDP-Lite hash table entries: 256 (order: 1, 8192 bytes, linear)
[    0.190229] NET: Registered PF_UNIX/PF_LOCAL protocol family
[    0.198587] RPC: Registered named UNIX socket transport module.
[    0.198606] RPC: Registered udp transport module.
[    0.198610] RPC: Registered tcp transport module.
[    0.198614] RPC: Registered tcp NFSv4.1 backchannel transport module.
[    0.200060] Initialise system trusted keyrings
[    0.201083] workingset: timestamp_bits=14 max_order=17 bucket_order=3
[    0.202325] NFS: Registering the id_resolver key type
[    0.202393] Key type id_resolver registered
[    0.202399] Key type id_legacy registered
[    0.202450] jffs2: version 2.2. (NAND) (SUMMARY)  © 2001-2006 Red Hat, Inc.
[    0.202847] Key type asymmetric registered
[    0.202859] Asymmetric key parser 'x509' registered
[    0.202930] io scheduler mq-deadline registered
[    0.202940] io scheduler kyber registered
[    0.212363] Serial: 8250/16550 driver, 6 ports, IRQ sharing enabled
[    0.252875] brd: module loaded
[    0.290217] loop: module loaded
[    0.291012] mtdoops: mtd device (mtddev=name/number) must be supplied
[    0.293685] UDC core: g_ether: couldn't find an available UDC
[    0.293699] i2c_dev: i2c /dev entries driver
[    0.293733] Driver for 1-wire Dallas network protocol.
[    0.294633] cpuidle: enable-method property 'ti,am3352' found operations
[    0.295028] sdhci: Secure Digital Host Controller Interface driver
[    0.295036] sdhci: Copyright(c) Pierre Ossman
[    0.295202] sdhci-pltfm: SDHCI platform and OF driver helper
[    0.295453] ledtrig-cpu: registered to indicate activity on CPUs
[    0.296178] Initializing XFRM netlink socket
[    0.296382] NET: Registered PF_INET6 protocol family
[    0.298229] Segment Routing with IPv6
[    0.298295] In-situ OAM (IOAM) with IPv6
[    0.298389] sit: IPv6, IPv4 and MPLS over IPv4 tunneling driver
[    0.299280] NET: Registered PF_PACKET protocol family
[    0.299306] NET: Registered PF_KEY protocol family
[    0.299432] Key type dns_resolver registered
[    0.299534] ThumbEE CPU extension supported.
[    0.299549] Registering SWP/SWPB emulation handler
[    0.299987] omap_voltage_late_init: Voltage driver support not added
[    0.300323] SmartReflex Class3 initialized
[    0.301239] Loading compiled-in X.509 certificates
[    0.385819] /ocp/interconnect@44c00000/segment@200000/target-module@b000/i2c@0/tda19988@70: Fixed0
[    0.391297] /ocp/interconnect@44c00000/segment@200000/target-module@b000/i2c@0/tda19988@70: Fixed0
[    0.398073] /ocp/interconnect@44c00000/segment@200000/target-module@b000/i2c@0/tda19988@70: Fixed0
[    0.398829] /ocp/interconnect@44c00000/segment@200000/target-module@b000/i2c@0/tda19988@70: Fixed0
[    0.401529] /ocp/interconnect@44c00000/segment@200000/target-module@10000/scm@0/pinmux@800: Fixedn
[    0.402085] pinctrl-single 44e10800.pinmux: 142 pins, size 568
[    0.406016] ti-sysc: probe of 44e31000.target-module failed with error -16
[    0.424510] /ocp/interconnect@48000000/segment@300000/target-module@e000/lcdc@0: Fixed dependency0
[    0.431169] ti-sysc: probe of 48040000.target-module failed with error -16
[    0.440373] gpio gpiochip0: (gpio-0-31): not an immutable chip, please consider fixing it!
[    0.463034] OMAP GPIO hardware version 0.1
[    0.488872] omap_i2c 4819c000.i2c: bus 2 rev0.11 at 100 kHz
[    0.493446] gpio gpiochip1: (gpio-32-63): not an immutable chip, please consider fixing it!
[    0.495988] gpio gpiochip2: (gpio-64-95): not an immutable chip, please consider fixing it!
[    0.502007] /ocp/interconnect@48000000/segment@300000/target-module@e000/lcdc@0: Fixed dependency0
[    0.504212] /ocp/interconnect@48000000/segment@300000/target-module@e000/lcdc@0: Fixed dependency0
[    0.504794] /ocp/interconnect@44c00000/segment@200000/target-module@b000/i2c@0/tda19988@70: Fixed0
[    0.504930] /ocp/interconnect@48000000/segment@300000/target-module@e000/lcdc@0: Fixed dependency0
[    0.513880] sdhci-omap 481d8000.mmc: supply pbias not found, using dummy regulator
[    0.514326] sdhci-omap 481d8000.mmc: supply vqmmc not found, using dummy regulator
[    0.572199] davinci_mdio 4a101000.mdio: davinci mdio revision 1.6, bus freq 1000000
[    0.620040] davinci_mdio 4a101000.mdio: phy[0]: device 4a101000.mdio:00, driver SMSC LAN8710/LAN80
[    0.620377] cpsw-switch 4a100000.switch: initialized cpsw ale version 1.4
[    0.620390] cpsw-switch 4a100000.switch: ALE Table size 1024
[    0.620507] cpsw-switch 4a100000.switch: cpts: overflow check period 500 (jiffies)
[    0.620522] cpsw-switch 4a100000.switch: CPTS: ref_clk_freq:250000000 calc_mult:2147483648 calc_sc
[    0.620613] cpsw-switch 4a100000.switch: Detected MACID = d0:5f:b8:fa:06:53
[    0.622256] cpsw-switch 4a100000.switch: initialized (regs 0x4a100000, pool size 256) hw_ver:0019)
[    0.631644] debugfs: Directory '49000000.dma' with parent 'dmaengine' already present!
[    0.631678] edma 49000000.dma: TI EDMA DMA engine driver
[    0.680884] hw perfevents: enabled with armv7_cortex_a8 PMU driver, 5 counters available
[    0.682775] gpio gpiochip3: (gpio-96-127): not an immutable chip, please consider fixing it!
[    0.687967] l3-aon-clkctrl:0000:0: failed to disable
[    0.690110] printk: console [ttyS0] disabled
[    0.690316] 44e09000.serial: ttyS0 at MMIO 0x44e09000 (irq = 34, base_baud = 3000000) is a 8250
[    0.690728] printk: console [ttyS0] enabled
[    0.692220] printk: console [ttyS0] printing thread started
[    1.932498] tps65217-pmic: Failed to locate of_node [id: -1]
[    2.103802] tps65217-bl: Failed to locate of_node [id: -1]
[    2.119824] tps65217 0-0024: TPS65217 ID 0xe version 1.2
[    2.120415] /ocp/interconnect@48000000/segment@300000/target-module@e000/lcdc@0: Fixed dependency0
[    2.120611] /ocp/interconnect@44c00000/segment@200000/target-module@b000/i2c@0/tda19988@70: Fixed0
[    2.120840] omap_i2c 44e0b000.i2c: bus 0 rev0.11 at 400 kHz
[    2.122684] omap_gpio 44e07000.gpio: Could not set line 6 debounce to 200000 microseconds (-22)
[    2.122695] sdhci-omap 48060000.mmc: Got CD GPIO
[    2.122981] sdhci-omap 48060000.mmc: supply pbias not found, using dummy regulator
[    2.123439] sdhci-omap 48060000.mmc: supply vqmmc not found, using dummy regulator
[    2.128499] sdhci-omap 481d8000.mmc: supply pbias not found, using dummy regulator
[    2.138793] sdhci-omap 481d8000.mmc: supply vqmmc not found, using dummy regulator
[    2.166502] mmc1: SDHCI controller on 481d8000.mmc [481d8000.mmc] using External DMA
[    2.171166] mmc0: SDHCI controller on 48060000.mmc [48060000.mmc] using External DMA
[    2.231235] clk: Disabling unused clocks
[    2.232180] Waiting for root device /dev/mmcblk0p2...
[    2.381477] mmc0: new high speed SDHC card at address 0007
[    2.382894] mmcblk0: mmc0:0007 SD32G 29.0 GiB 
[    2.386190]  mmcblk0: p1 p2
[    2.402563] mmc1: new high speed MMC card at address 0001
[    2.403784] mmcblk1: mmc1:0001 MMC04G 3.60 GiB 
[    2.406284]  mmcblk1: p1
[    2.407407] mmcblk1boot0: mmc1:0001 MMC04G 2.00 MiB 
[    2.427372] mmcblk1boot1: mmc1:0001 MMC04G 2.00 MiB 
[    2.429344] mmcblk1rpmb: mmc1:0001 MMC04G 128 KiB, chardev (250:0)
[    2.463339] EXT4-fs (mmcblk0p2): mounted filesystem with ordered data mode. Quota mode: none.
[    2.463476] VFS: Mounted root (ext4 filesystem) readonly on device 179:2.
[    2.466477] devtmpfs: mounted
[    2.467932] Freeing unused kernel image (initmem) memory: 1024K
[    2.493002] Run /sbin/init as init process
[    3.016180] cpsw-switch 4a100000.switch: starting ndev. mode: dual_mac
[    3.108995] SMSC LAN8710/LAN8720 4a101000.mdio:00: attached PHY driver (mii_bus:phy_addr=4a101000)



 _   _  __       _    _     _                  
| | | |/ _| __ _| | _| |   (_)_ __  _   ___  __
| | | | |_ / _` | |/ / |   | | '_ \| | | \ \/ /
| |_| |  _| (_| |   <| |___| | | | | |_| |>  < 
 \___/|_|  \__,_|_|\_\_____|_|_| |_|\__,_/_/\_\
                                               



Welcome to UfakLinux :)
hevalakts@gmail.com


UfakLinux login:
